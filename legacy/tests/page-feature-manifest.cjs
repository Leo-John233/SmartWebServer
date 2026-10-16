// 原有页面显示、轮询和命令入口清单
module.exports = {
  "index/AmbientTile.cpp": [
    "ambientTile",
    "ambientTileAjax",
    "ambientTileGet"
  ],
  "index/AxisTile.cpp": [
    "axisTile",
    "axisTileAjax",
    "axisTileGet"
  ],
  "index/Index.cpp": [
    "ambientTile",
    "statusTile",
    "axisTile",
    "servoTile",
    "stallGuardTile",
    "ambientTileAjax",
    "statusTileAjax",
    "axisTileAjax",
    "servoTileAjax",
    "stallGuardTileAjax",
    "ambientTileGet",
    "statusTileGet",
    "axisTileGet",
    "servoTileGet",
    "stallGuardTileGet"
  ],
  "index/ServoTile.cpp": [
    "servoTile",
    "servoTileAjax",
    "servoTileGet"
  ],
  "index/StallGuardTile.cpp": [
    "stallGuardTile",
    "stallGuardTileAjax",
    "stallGuardTileGet"
  ],
  "index/StatusTile.cpp": [
    "statusTile",
    "statusTileAjax",
    "statusTileGet"
  ],
  "mount/AlignTile.cpp": [
    "alignTile",
    "alignTileAjax",
    "alignTileGet"
  ],
  "mount/CoordinateTile.cpp": [
    "coordinateTile",
    "coordinateTileGet"
  ],
  "mount/GotoTile.cpp": [
    "gotoTile",
    "gotoTileAjax",
    "gotoTileGet"
  ],
  "mount/GuideTile.cpp": [
    "guideTile",
    "guideTileAjax",
    "guideTileGet"
  ],
  "mount/HomeParkTile.cpp": [
    "homeParkTile",
    "homeParkTileAjax",
    "homeParkTileGet"
  ],
  "mount/LibraryTile.cpp": [
    "libraryTile",
    "libraryTileAjax",
    "libraryTileGet"
  ],
  "mount/LimitsTile.cpp": [
    "limitsTile",
    "limitsTileAjax",
    "limitsTileGet"
  ],
  "mount/Mount.cpp": [
    "siteTile",
    "homeParkTile",
    "alignTile",
    "gotoTile",
    "libraryTile",
    "guideTile",
    "trackingTile",
    "pecTile",
    "coordinateTile",
    "limitsTile",
    "siteTileAjax",
    "homeParkTileAjax",
    "alignTileAjax",
    "gotoTileAjax",
    "libraryTileAjax",
    "guideTileAjax",
    "trackingTileAjax",
    "pecTileAjax",
    "limitsTileAjax",
    "siteTileGet",
    "homeParkTileGet",
    "coordinateTileGet",
    "alignTileGet",
    "gotoTileGet",
    "libraryTileGet",
    "guideTileGet",
    "trackingTileGet",
    "pecTileGet",
    "limitsTileGet"
  ],
  "mount/PecTile.cpp": [
    "pecTile",
    "pecTileAjax",
    "pecTileGet"
  ],
  "mount/SiteTile.cpp": [
    "siteTile",
    "siteTileAjax",
    "siteTileGet"
  ],
  "mount/TrackingTile.cpp": [
    "trackingTile",
    "trackingTileAjax",
    "trackingTileGet"
  ],
  "focuser/BacklashTcfTile.cpp": [
    "focuserBacklashTcfTile",
    "focuserBacklashTcfTileAjax",
    "focuserBacklashTcfTileGet"
  ],
  "focuser/Focuser.cpp": [
    "focuserSelectTile",
    "focuserHomeTile",
    "focuserSlewingTile",
    "focuserBacklashTcfTile",
    "focuserSelectTileAjax",
    "focuserHomeTileAjax",
    "focuserSlewingTileAjax",
    "focuserBacklashTcfTileAjax",
    "focuserSelectTileGet",
    "focuserHomeTileGet",
    "focuserSlewingTileGet",
    "focuserBacklashTcfTileGet"
  ],
  "focuser/HomeTile.cpp": [
    "focuserHomeTile",
    "focuserHomeTileAjax",
    "focuserHomeTileGet"
  ],
  "focuser/SelectTile.cpp": [
    "focuserSelectTile",
    "focuserSelectTileAjax",
    "focuserSelectTileGet"
  ],
  "focuser/SlewingTile.cpp": [
    "focuserSlewingTile",
    "focuserSlewingTileAjax",
    "focuserSlewingTileGet"
  ],
  "rotator/BacklashTile.cpp": [
    "rotatorBacklashTile",
    "rotatorBacklashTileAjax",
    "rotatorBacklashTileGet"
  ],
  "rotator/DeRotatorTile.cpp": [
    "deRotatorTile",
    "deRotatorTileAjax",
    "deRotatorTileGet"
  ],
  "rotator/HomeTile.cpp": [
    "rotatorHomeTile",
    "rotatorHomeTileAjax",
    "rotatorHomeTileGet"
  ],
  "rotator/Rotator.cpp": [
    "rotatorHomeTile",
    "rotatorSlewingTile",
    "deRotatorTile",
    "rotatorBacklashTile",
    "rotatorHomeTileAjax",
    "rotatorSlewingTileAjax",
    "deRotatorTileAjax",
    "rotatorBacklashTileAjax",
    "rotatorHomeTileGet",
    "rotatorSlewingTileGet",
    "deRotatorTileGet",
    "rotatorBacklashTileGet"
  ],
  "rotator/SlewingTile.cpp": [
    "rotatorSlewingTile",
    "rotatorSlewingTileAjax",
    "rotatorSlewingTileGet"
  ],
  "encoders/AxisTile.cpp": [
    "encAxisTile",
    "encAxisTileAjax",
    "encAxisTileGet"
  ],
  "encoders/Encoders.cpp": [
    "syncTile",
    "encAxisTile",
    "syncTileAjax",
    "encAxisTileAjax",
    "syncTileGet",
    "encAxisTileGet"
  ],
  "encoders/SyncTile.cpp": [
    "syncTile",
    "syncTileAjax",
    "syncTileGet"
  ],
  "auxiliary/Auxiliary.cpp": [],
  "network/Network.cpp": [],
  "network/NetworkWifi.cpp": []
}
;
