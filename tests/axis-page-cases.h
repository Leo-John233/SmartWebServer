// 逐项检查设备返回的参数没有被预览或新布局省略
int main() {
  char field[320];const std::string longName(79,'x'),longValue(23,'9');
  const int fieldLength=snprintf_P(field,sizeof(field),html_configAxisFloat,longValue.c_str(),2147483647,2147483647,(-2147483647L-1),2147483647L,longName.c_str());
  assert(fieldLength>256&&fieldLength<int(sizeof(field))&&std::string(field).find("</span></label>\n")!=std::string::npos);
  String data;
  assert(axisTile(0,data));
  for(int p=1;p<=26;p++) {
    const std::string key="name='a1p"+std::to_string(p)+"value'";
    assert(www.output.find(key)!=std::string::npos);
  }
  assert(www.output.find(L_AXPN_STEPS_PER_DEG)!=std::string::npos);
  assert(www.output.find(L_AXPN_CURRENT_RUN)!=std::string::npos);
  assert(www.output.find(L_REVERT)!=std::string::npos);
  assert(www.output.find(L_UPLOAD)!=std::string::npos);
  for(int p=1;p<=26;p++)www.args["a9p"+std::to_string(p)+"value"]="1";
  axisTileGet();
  assert(onStep.sent.size()==26);
  assert(onStep.sent.front()==":SXA9,1,1#");
  assert(onStep.sent.back()==":SXA9,26,1#");
  std::cout<<www.output;
}
