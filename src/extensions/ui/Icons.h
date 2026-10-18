#pragma once

// 路径集中维护，随原生页头直接输出，避免等待外部图标资源后再次绘制
#define SWS_NAV_ICON(name) "<svg viewBox='0 0 32 32' aria-hidden='true' focusable='false'>" SWS_NAV_PATH_##name "</svg>"

#define SWS_NAV_PATH_index "<path d=\"M8 5h16a3 3 0 0 1 3 3v16a3 3 0 0 1-3 3H8a3 3 0 0 1-3-3V8a3 3 0 0 1 3-3Zm3 5v12h10V10Z\" fill-rule=\"evenodd\"/><path d=\"M9 1h3v4H9zm6 0h3v4h-3zm6 0h3v4h-3zM9 27h3v4H9zm6 0h3v4h-3zm6 0h3v4h-3zM1 9h4v3H1zm0 6h4v3H1zm0 6h4v3H1zM27 9h4v3h-4zm0 6h4v3h-4zm0 6h4v3h-4z\" opacity=\".72\"/><rect x=\"13\" y=\"12\" width=\"6\" height=\"8\" rx=\"1\"/>"
#define SWS_NAV_PATH_mount "<path d=\"M13 20h5l1 5 4 4H9l4-5Z\" opacity=\".78\"/><g transform=\"rotate(-28 16 14)\"><path d=\"M12 8h13c5 0 5 12 0 12H12Zm13.5 1.8c-2.6 0-2.6 8.4 0 8.4s2.6-8.4 0-8.4Z\" fill-rule=\"evenodd\"/><path d=\"M7 10h4v8H7a1 1 0 0 1-1-1v-6a1 1 0 0 1 1-1Z\" opacity=\".8\"/><path d=\"M2 12h3v4H2a1 1 0 0 1-1-1v-2a1 1 0 0 1 1-1Z\" opacity=\".65\"/></g>"
#define SWS_NAV_PATH_focuser "<path d=\"M3 2h7v3H5v5H2V3a1 1 0 0 1 1-1Zm19 0h7a1 1 0 0 1 1 1v7h-3V5h-5ZM2 22h3v5h5v3H3a1 1 0 0 1-1-1Zm25 0h3v7a1 1 0 0 1-1 1h-7v-3h5Z\" opacity=\".72\"/><path d=\"M16 6a10 10 0 1 0 0 20 10 10 0 0 0 0-20Zm0 4a6 6 0 1 1 0 12 6 6 0 0 1 0-12Z\" fill-rule=\"evenodd\"/><circle cx=\"16\" cy=\"16\" r=\"3\"/>"
#define SWS_NAV_PATH_rotator "<path d=\"M6.2 8.4A12 12 0 0 1 27 10h4L25 17l-6-7h4a8 8 0 0 0-13.7.8Z\"/><path d=\"M25.8 23.6A12 12 0 0 1 5 22H1l6-7 6 7H9a8 8 0 0 0 13.7-.8Z\" opacity=\".72\"/><circle cx=\"16\" cy=\"16\" r=\"3\"/>"
#define SWS_NAV_PATH_auxiliary "<path d=\"M8 3h16a6 6 0 0 1 0 12H8A6 6 0 0 1 8 3Zm0 3a3 3 0 1 0 0 6 3 3 0 0 0 0-6Z\" fill-rule=\"evenodd\"/><path d=\"M8 18h16a6 6 0 0 1 0 12H8a6 6 0 0 1 0-12Zm16 3a3 3 0 1 0 0 6 3 3 0 0 0 0-6Z\" fill-rule=\"evenodd\" opacity=\".72\"/>"
#define SWS_NAV_PATH_enc "<path d=\"M16 2a14 14 0 1 0 0 28 14 14 0 0 0 0-28Zm-1 2h2v4h-2ZM5.1 8l1.4-1.4 2.8 2.8L8 10.8ZM4 15h4v2H4Zm3 10-1.4-1.4 2.8-2.8L10 22.2ZM15 24h2v4h-2Zm10-1-1.4 1.4-2.8-2.8 1.4-1.4ZM24 15h4v2h-4Zm.9-7-2.8 2.8-1.4-1.4 2.8-2.8ZM16 11a5 5 0 1 0 0 10 5 5 0 0 0 0-10Z\" fill-rule=\"evenodd\"/><circle cx=\"16\" cy=\"16\" r=\"2.5\" opacity=\".72\"/>"
#define SWS_NAV_PATH_net "<path d=\"M1 9a23 23 0 0 1 30 0l-3 3a19 19 0 0 0-24 0Zm5 6a15 15 0 0 1 20 0l-3 3a11 11 0 0 0-14 0Zm5 6a8 8 0 0 1 10 0l-5 6Z\"/>"
#define SWS_NAV_PATH_more "<circle cx=\"6\" cy=\"16\" r=\"2.5\"/><circle cx=\"16\" cy=\"16\" r=\"2.5\"/><circle cx=\"26\" cy=\"16\" r=\"2.5\"/>"
