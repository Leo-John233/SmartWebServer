// -----------------------------------------------------------------------------------
// Ambient tile
#include "AmbientTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

#if DISPLAY_WEATHER == ON

// create the related webpage tile
void ambientTile(String &data)
{
  char temp[240] = "";

  // 环境指标直接并入控制器概览，保留原有显示开关与刷新标识
  // Ambient conditions
  snprintf_P(temp, sizeof(temp), html_indexTPHD, L_AMBIENT_TEMPERATURE, 't', state.siteTemperatureStr); data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_indexTPHD, L_PRESSURE, 'p', state.sitePressureStr); data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_indexTPHD, L_HUMIDITY, 'h', state.siteHumidityStr); data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_indexTPHD, L_DEW_POINT, 'd', state.siteDewPointStr); data.concat(temp);
  www.sendContentAndClear(data);



}

// use Ajax key/value pairs to pass related data to the web client in the background
void ambientTileAjax(String &data)
{
  data.concat(F("tphd_t|")); data.concat(state.siteTemperatureStr); data.concat("\n");
  data.concat(F("tphd_p|")); data.concat(state.sitePressureStr); data.concat("\n");
  data.concat(F("tphd_h|")); data.concat(state.siteHumidityStr); data.concat("\n");
  data.concat(F("tphd_d|")); data.concat(state.siteDewPointStr); data.concat("\n");

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void ambientTileGet()
{
  String v;
}

#endif
