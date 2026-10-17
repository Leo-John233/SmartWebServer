#include "CoordinateInput.h"
#include "../../pages/Pages.common.h"

// 一次请求内校验并设置完整坐标，任一步失败均不继续启动移动或同步
void gotoCoordinatesGet() {
  const char *names[7] = {"ra_h", "ra_m", "ra_s", "dec_sign", "dec_d", "dec_m", "dec_s"};
  String parts[7]; const char *values[7];
  for (int i = 0; i < 7; ++i) { parts[i] = www.arg(names[i]); values[i] = parts[i].c_str(); }
  char ra[24], dec[24], reply[80] = "";
  String action = www.arg("coord_action");
  if ((!action.equals("go") && !action.equals("sync")) || !coordinateCommands(values, ra, dec)) {
    www.sendContent("invalid"); return;
  }
  if (!status.onStepFound || !onStep.commandBool(ra) || !onStep.commandBool(dec)) {
    www.sendContent("rejected"); return;
  }
  bool accepted;
  if (action.equals("sync")) accepted = onStep.commandBool(":CS#");
  else accepted = onStep.command(":MS#", reply) && reply[0] == '0';
  www.sendContent(accepted ? "ok" : "rejected");
}
