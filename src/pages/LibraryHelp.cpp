// 天体库帮助按说明、样例和类型分区展示
#include "LibraryHelp.h"
#include "Page.h"
#include "Pages.common.h"

void handleLibraryHelp() {
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/html", String());
  pageHead();
  String data;
  data.concat(FPSTR(html_body_begin));
  www.sendContentAndClear(data);
  data.concat(F("<main id='sws-nav-ready' class='sws-document'><header><h1>" L_PAGE_LIBRARY "</h1><a href='/mount.htm' target='_top'>" L_PAGE_MOUNT "</a></header><section>"));
  data.concat(FPSTR(html_libCatalogHelp1));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_libCatalogHelp2));
  data.concat(F("</section></main></body></html>"));
  www.sendContentAndClear(data);
  www.sendContent("");
}
