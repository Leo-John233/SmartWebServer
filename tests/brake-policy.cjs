const output = require('./test-output.cjs');
const fs = require('node:fs'), path = require('node:path'), cp = require('node:child_process');
const root = path.join(__dirname, '..');
const source = String.raw`
#include <cassert>
#include <iostream>
#include "src/libApp/boardIo/Policy.h"

ProtectionFeedback frame(uint32_t at, bool st = true, bool fault = false, bool valid = true) {
  ProtectionFeedback f;
  f.sampledAt = at; f.driverValid = valid; f.driverFault = fault; f.standstill = st;
  return f;
}
void feed(BoardIoPolicy &p, uint32_t at, bool st = true, bool fault = false, bool valid = true) {
  p.feedback(frame(at, st, fault, valid), at); p.advanceRelease(at);
}
void start(BoardIoPolicy &p, uint32_t at = 10) {
  p.configure(true, true, at);
  assert(!p.releaseBrake() && !p.motionAllowed());
  feed(p, at); feed(p, at + 499); assert(!p.releaseBrake());
  feed(p, at + 500); assert(p.releaseBrake() && p.motionAllowed());
}
ProtectionStopResult sent() { ProtectionStopResult r; r.haltSent = r.trackingOffAccepted = true; return r; }

int main() {
  BoardIoPolicy p; start(p);
  // 正常运行时单独 ST 不抱闸，也不要求重新启动跟踪。
  feed(p, 750); assert(p.releaseBrake() && !p.stopRequired());
  feed(p, 800, true, true);
  assert(p.releaseBrake() && !p.motionAllowed() && p.stopRequired());
  p.stopResult(sent(), 900);
  feed(p, 900, true, true); feed(p, 1000, true, true);
  feed(p, 1249, true, true); assert(p.releaseBrake());
  feed(p, 1250, true, true); assert(!p.releaseBrake());
  // 故障恢复、重开保护均不能自行解除持久锁定。
  feed(p, 1500); feed(p, 2000); assert(!p.releaseBrake() && !p.motionAllowed());
  p.configure(true, false, 2000); assert(!p.resetFault(2000));
  p.configure(true, true, 2001); feed(p, 2250); assert(!p.resetFault(2250));
  feed(p, 2750); assert(p.resetFault(2750) && p.motionAllowed() && p.releaseBrake());
  assert(!p.lastStopResult().complete() && !p.stopRequired());

  // 关闭保护走相同停止流程，关闭期间仍继续采样。
  p.configure(true, false, 2800); assert(p.releaseBrake() && p.stopRequired() && !p.motionAllowed());
  p.stopResult(sent(), 2800); feed(p, 2900); feed(p, 3150);
  assert(!p.releaseBrake() && p.feedbackDue(3400));
  p.checkFeedback(false, 4000); assert(!p.releaseBrake() && !p.stopRequired());
  assert(p.faultReason() == BrakeFault::ProtectionOff);

  // 失联、超时和无效反馈保持高电平，停止只发一次；没有固定超时抱闸。
  for (int failure = 0; failure < 3; failure++) {
    BoardIoPolicy q; start(q);
    if (failure == 0) q.checkFeedback(false, 600);
    if (failure == 1) q.checkFeedback(true, 1511);
    if (failure == 2) feed(q, 600, false, false, false);
    assert(q.releaseBrake() && !q.motionAllowed() && q.stopRequired());
    q.stopResult(sent(), 2000);
    for (uint32_t at = 2250; at < 10000; at += 250) feed(q, at, false);
    assert(q.releaseBrake() && !q.stopRequired() && !q.resetFault(9999));
    q.checkFeedback(false, 11000); assert(q.releaseBrake() && !q.stopRequired());
  }

  // 中断、无效、重复、旧帧不能凑成停止确认。
  BoardIoPolicy q; start(q); feed(q, 600, true, true); q.stopResult(sent(), 601);
  feed(q, 700); feed(q, 800, false, false, false); feed(q, 950); assert(q.releaseBrake());
  q.feedback(frame(950), 1200); assert(q.releaseBrake());
  feed(q, 1000, false); feed(q, 1100); feed(q, 1349); assert(q.releaseBrake());
  feed(q, 1350); assert(!q.releaseBrake());
  q.checkFeedback(false, 1400); assert(!q.releaseBrake());

  // 发送失败只保留电平；必须人工重试，后续 ST 不替代发送结果。
  BoardIoPolicy failed; start(failed); feed(failed, 600, true, true);
  ProtectionStopResult partial; partial.haltSent = true; failed.stopResult(partial, 650);
  feed(failed, 700); feed(failed, 1200); assert(failed.releaseBrake() && !failed.stopRequired());
  assert(failed.retryStop() && failed.stopRequired()); failed.stopResult(sent(), 1250);
  feed(failed, 1300); feed(failed, 1550); assert(!failed.releaseBrake());

  // 恢复保存的故障时保持抱闸，只有人工复位能清除；复位不发运动命令。
  BoardIoPolicy boot; boot.configure(true, true, 0); boot.restoreFault(BrakeFault::Driver);
  assert(!boot.releaseBrake() && !boot.stopRequired()); feed(boot, 10); feed(boot, 510);
  assert(!boot.releaseBrake() && boot.resetEligible(510)); assert(boot.resetFault(510));

  // 上电缺失/无效反馈始终抱闸，正常状态必须重新连续保持 500ms。
  BoardIoPolicy startup; startup.configure(true, true, 0);
  startup.checkFeedback(false, 1000); feed(startup, 1100, false, false, false);
  assert(!startup.releaseBrake() && !startup.motionAllowed());
  feed(startup, 1200); feed(startup, 2201); assert(!startup.releaseBrake());
  feed(startup, 2701); assert(startup.releaseBrake());

  // 停止确认和正常计时均兼容毫秒计数回绕。
  BoardIoPolicy wrap; start(wrap, 0xfffffc00u);
  feed(wrap, 0xffffff00u, true, true); wrap.stopResult(sent(), 0xffffff10u);
  feed(wrap, 0xffffff20u, true, true); feed(wrap, 0x20u, true, true);
  assert(!wrap.releaseBrake() && !wrap.motionAllowed());
  std::cout << "PASS: startup gating, unified stop, fresh ST, level hold, manual reset, retry, saved fault and rollover\n";
}
`;
const work = fs.mkdtempSync(path.join(output, 'brake-policy-'));
const file = path.join(work, 'test.cpp'), exe = path.join(work, 'test.exe');
fs.writeFileSync(file, source);
cp.execFileSync(process.env.CXX || 'g++', ['-std=c++11', '-Wall', '-Wextra', '-Werror', '-I', root,
  file, path.join(root, 'src/libApp/boardIo/Policy.cpp'), '-o', exe], {stdio:'pipe'});
process.stdout.write(cp.execFileSync(exe, {encoding:'utf8'}));
