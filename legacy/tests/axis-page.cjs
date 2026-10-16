const testOutput = require('./test-output.cjs');
// 使用实际轴页面代码和语言资源，验证完整参数与第九轴提交
const fs=require('node:fs'),path=require('node:path'),cp=require('node:child_process');
const root=path.join(__dirname,'../SmartWebServer'),read=p=>fs.readFileSync(path.join(root,p),'utf8');
const clean=s=>s.replace(/^#include.*$/gm,'').replace(/^#pragma once.*$/gm,'');
const temp=fs.mkdtempSync(path.join(testOutput,'sws-axis-'));
const program=fs.readFileSync(path.join(__dirname,'axis-page-stubs.h'),'utf8')+'\n'+require('./locale-source.cjs')()+'\n'+require('./page-support.cjs')()+'\n'+clean(read('src/pages/htmlHeaders.h'))+'\n'+clean(read('src/pages/index/AxisTile.h'))+'\n'+clean(read('src/pages/index/AxisTile.cpp'))+'\n'+fs.readFileSync(path.join(__dirname,'axis-page-cases.h'),'utf8');
fs.writeFileSync(path.join(temp,'test.cpp'),program);
const exe=path.join(temp,'axis.exe');const build=cp.spawnSync(process.env.CXX||'g++',['-std=c++11',path.join(temp,'test.cpp'),'-o',exe],{encoding:'utf8'});if(build.status!==0){process.stderr.write(build.stderr||'');process.exit(1);}
const result=cp.spawnSync(exe,[],{encoding:'utf8'});if(result.status!==0){process.stderr.write(result.stderr||'');process.exit(1);}if(process.argv[2])fs.writeFileSync(process.argv[2],result.stdout);console.log('PASS: actual axis renderer, all 26 localized parameters, controls, revert, ninth axis submission');
