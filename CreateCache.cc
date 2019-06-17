#include "base.h"

void CreateCache(){
  temp = i_num_line; //临时变量获取CPU cache中行数
#ifndef NDEBUG 
  //输出CPU cache前100行命中情况
  for(i = 0; i < 100; ++i){
    std::cout << cache_item[i] << std::endl;
  }
#endif
  for(i = 0; i < temp; ++i){
    cache_item[i][31] = true;
  }
#ifndef NDEBUG 
  for(i = 0; i < 100; ++i){
    std::cout << cache_item[i] << std::endl;
  }
#endif
}
