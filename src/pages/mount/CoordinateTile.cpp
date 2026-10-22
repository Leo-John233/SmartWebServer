// -----------------------------------------------------------------------------------
// Coordinate basis tile
#include "CoordinateTile.h"
#include "../../extensions/ui/Template.h"

#if DISPLAY_COORDINATE_ORIGIN == ON

#include "../Pages.common.h"

// create the related webpage tile
void coordinateTile(String &data)
{
  appendPageFormat(data, html_tile_text_beg, "22em", "15em", L_COORD_ORIGIN_TITLE);
  data.concat(F("<br /><hr>"));
  data.concat(FPSTR(html_coordinateBasis));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// pass related data back to OnStep
void coordinateTileGet()
{
  String v = www.arg("ob");
  if (v.equals("set")) onStep.commandBool(":SEO#");
}

#endif
