const fs=require('node:fs');
const translations={
cn:['赤经轴停止与抱闸','驱动异常时先停止再抱闸；关闭后保持抱闸'],
en:['RA stop and brake','Driver faults send STOP before locking the brake; stays locked when off'],
ca:['Aturada i fre d’AR','Els errors del motor envien STOP abans de bloquejar el fre; desactivada manté bloquejat'],
de:['RA-Stopp und Bremse','Bei Treiberfehler zuerst STOP senden, dann Bremse sperren; ausgeschaltet bleibt sie gesperrt'],
es:['Parada y freno de AR','Con un fallo envía STOP antes de bloquear el freno; apagado mantiene bloqueado'],
fr:['Arrêt et frein AD','Sur défaut du pilote, envoie STOP puis bloque le frein ; désactivée, reste bloqué'],
it:['Arresto e freno AR','Con un guasto invia STOP prima di bloccare il freno; disattivato resta bloccato'],
jp:['赤経軸停止とブレーキ','ドライバー異常時は停止指令後にロック、オフ時もロックを維持'],
ro:['Oprire și frână AD','La eroare trimite STOP înainte de blocarea frânei; oprită menține blocat']};
for(const [language,values]of Object.entries(translations)){
 const file='legacy/SmartWebServer/src/locales/Strings_'+language+'.h';let source=fs.readFileSync(file,'utf8');
 ['L_IO_BRAKE_TITLE','L_IO_BRAKE_HELP'].forEach((key,i)=>source=source.replace(new RegExp('^#define '+key+' .*$', 'm'),'#define '+key+' '+JSON.stringify(values[i])));
 fs.writeFileSync(file,source);
}
