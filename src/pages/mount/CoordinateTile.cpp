// -----------------------------------------------------------------------------------
// Coordinate basis tile
#include "CoordinateTile.h"

#if DISPLAY_COORDINATE_ORIGIN == ON

#include "../Pages.common.h"
#include "../../api/SemanticActions.h"

// create the related webpage tile
void coordinateTile(String &data)
{
  char temp[240] = "";

  snprintf_P(temp, sizeof(temp), html_tile_text_beg, "22em", "15em", L_COORD_ORIGIN_TITLE);
  data.concat(temp);
  data.concat(F("<br /><hr>"));
  data.concat(FPSTR(html_coordinateBasis));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// pass related data back to OnStep
void coordinateTileGet()
{
  String value = www.arg("ob");
  if (!value.equals(EmptyStr)) performMountPecAction("ob", value);
}

#endif
