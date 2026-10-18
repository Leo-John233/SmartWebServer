#pragma once
#include "Text.h"

// 页头由固件直接输出，脚本只等待已发起的运动停止，不重建页面布局
const char html_page_navigation[] PROGMEM = R"OPTI(
<script>(()=>{let leaving=false;addEventListener("pageshow",()=>{leaving=false});document.addEventListener("click",async event=>{const a=event.target instanceof Element&&event.target.closest(".sws-nav a");if(!a)return;if(event.button!==0||event.ctrlKey||event.metaKey||event.shiftKey||event.altKey)return;if(leaving){event.preventDefault();return}const stops=(window.swsMotionStops||[]).map(stop=>stop()).filter(Boolean);if(!stops.length){if(window.swsPausePolling)swsPausePolling();return}event.preventDefault();leaving=true;if(window.swsPausePolling)swsPausePolling();let deadline;try{await Promise.race([Promise.all(stops),new Promise((_,reject)=>{deadline=setTimeout(()=>reject(Error("stop timeout")),5e3)})]);location.assign(a.href)}catch(_){leaving=false;if(window.swsResumePolling)swsResumePolling();const hint=document.createElement("p");hint.setAttribute("role","status");hint.textContent=)OPTI" SWS_JS_TEXT(L_UI_SLOW_LOAD) R"OPTI(;const topbar=document.querySelector(".sws-topbar");if(topbar)topbar.appendChild(hint)}finally{clearTimeout(deadline)}})})();</script>
)OPTI";
