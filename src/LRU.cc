#include "base.h"

//如果替换策略是LRU，并且命中
void LruHitProcess(){
  if(full_associative == t_assoc){
    for(i = 0; i < i_num_line; ++i){
      //如果该行比正在访问的行的行计数器小，并且当前行的hit标记为true
      //PS：这里有一个额外添加的策略
      if(LRU_priority[i] < LRU_priority[current_line] && cache_item[current_line][30]){ 
        ++LRU_priority[i];
      }
    }
  } else if (set_associative == t_assoc){
    for(i = (current_set * i_cache_set); i < ((current_set + 1) * i_cache_set); ++i){
      //同上
      if(LRU_priority[i] < LRU_priority[current_line] && cache_item[current_line][30]){
        ++LRU_priority[i];
      }
    }
  }
  LRU_priority[current_line] = 0;
}

//如果替换策略是LRU，并且没有命中，并且所访问的内容在cache中
void LruUnhitSpace(){
  if(full_associative == t_assoc){
    for(i = 0; i < i_num_line; ++i){
      if(cache_item[current_line][30])
        ++LRU_priority[i]; //如果该行中hit为true
    }
  } else if (set_associative == t_assoc){
    for(i = (current_set * i_cache_set); i < ((current_set+1) * i_cache_set); ++i){
      if(cache_item[current_line][30])
        ++LRU_priority[i]; //如果该行中hit为true
    }
  }
  LRU_priority[current_line] = 0;
}

//如果替换策略是LRU，并且没有命中，并且所访问的内容不在cache中
void LruUnhitUnspace(){
  if(full_associative == t_assoc){
    //找出LRU算法设定最久不曾访问的块
    temp = LRU_priority[0];
    for(i = 0; i < i_num_line; ++i){
      if(LRU_priority[i] >= temp){
        temp = LRU_priority[i];
        j = i;
      }
    }
    current_line = j;
  }

  if(set_associative == t_assoc){
    temp = LRU_priority[current_set * i_cache_set];
    for(i = (current_set * i_cache_set); i < ((current_set+1) * i_cache_set); ++i){
      if(LRU_priority[i] >= temp){
        temp = LRU_priority[i];
        j = i;
      }
    }
    current_line = j;
  }
}

