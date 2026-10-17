const testOutput = require('./test-output.cjs');
// 运行真实 PEC 状态处理，验证正常状态与读取失败时的文字。
const fs = require('node:fs');
const path = require('node:path');
const cp = require('node:child_process');
const source = fs.readFileSync(
  path.join(__dirname, '../SmartWebServer/src/pages/mount/PecTile.cpp'),
  'utf8',
);
const begin = source.indexOf('void pecTileAjax(String &data)');
const end = source.indexOf('// pass related data', begin);
const program =
  `
#include <string>
#include <cstring>
#include <cassert>
#include <iostream>
#define F(text) text
enum { MT_GEM = 1, MT_ALTAZM = 3 };
` +
  require('./locale-source.cjs')() +
  `
struct String : std::string {
  using std::string::string;
  void concat(const char *text) { append(text); }
};
struct Status { int mountType = MT_GEM; bool pecRecording = false; } status;
struct Device {
  const char *reply = "I"; bool available = true;
  bool command(const char *command, char *out) {
    assert(!strcmp(command, ":$QZ?#"));
    if (!available) return false;
    strcpy(out, reply); return true;
  }
} onStep;
struct Web {
  std::string output;
  void sendContentAndClear(String &data) { output += data; data.clear(); }
} www;
` +
  source.slice(begin, end) +
  `
void check(const char *expected) {
  String data; www.output.clear(); pecTileAjax(data);
  assert(www.output == std::string("pec_sta|") + expected + "\\n");
}
int main() {
  onStep.reply = "I"; check(L_PEC_IDLE);
  onStep.reply = "p"; check(L_PEC_WAIT_PLAY);
  onStep.reply = "P"; check(L_PEC_PLAYING);
  onStep.reply = "r"; check(L_PEC_WAIT_REC);
  onStep.reply = "R"; check(L_PEC_RECORDING);
  onStep.reply = "x"; check(L_PEC_UNK);
  onStep.available = false; check(L_PEC_UNK);
  status.mountType = MT_ALTAZM; check(L_PEC_UNK);
  status.mountType = MT_GEM; onStep.available = true;
  status.pecRecording = true; onStep.reply = "I";
  check(L_PEC_IDLE " (" L_PEC_EEWRITING ")");
  std::cout << "PASS: actual PEC states, unknown response, unavailable data and storage status";
}
`;
const directory = fs.mkdtempSync(path.join(testOutput, 'sws-pec-'));
const input = path.join(directory, 'pec.cpp'),
  executable = path.join(directory, 'pec.exe');
fs.writeFileSync(input, program);
cp.execFileSync(process.env.CXX || 'g++', ['-std=c++11', input, '-o', executable], {
  stdio: 'pipe',
});
process.stdout.write(cp.execFileSync(executable, [], { encoding: 'utf8' }));
