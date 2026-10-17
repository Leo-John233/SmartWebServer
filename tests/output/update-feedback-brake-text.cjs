const fs=require('node:fs');
const keys=['L_IO_BRAKE_HELP','L_IO_BRAKE_WAITING','L_IO_BRAKE_STALE','L_IO_BRAKE_STOPPING','L_IO_BRAKE_STOP_FAILED'];
const translations={
cn:['故障先停止，确认静止后抱闸；无反馈松闸，关闭后抱闸','等待有效反馈，保持松闸','反馈超时，保持松闸','已发停止，等待静止反馈','停止指令未发出，保持松闸'],
en:['On a fault, send STOP and confirm standstill before braking; no feedback releases, off locks','Waiting for valid feedback, released','Feedback timeout, released','STOP sent, waiting for standstill','STOP not sent, released'],
ca:['Amb error, envia STOP i confirma repòs abans de frenar; sense resposta allibera, desactivada bloqueja','Esperant resposta vàlida, alliberat','Resposta excedida, alliberat','STOP enviat, esperant repòs','STOP no enviat, alliberat'],
de:['Bei Fehler STOP senden und Stillstand vor dem Bremsen bestätigen; ohne Rückmeldung gelöst, ausgeschaltet gesperrt','Warte auf gültige Rückmeldung, gelöst','Rückmeldung überfällig, gelöst','STOP gesendet, warte auf Stillstand','STOP nicht gesendet, gelöst'],
es:['Con fallo envía STOP y confirma reposo antes de frenar; sin respuesta libera, apagado bloquea','Esperando respuesta válida, liberado','Respuesta agotada, liberado','STOP enviado, esperando reposo','STOP no enviado, liberado'],
fr:['Sur défaut, envoie STOP puis confirme l’arrêt avant le freinage ; sans retour libéré, désactivée bloqué','Attente d’un retour valide, libéré','Retour trop tardif, libéré','STOP envoyé, attente de l’arrêt','STOP non envoyé, libéré'],
it:['Con un guasto invia STOP e conferma l’arresto prima di frenare; senza risposta rilascia, disattivato blocca','Attesa di risposta valida, rilasciato','Risposta scaduta, rilasciato','STOP inviato, attesa arresto','STOP non inviato, rilasciato'],
jp:['異常時は停止指令と静止確認後にロック、応答なしは解除、オフ時はロック','有効な応答待ち、解除を維持','応答切れ、解除を維持','停止指令送信済み、静止応答待ち','停止指令未送信、解除を維持'],
ro:['La eroare trimite STOP și confirmă repausul înainte de frânare; fără răspuns eliberează, oprită blochează','Așteptare răspuns valid, eliberată','Răspuns expirat, eliberată','STOP trimis, așteptare repaus','STOP netrimis, eliberată']};
for(const [language,values]of Object.entries(translations)){
 const file='legacy/SmartWebServer/src/locales/Strings_'+language+'.h';let source=fs.readFileSync(file,'utf8');
 keys.forEach((key,i)=>{const pattern=new RegExp('^#define '+key+' .*$', 'm'),line='#define '+key+' '+JSON.stringify(values[i]);source=pattern.test(source)?source.replace(pattern,line):source.trimEnd()+'\n'+line+'\n';});fs.writeFileSync(file,source);
}
const file='legacy/tests/board-io-ui.cjs';let source=fs.readFileSync(file,'utf8').replaceAll('等待驱动，已抱闸','等待有效反馈，保持松闸');fs.writeFileSync(file,source);
