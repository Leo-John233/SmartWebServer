// 检查演示中的角分解析、停止与速度读数，避免预览和真实页面含义不同
const fs=require('node:fs'),vm=require('node:vm'),assert=require('node:assert/strict');
const elements=new Map(),timers=new Map();let timerId=0;
const get=id=>{if(!elements.has(id))elements.set(id,{textContent:'',style:{},dataset:{}});return elements.get(id);};
const context={location:{pathname:'/rotator.htm'},localStorage:{getItem:()=>null},document:{getElementById:get,addEventListener:()=>{}},addEventListener:()=>{},clearInterval:id=>timers.delete(id),setInterval:cb=>{timers.set(++timerId,cb);return timerId;},URL,window:{}};
vm.createContext(context);vm.runInContext(fs.readFileSync(require('node:path').join(__dirname,'../demo/simulation.js'),'utf8'),context);
const command=(page,key,value)=>context.demoAccessory(new URL('http://demo/'+page+'-ajax-get.txt?'+key+'='+encodeURIComponent(value)));
const tick=()=>[...timers.values()].forEach(cb=>cb());
get('rotatorpos').textContent='45°30′';command('rotator','rot','cw');tick();assert.equal(get('rotatorpos').textContent,'45.6°');
command('rotator','rot','stop');tick();assert.equal(get('rotatorpos').textContent,'45.6°');assert.equal(timers.size,0);
get('rotatorpos').textContent='-00°30′';command('rotator','rot','cw');tick();assert.equal(get('rotatorpos').textContent,'-0.4°');command('rotator','rot','stop');
get('focuserpos').textContent='12500 µm';command('focuser','foc','in');tick();assert.equal(get('focuserpos').textContent,'12490 µm');command('focuser','foc','stop');
command('focuser','foc','n');assert.equal(get('foc_rate').textContent,'0.30mm/s');command('focuser','foc','vf');assert.equal(get('foc_rate').textContent,'0.60mm/s');
command('rotator','rot','n');assert.equal(get('rot_rate').textContent,'3.0°/s');command('rotator','rot','vs');assert.equal(get('rot_rate').textContent,'1.5°/s');
console.log('PASS: demo DMS continuity, signed zero, release stop and positioning speed units');
