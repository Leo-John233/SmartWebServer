// 演示沿用辅助页的请求参数和状态键，只修改模拟通道
module.exports=preview=>{
 const features=structuredClone(preview.features),times=preview.timeValues;
 function command(params){
  const changes=[];
  for(const [key,text]of params){
   if(key==='x'||key==='nocache')continue;
   const match=/^x([1-8])v([1-4])$/.exec(key);if(!match||!/^[-]?\d+$/.test(text))return false;
   const feature=features[Number(match[1])-1],field=Number(match[2]),value=Number(text),type=feature.purpose;
   if(!type)return false;
   if(field===1){if(type===2?value<0||value>255:![0,1].includes(value))return false;}
   else if(type===3&&[2,3].includes(field)){if(value< -10||value>40)return false;}
   else if(type===4){if(value<(field===3?10:field===4?1:0)||value>255)return false;}
   else return false;
   changes.push({feature,field,value:type===3&&field>1?value/2:type===4&&[2,3].includes(field)?times[value]:value});
  }
  for(const {feature,field,value}of changes)feature.values[field-1]=value;
  return true;
 }
 function poll(){
  const lines=[],add=(key,value)=>lines.push(key+'|'+value);
  features.forEach((feature,index)=>{
   const {purpose:type,values:v,voltage,current}=feature,n=index+1;if(!type)return;
   if(type!==4){if(Number.isFinite(voltage))add('vout'+n,v[0]?voltage.toFixed(1):'0.0');if(Number.isFinite(current))add('iout'+n,Math.abs(current)>.2499?current.toFixed(1):'0.0');}
   if(type===1||type===3){add('sw'+n+'_on',v[0]?'selected':'unselected');add('sw'+n+'_off',v[0]?'unselected':'selected');}
   if(type===2)add('x'+n+'v1',Math.round(v[0]/255*100));
   if(type===3)for(const field of [2,3,4])add('x'+n+'v'+field,v[field-1].toFixed(1));
   if(type===4){add('x'+n+'v1',Math.abs(v[0])<.001?'-':Math.trunc(v[0]));add('x'+n+'v2',v[1].toFixed(v[1]<1?3:v[1]<10?2:v[1]<30?1:0));add('x'+n+'v3',v[2].toFixed(v[2]<10?2:v[2]<30?1:0));add('x'+n+'v4',Math.trunc(v[3]));}
  });
  return lines.join('\n')+'\n';
 }
 return {command,poll};
};
