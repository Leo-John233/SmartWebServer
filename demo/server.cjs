// 仅在本机提供模拟设备，不连接串口或真实硬件
const http=require('node:http'),fs=require('node:fs'),path=require('node:path');
const {doc,state,head,shared,templates,auxiliary,resources,brakeAvailable}=require('./render.cjs');
const auxiliaryDevice=require('./auxiliary.cjs')(auxiliary);
const client=fs.readFileSync(path.join(__dirname,'simulation.js'),'utf8');
const pages=['/index.htm','/mount.htm','/net.htm','/focuser.htm','/rotator.htm','/auxiliary.htm','/enc.htm'];
const networks=[{ssid:'Redmi_Hotel',rssi:-48,open:false},{ssid:'Observatory',rssi:-62,open:false},{ssid:'Guest',rssi:-73,open:true},{ssid:'Home Wi-Fi',rssi:-55,open:false}];
let connectAt=0,scanAt=0,fail=false;
let encoderAutoSync=false;
const boardIoState={activity:true,protection:brakeAvailable,locked:false,manualUnlocked:false};
const server=http.createServer(async(req,res)=>{
  res.setHeader('Cache-Control','no-store');
  const url=new URL(req.url,'http://127.0.0.1');let body='';for await(const chunk of req){body+=chunk;if(body.length>65536){res.writeHead(413);res.end();return;}}
  const args=new URLSearchParams(body),send=(data,type='text/html; charset=utf-8')=>{res.setHeader('Content-Type',type);res.end(data);};
  if(url.pathname==='/sws-ui.css'||url.pathname==='/sws-ui.js'){
    res.setHeader('Cache-Control','public, max-age=31536000, immutable');
    return url.pathname.endsWith('.css')?send(resources.css,'text/css; charset=utf-8'):send(resources.js,'application/javascript; charset=utf-8');
  }
  if(url.pathname==='/demo.js')return send(client,'text/javascript; charset=utf-8');
  if(url.pathname==='/board-io'){
    if(req.method==='POST'){
      const action=args.get('action');
      if(action){
        if(!brakeAvailable||!['reset','retry-stop','manual-unlock'].includes(action)||(action!=='manual-unlock'&&!boardIoState.locked)||(action==='reset'&&!boardIoState.protection)){
          res.statusCode=409;return send('{}','application/json');
        }
        if(action==='manual-unlock'){boardIoState.manualUnlocked=true;boardIoState.protection=false;boardIoState.locked=false;}
        if(action==='reset')boardIoState.locked=false;
      }else{
        const setting=args.get('setting'),enabled=args.get('enabled');
        if(!['activity','protection'].includes(setting)||!['0','1'].includes(enabled)){res.statusCode=400;return send('{}','application/json');}
        if(setting==='protection'&&!brakeAvailable){res.statusCode=409;return send('{}','application/json');}
        if(setting==='protection'&&boardIoState.protection&&enabled==='0')boardIoState.locked=true;
        if(setting==='protection'&&enabled==='1')boardIoState.manualUnlocked=false;
        boardIoState[setting]=enabled==='1';
      }
    }
    // 演示服务模拟完成停止，不连接硬件，也不会因重开开关自动解锁
    return send(JSON.stringify({...boardIoState,showUnlock:boardIoState.locked,showStopRetry:false,message:boardIoState.manualUnlocked?'已手动解锁，自动锁定暂停':boardIoState.locked?'已抱闸，可手动解锁':boardIoState.protection?'状态正常，已松闸':'自动锁定关闭，已抱闸',fault:boardIoState.locked?'锁定原因：关闭保护':''}),'application/json; charset=utf-8');
  }
  if(url.pathname==='/net-wifi'){
    const action=url.searchParams.get('action');
    if(action==='scan'){if(!scanAt)scanAt=Date.now()+1000;const running=Date.now()<scanAt;if(!running)scanAt=0;return send(JSON.stringify({running,networks:running?[]:networks}),'application/json');}
    if(action==='connect'){
      state.messageTarget='sta';
      if(args.get('enabled')==='1'&&!args.get('ssid')){res.statusCode=400;return send(JSON.stringify({message:'请输入 Wi-Fi 名称'}),'application/json');}
      for(const k of ['ssid','ip','gw','sn'])if(args.has(k))state.sta[k]=args.get(k);
      for(const k of ['enabled','open','dhcp'])state.sta[k]=args.get(k)==='1';
      fail=args.get('password')==='wrongpass';state.connected=false;state.connecting=state.sta.enabled;state.failed=false;connectAt=Date.now()+2200;state.message=state.sta.enabled?'正在连接':'已关闭路由器连接';
    }
    if(action==='ap'){state.messageTarget='ap';for(const k of ['ssid','ip','gw','sn'])if(args.has(k))state.ap[k]=args.get(k);state.ap.enabled=args.get('enabled')==='1';state.ap.channel=Number(args.get('channel')||7);state.apActive=state.ap.enabled;state.apIp=state.ap.ip;state.apSsid=state.ap.ssid;state.message='热点设置已保存';}
    if(state.connecting&&Date.now()>=connectAt){state.connecting=false;state.connected=!fail;state.failed=fail;state.ssid=state.sta.ssid;state.ip=state.sta.dhcp?'192.168.31.229':state.sta.ip;state.message=fail?'密码错误，请重新输入':'已连接';}
    return send(JSON.stringify(state),'application/json');
  }
  if(url.pathname==='/mount-ajax-get.txt'&&['T1','T0'].includes(url.searchParams.get('dr')))return send('ok','text/plain');
  if(url.pathname==='/mount-ajax-get.txt'&&url.searchParams.has('coord_action')){
    const q=url.searchParams,fields=['ra_h','ra_m','ra_s','dec_d','dec_m','dec_s'],max=[23,59,59,90,59,59];
    const valid=fields.every((k,i)=>/^\d+$/.test(q.get(k)||'')&&Number(q.get(k))<=max[i])&&['+','-'].includes(q.get('dec_sign'))&&['go','sync'].includes(q.get('coord_action'))&&!(Number(q.get('dec_d'))===90&&(Number(q.get('dec_m'))||Number(q.get('dec_s'))));
    return send(valid?'ok':'invalid','text/plain');
  }
  if(url.pathname==='/auxiliary-ajax-get.txt'){if(!auxiliaryDevice.command(url.searchParams))res.statusCode=400;return send('','text/plain');}
  if(url.pathname==='/auxiliary-ajax.txt')return send(auxiliaryDevice.poll(),'text/plain');
  if(url.pathname==='/enc-ajax-get.txt') {
    const action=url.searchParams.get('enc');
    if(action==='on'||action==='off')encoderAutoSync=action==='on';
    return send('','text/plain');
  }
  if(url.pathname==='/enc-ajax.txt')return send(
    'enc_as_on|'+(encoderAutoSync?'selected':'unselected')+'\n'+
    'enc_as_off|'+(encoderAutoSync?'unselected':'selected')+'\n', 'text/plain');
  if(url.pathname.endsWith('.txt')||url.pathname==='/preview-poll')return send('','text/plain');
  if(url.pathname==='/libraryHelp.htm')return send(head+'</head><body>'+shared+'<main class="sws-document"><h1>天体库</h1>'+Object.values(templates['LibraryHelp.h']).join('')+'</main></body></html>');
  const page=url.pathname==='/'?'/index.htm':url.pathname;
  if(!pages.includes(page)){res.statusCode=404;return send('页面不存在');}
  let html=doc(page,url.searchParams.has('view')||req.method==='POST');
  // 模拟脚本先于内联按钮脚本加载，保存值仅写入浏览器本地存储
  html=html.replace('</head>','<script src="/demo.js"></script></head>');
  send(html);
});
server.listen(Number(process.env.SWS_DEMO_PORT||8765),'127.0.0.1',()=>console.log('SWS demo ready http://127.0.0.1:'+server.address().port));
