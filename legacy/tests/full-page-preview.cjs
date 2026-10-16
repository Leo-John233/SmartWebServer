// 预览使用实际控件模板，设备条件与数值采用模拟数据
module.exports=({templates:t,defs:d,tile,h,motion,axis})=>{
  const fmt=s=>s.replace(/%%|%[-+\d.]*l?[sdfu]/g,m=>m==='%%'?'%':m.endsWith('s')?'0':'1');
  const controls=(file,omit=[])=>Object.entries(t[file]).filter(([key])=>!omit.includes(key)&&!key.startsWith('html_script')).map(([,s])=>fmt(s)).join('');
  const group=(name,body)=>'<section><div class="sws-section-head"><h2 class="sws-section-title">'+name+'</h2></div><div class="sws-settings-grid">'+body+'</div></section>';
  const settings=(body,label=d.L_CONTROLS)=>h('html_collapsable_beg').replace('%s',label)+body+h('html_collapsable_end');
  // 模拟明确的设备状态，避免把互斥模板直接拼在同一张卡片上
  const fill=(value,...args)=>value.replace(/%[-+\d.]*l?[sdfuc]/g,()=>String(args.shift()??0));
  const status=t['index/StatusTile.h'],ambient=t['index/AmbientTile.h'];
  const boardIo=require('./board-io-backend.cjs')({preview:true});
  const metrics=fill(status.html_indexGeneralError,d.L_NONE)+fill(status.html_indexSignalStrength,'-55 dBm')+fill(status.html_indexTemp,d.L_INTERNAL_TEMP,'m','32 °C')+[[d.L_AMBIENT_TEMPERATURE,'t','20.6 °C'],[d.L_PRESSURE,'p','1012 hPa'],[d.L_HUMIDITY,'h','58%'],[d.L_DEW_POINT,'d','12.1 °C']].map(args=>fill(ambient.html_indexTPHD,...args)).join('');
  const overview='<div class="sws-overview-grid">'+tile(d.L_STATE,'<div class="sws-metrics">'+metrics+'</div>'+settings(boardIo+'<form method="get">'+fill(status.html_indexMountType,1)+'<button type="submit">'+d.L_UPLOAD+'</button></form><hr><button type="button" onpointerdown="s(\'boot\',\'reset\')">'+d.L_RESET+'</button>'+status.html_resetNotes,d.L_SETTINGS))+'</div>';
  const controller='<div class="sws-controller">'+overview+'<section><div class="sws-section-head"><h2 class="sws-section-title">'+d.L_DRIVER+'</h2></div><div class="sws-axis-grid">'+[1,2,3].map(n=>axis.replaceAll('a1p','a'+n+'p').replace('dvr_stat0','dvr_stat'+(n-1)).replace('Axis1 RA/Azm',n===1?'Axis1 RA/Azm':n===2?'Axis2 Dec/Alt':'Axis3 '+d.L_ROTATOR)).join('')+'</div></section><details class="sws-more" open><summary>'+d.L_ADV_SET_TITLE+'</summary><form method="get" action="/index.htm"><button name="advanced" value="enable">'+d.L_ADV_ENABLE+'</button><button name="advanced" value="disable">'+d.L_ADV_DISABLE+'</button></form>'+controls('index/Index.h')+'</details></div>';
  // 直接运行固件卡片生成函数，完整保留动态输入项与提交按钮
  const actual=require('./mount-page.cjs')();
  const gt=t['mount/GotoTile.h'];
  const go=actual.Goto;
  const layout=t['ui/Layouts.h'];
  const mount=layout.html_layout_mount_begin+actual.Guide+layout.html_layout_mount_tracking+actual.Tracking+layout.html_layout_mount_location+actual.Site+actual.Limits+layout.html_layout_mount_target+actual.Goto+layout.html_layout_mount_home+actual.HomePark+layout.html_layout_mount_library+actual.Library+layout.html_layout_mount_advanced+actual.Align+actual.Pec+tile(d.L_COORD_ORIGIN_TITLE,controls('mount/CoordinateTile.h'))+layout.html_layout_mount_end;
  const optional={};
  // 调焦器与旋转器执行真实卡片函数，保留条件判断与完整表单
  for(const kind of ['focuser','rotator']){
    const cards=require('./accessory-page.cjs')({kind});
    optional['/'+kind+'.htm']=fill(t['htmlScripts.h'].html_script_ajax_get,kind+'-ajax-get.txt')+t['htmlScripts.h'].html_script_ajax_shortcuts+'<script>function autoFastRun(){}</script><div class="sws-device-layout sws-accessory-layout">'+(kind==='focuser'?'<section class="sws-device-selector">'+cards.Select+'</section>':'')+'<div class="sws-accessory-grid"><div class="sws-accessory-main"><section class="sws-device-primary sws-accessory-motion">'+cards.Slewing+'</section><section class="sws-accessory-home">'+cards.Home+'</section></div><section class="sws-accessory-settings">'+(kind==='focuser'?cards.BacklashTcf:cards.Backlash+cards.DeRotator)+'</section></div></div>';
  }
  optional['/enc.htm']=require('./encoders-page.cjs')();
  const auxiliary=require('./auxiliary-page.cjs')();
  optional['/auxiliary.htm']=auxiliary.body;
  return {controller,mount,optional,auxiliary};
};
