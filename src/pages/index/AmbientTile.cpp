// -----------------------------------------------------------------------------------
// Ambient tile
#include "AmbientTile.h"
#include "../../extensions/ui/Template.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

#if DISPLAY_WEATHER == ON

// create the related webpage tile
void ambientTile(String &data)
{
  // 环境指标直接并入控制器概览，保留原有显示开关与刷新标识
  // Ambient conditions
  appendPageFormat(data, html_indexTPHD, L_AMBIENT_TEMPERATURE, 't', state.siteTemperatureStr);
  appendPageFormat(data, html_indexTPHD, L_PRESSURE, 'p', state.sitePressureStr);
  appendPageFormat(data, html_indexTPHD, L_HUMIDITY, 'h', state.siteHumidityStr);
  appendPageFormat(data, html_indexTPHD, L_DEW_POINT, 'd', state.siteDewPointStr);
  www.sendContentAndClear(data);



}

// use Ajax key/value pairs to pass related data to the web client in the background
void ambientTileAjax(String &data)
{
  data.concat(F("tphd_t|")); data.concat(state.siteTemperatureStr); data.concat("\n");
  data.concat(F("tphd_p|")); data.concat(state.sitePressureStr); data.concat("\n");
  data.concat(F("tphd_h|")); data.concat(state.siteHumidityStr); data.concat("\n");
  data.concat(F("tphd_d|")); data.concat(state.siteDewPointStr); data.concat("\n");

  if (data.length() >= 768) { www.sendContentAndClear(data); }
}

// pass related data back to OnStep
extern void ambientTileGet()
{
  String v;
}

#endif
