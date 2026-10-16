// 演示交互只更新模拟数据，保留真实页面的按钮和参数名称
const demoKey='sws-demo'+location.pathname;
let demoSaved=JSON.parse(localStorage.getItem(demoKey)||'{}');
function demoNotice(message){let e=document.getElementById('demo-toast');if(!e){e=document.createElement('div');e.id='demo-toast';e.style.cssText='position:fixed;bottom:20px;right:20px;z-index:9999;background:#59303b;color:#f1dbe2;padding:12px 20px;border-radius:10px;max-width:85vw';e.setAttribute('role','status');document.body.appendChild(e);}e.textContent=message;clearTimeout(window.demoToastTimer);window.demoToastTimer=setTimeout(()=>e.remove(),2300);}
function demoText(id,value){const e=document.getElementById(id);if(e)e.textContent=value;}
function s(key,value){
  if(key==='dr'&&(value==='T1'||value==='T0')){document.getElementById('trk_on').style.background=value==='T1'?'#994b57':'#823b46';document.getElementById('trk_off').style.background=value==='T0'?'#994b57':'#823b46';}
  if(key==='lib_index'){demoText('lib_message',value==='0'?'未选择星表':'星表 '+value);['cat_prev','cat_next','cat_select','cat_download'].forEach(id=>{const e=document.getElementById(id);if(e)e.disabled=value==='0';});}
  if(key==='cat_upload'){demoSaved.catalog=value.replace(/_/g,' ');demoText('lib_message','模拟星表已上传');document.getElementById('cat_upload').disabled=false;document.getElementById('cat_download').disabled=false;}
  if(key==='lib'&&value==='clear'){demoSaved.catalog='';const area=document.getElementById('cat_data');if(area)area.value='';demoText('lib_message','模拟星表已清除');}
  demoSaved[key]=value;localStorage.setItem(demoKey,JSON.stringify(demoSaved));
  const labels={T1:'已开启追踪',T0:'已关闭追踪',Ts:'已选择恒星速',Tl:'已选择月球速',Th:'已选择太阳速',Tk:'已选择大气折射修正速',go:'正在前往目标',stop:'已停止',cat_prev:'M31 仙女座星系',cat_next:'M42 猎户座星云',cat_select:'已设为目标',cat_download:'星表已载入',clear:'星表已清除'};
  if(key==='lib'){demoText('cat_message',labels[value]||'已更新');const area=document.getElementById('cat_data');if(area&&value==='cat_download')area.value=demoSaved.catalog||'$Messier\nM31,SG,00:42:44,+41*16:09\nM42,DN,05:35:17,-05*23:28';}
  if(key==='lib'&&value==='cat_download'){document.getElementById('cat_download').disabled=false;document.getElementById('cat_upload').disabled=false;demoText('lib_message','模拟星表已下载');}
  if(key==='fs'||key==='rs')demoNotice('模拟位置已设为 '+value);else demoNotice(labels[value]||'模拟设置已更新');
  if(value==='go')setTimeout(()=>{demoText('gto_i1','05h35m17s');demoText('gto_i2','-05°23′28″');demoNotice('已到达模拟目标');},1500);
}
function g(v){s('dr',v);}function gf(v){g(v);}function sf(k,v){s(k,v);}function sz(k,v){s(k,v);}function t(k,v){s(k,v);}function busy(){demoNotice('正在处理');}function autoFastRun(){}
document.addEventListener('DOMContentLoaded',()=>{
  // 用固定标记区分演示与实机，导航和原有功能描述保持不变
  if(!location.search.includes('view')){const e=document.createElement('div');e.textContent='模拟演示';e.style.cssText='position:fixed;right:22px;top:8px;z-index:50;font-size:11px;color:#bc8f9e;pointer-events:none';document.body.appendChild(e);return;}
  for(const [id,value] of Object.entries({last_err:'无',work_load:'15%',signal:'-55 dBm',gto_i1:'23h57m00s',gto_i2:'+90°00′00″',gto_t1:'05h35m17s',gto_t2:'-05°23′28″',gto_az1:'120°',gto_az2:'45°'}))demoText(id,value);
  document.querySelectorAll('button[disabled]').forEach(e=>e.disabled=false);
  document.querySelectorAll('input,select,textarea').forEach(e=>{if(e.name in demoSaved&&e.type!=='password'){if(e.type==='checkbox')e.checked=demoSaved[e.name]==='on';else e.value=demoSaved[e.name];}});
  document.querySelectorAll('form').forEach(form=>{if(form.id==='nw-sta'||form.id==='nw-ap'||form.id==='sws-coordinate-form')return;form.addEventListener('submit',e=>{if(location.pathname==='/net.htm'&&form.closest('.nw-card'))return;e.preventDefault();const values=Object.fromEntries(new FormData(form));if(e.submitter?.name)values[e.submitter.name]=e.submitter.value;for(const field of form.querySelectorAll('input[type=password]'))delete values[field.name];demoSaved={...demoSaved,...values};localStorage.setItem(demoKey,JSON.stringify(demoSaved));demoNotice('模拟设置已保存');});});
  // 成功提交坐标后更新模拟位置，真实设备仍使用自身状态轮询
  const nativeFetch=window.fetch;window.fetch=async function(input,init){const response=await nativeFetch.call(this,input,init);const url=new URL(typeof input==='string'?input:input.url,location.href);if(url.pathname==='/mount-ajax-get.txt'&&url.searchParams.has('coord_action')&&(await response.clone().text()).trim()==='ok'){const q=url.searchParams,pad=k=>q.get(k).padStart(2,'0');const ra=pad('ra_h')+'h'+pad('ra_m')+'m'+pad('ra_s')+'s',dec=q.get('dec_sign')+pad('dec_d')+'°'+pad('dec_m')+'′'+pad('dec_s')+'″';demoText('gto_t1',ra);demoText('gto_t2',dec);demoText('gto_i1',ra);demoText('gto_i2',dec);}return response;};
  // 为按住移动的真实请求显示反馈，避免演示按钮无响应
  const originalOpen=XMLHttpRequest.prototype.open;XMLHttpRequest.prototype.open=function(method,url,...rest){const u=new URL(url,location.href);if(u.pathname.endsWith('-ajax-get.txt')){const value=[...u.searchParams].find(([k])=>k!=='x')?.[1];demoNotice(['q','stop','n0','s0','e0','w0'].includes(value)?'模拟移动已停止':'正在模拟移动');}return originalOpen.call(this,method,url,...rest);};
});
