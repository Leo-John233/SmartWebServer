const fs=require('node:fs');
const translations={
 cn:['驱动正常后松闸，异常或超时抱闸；关闭后保持抱闸','保护关闭，已抱闸','关闭保护后将保持抱闸，是否继续？'],
 en:['Releases on healthy feedback, locks on faults or timeout; stays locked when off','Protection off, locked','Turning protection off keeps the brake locked; continue?'],
 ca:['Allibera amb resposta correcta, bloqueja amb error o temps excedit; desactivada manté bloquejat','Protecció desactivada, bloquejat','Desactivar la protecció manté el fre bloquejat; voleu continuar?'],
 de:['Gibt bei fehlerfreier Rückmeldung frei, sperrt bei Fehler oder Zeitüberschreitung; ausgeschaltet bleibt die Bremse gesperrt','Schutz aus, gesperrt','Ausschalten hält die Bremse gesperrt; fortfahren?'],
 es:['Libera con respuesta normal, bloquea con fallos o demora; apagado mantiene bloqueado','Protección apagada, bloqueado','Desactivar mantiene el freno bloqueado; ¿continuar?'],
 fr:['Libère après un retour normal, bloque sur défaut ou délai dépassé ; désactivée, reste bloqué','Protection désactivée, bloqué','Désactiver maintient le frein bloqué ; continuer ?'],
 it:['Rilascia con risposta normale, blocca per guasti o ritardi; disattivato resta bloccato','Protezione disattivata, bloccato','Disattivare mantiene il freno bloccato; continuare?'],
 jp:['正常応答で解除、異常や応答切れでロック、オフ時もロックを維持','保護オフ、ロック中','オフにするとブレーキをロックしたままにしますが、続行しますか？'],
 ro:['Eliberează la răspuns normal, blochează la eroare sau expirare; oprită menține blocat','Protecție oprită, blocată','Oprirea protecției menține frâna blocată; continuați?']
};
for(const [language,values] of Object.entries(translations)){
 const file='legacy/SmartWebServer/src/locales/Strings_'+language+'.h';let source=fs.readFileSync(file,'utf8');
 ['L_IO_BRAKE_HELP','L_IO_BRAKE_DISABLED','L_IO_RELEASE_CONFIRM'].forEach((key,i)=>source=source.replace(new RegExp('^#define '+key+' .*$', 'm'),'#define '+(i===2?'L_IO_BRAKE_OFF_CONFIRM':key)+' '+JSON.stringify(values[i])));
 fs.writeFileSync(file,source);
}
for(const file of ['legacy/demo/server.cjs','legacy/tests/board-io-ui.cjs']){
 let source=fs.readFileSync(file,'utf8').replaceAll('保护关闭，已松闸','保护关闭，已抱闸').replaceAll('取消手动释放','取消关闭保护').replaceAll('release confirmation','disable confirmation');fs.writeFileSync(file,source);
}
