// -----------------------------------------------------------------------------------
// Focuser Home tile
#include "HomeTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"
#include "../../api/SemanticActions.h"

// create the related webpage tile
void focuserHomeTile(String &data)
{
  char temp[240] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "13em", L_HOMING);
  data.concat(temp);
  data.concat(F("<br /><hr>"));

  data.concat(FPSTR(html_focuserHome));

  data.concat(F("<hr>"));

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void focuserHomeTileAjax(String &data)
{
  UNUSED(data);
}

// pass related data back to OnStep
extern void focuserHomeTileGet()
{
  String value = www.arg("dr");
  if (!value.equals(EmptyStr)) performFocuserAction("dr", value);
}
