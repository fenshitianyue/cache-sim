#include "base.h"

unsigned int long i_cache_size = 0; //cache size
unsigned int long i_cache_line_size = 0; //cacheline size
unsigned int long i_cache_set = 0; //cache set

unsigned int long i_num_line = 0; //how many lines of the cache
unsigned int long i_num_set = 0; //how many sets of the cache

ASSOC t_assoc = direct_mapped; //associativity method -> default direct_mapped
REPLACE t_replace = none; //replacement policy -> default Random
WRITE t_write = write_back; //write policy -> default write_back

short unsigned int bit_block = 0; //how many bits of the block
short unsigned int bit_line = 0; //how many bits of the line
short unsigned int bit_tag = 0; //how many bits of the tag
short unsigned int bit_set = 0; //how many bits of the set


unsigned long int i_num_access = 0; //number of cache access
unsigned long int i_num_load = 0; //number of cache load
unsigned long int i_num_store = 0; //number of cache store
unsigned long int i_num_space = 0; //number of space line

unsigned long int i_num_hit = 0; //number of cache hit
unsigned long int i_num_load_hit = 0; //number of load hit
unsigned long int i_num_store_hit = 0; //number of store hit

float f_ave_rate = 0.0; //average cache hit rate
float f_load_rate = 0.0; //cache hit rate for loads
float f_store_rate = 0.0; //cache hit rate for stores

std::bitset<32> cache_item[MAX_CACHE_LINE]; // [31]:valid,[30]:hit,[29]:dirty,[28]-[0]:data
unsigned long int LRU_priority[MAX_CACHE_LINE]; //For LRU policy's priority
unsigned long int current_line = 0; // The line num which is processing
unsigned long int current_set = 0; // The set num which is processing
unsigned long int i=0,j=0; //For loop
unsigned long int temp = 0; //A temp varibale

///////////////////////////////////////////////////////
// defination of almost all function
///////////////////////////////////////////////////////

//using namespace std; 暂时不确定是否要使用全局命名空间

bool GetHitNum(char* address){
  bool is_store = false;
  bool is_load = false;
  bool is_space = false;
  bool hit = false;

  switch(address[0]){
    case 's':
      is_store = true;
      break;
    case 'l':
      is_load = true;
      break;
    case '\0': //in case of space line
      is_space = true;
      break;
    default:
      std::cout << "The address[0] is: " << address[0] << std::endl;
      return false;
  }
  temp = strtoul(address+2, NULL, 16);
  std::bitset<32> flags(temp);
#ifndef NDEBUG 
  std::cout << flags << std::endl;
#endif
  hit = IsHit(flags);
  if(hit && is_load){
    ++i_num_access; //访问次数+1
    ++i_num_load; //读取次数+1
    ++i_num_load_hit; //读取命中次数+1
    ++i_num_hit; //命中次数+1
#ifndef NDEBUG 
    std::cout << "Loading..." << std::endl;
    std::cout << "Hit" << std::endl;
    std::cout << "Read from Cache..." << std::endl;
#endif
    if(LRU == t_replace){
      LruHitProcess();
    }
  } else if (hit && is_store){
    ++i_num_access; //访问次数+1
    ++i_num_store; //存储次数+1
    ++i_num_store_hit; //存储命中次数+1
    ++i_num_hit; //命中次数+1
#ifndef NDEBUG 
    std::cout << "Storing..." << std::endl;
    std::cout << "Hit" << std::endl;
    std::cout << "Write to Cache..." << std::endl;
#endif
    
    cache_item[current_line][29] = true; //设置dirty位为true
    if(LRU == t_replace){
      LruHitProcess();
    }
    
  } else if (!hit && is_load){
    ++i_num_access; //访问次数+1
    ++i_num_load; //读取次数+1
#ifndef NDEBUG 
    std::cout << "Loading..." << std::endl;
    std::cout < "Not Hit" << std::endl;
#endif
    GetRead(flags); //read data from memory
#ifndef NDEBUG
    std::cout << "Read from cache..." << std::endl; 
#endif
    if(LRU == t_replace){
      LruUnhitSpace();
    }
  } else if (!hit && is_store){
    ++i_num_access; //访问次数+1
    ++i_num_store; //存储次数+1
#ifndef NDEBUG
    std::cout << "Storing..." << std::endl;
    std::cout << "Not Hit" << std::endl;
#ifndef NDEBUG
    GetRead(flags);
    std::cout << "Write to Cache..." << std::endl;
#endif 
    cache_item[current_line][29] = true; //设置dirty为true

    if(t_replace == LRU){
        LruUnhitSpace();
    }
#endif // NDEBUG
  } else if (is_space){ //if it's an empty line
    ++i_num_space;
  } else {
    std::cerr << "Something Error" << std::endl;
    return false;
  }
  if(i_num_space != 0){
#ifndef NDEBUG 
    std::cout << "There have" << i_num_space << " space lines"
              << std::endl;
#endif
  }

  return true;
}

bool IsHit(std::bitset<32> flags){
  bool ret = false;
  ///////////////////////////////////////////////////////
  //   对于直接相连映射固定行和块的对应关系，例如内存中第0
  //块必须放在cache第0行。
  ///////////////////////////////////////////////////////
  if(direct_mapped == t_assoc){ 
    //a temp variable
    std::bitset<32> flags_line; 
    //判断在 cache 哪一行
    for(j = 0, i = bit_block; i < (bit_block + bit_line); ++j, ++i){
      flags_line[j] = flags[i];
    }
    current_line = flags_line.to_ulong();
    assert(cache_item[current_line][31]);
    if(cache_item[current_line][30]){ //判断hit位是否为真
      ret = true;
      //判断标记是否相同。i -> address, j -> cache
      for(i = 31, j = 28; i > (31 - bit_tag); --i, --j){
        if(flags[i] != cache_item[current_line][j]){
          ret = false;
          break;
        }
      }//end for(i=31...)
    }//end if(cache_item...)
  } else if (full_associative == t_assoc){
    ///////////////////////////////////////////////////////
    //   对于全相联映射，因为任意一行都有可能存在所需数据
    //所以需要比较每一行的索引值才能确定cache中是否存在所
    //需要数据。
    ///////////////////////////////////////////////////////
    for(temp = 0; temp < i_num_line; ++i){
      if(cache_item[temp][30]){
        ret = true;
        //判断标记是否相同。i -> address, j -> cache
        for(i = 31, j = 28; i > (31 - bit_tag); --i, --j){
          if(flags[i] != cache_item[current_line][j]){
            ret = false;
            break;
          }   
        }//end for(i=31...)
      }//end if(cache_item...)
      if(ret){
        current_line = temp;
        break;
      }
    }//end for(temp = 0...)
  } else if (set_associative == t_assoc){
    ///////////////////////////////////////////////////////
    //   对于组相连映射，先将cache的行进行分组，然后内存块按
    //照组号求模来决定该内存块放置到cache的哪一组。但是具体
    //放到组内的哪一行则是随机的，具体由替换算法决定。
    ///////////////////////////////////////////////////////
    std::bitset<32> flags_set;
    //判断在cache哪一组
    for(j = 0, i = bit_block; i < (bit_block+bit_set); ++j, ++i){
      flags_set[j] = flags[i];
    }
    current_set = flags_set.to_ulong();
    for(temp = (current_set * i_cache_set); temp < ((current_set + 1) * i_cache_set); ++temp){
      if(cache_item[temp][30]){
        ret = true;
        //判断标记是否相同。i -> address, j -> cache
        for(i = 31, j = 28; i > (31 - bit_tag); --i, --j){
          if(flags[i] != cache_item[temp][j]){
            ret = false;
            break;
          }
        }//end for(i = 31...)
      }//end if(cache_item...)
      if(ret){
        current_line = temp;
        break;
      }
    }//end for(temp...)
  }
  return ret;
}

void GetReplace(std::bitset<32> flags){
  if(full_associative == t_assoc){
    if(Random == t_replace){
      //a random and current line line in(0, i_num_line - 1)
      current_line = rand() / (RAND_MAX/(i_num_line + 1));
    } else if (LRU == t_replace){
      LruUnhitUnspace();
    }
  } else if (set_associative == t_assoc){
    //从本组中任选一行进行替换
    if(Random == t_replace){
      //a random line in(0, i_cache_set - 1)
      temp = rand() / (RAND_MAX / i_cache_set + 1);
      //a random line in current_set
      current_line = current_set * i_cache_set + temp;
    } else if (LRU == t_replace){
      LruUnhitUnspace();
    }
  } else if (direct_mapped == t_assoc){
    //TODO
  }
  //写入策略采用写回法
  if(cache_item[current_line][29]){ //如果dirty位为1
    GetWrite();
  }
#ifndef NDEBUG 
  std::cout << "Read from Main Memory to Cache: " << std::endl;
#endif
  //设置标记
  for(i = 31, j = 28; i > (31 - bit_tag); --i, --j){
    cache_item[current_line][j] = flags[i];
    assert(j > 0);
  }
  //更新hit命中标记为true
  cache_item[current_line][30] = true;
}

void GetRead(std::bitset<32> flags){
  if(direct_mapped == t_assoc){
    if(!cache_item[current_line][30]){ //miss hit
#ifndef NDEBUG 
      std::cout << "Read from Main Memory to Cache!" << std::endl;
#endif
      for(i = 31, j = 28; i > (31 - bit_tag); --i, --j){
        cache_item[current_line][j] = flags[i];
        assert(j > 0);
      }
      //更新hit位为命中
      cache_item[current_line][30] = true;
    } else {
      GetReplace(flags);
    }
  } else if (full_associative == t_assoc){
    bool space = false;
    for(temp = 0; temp < i_num_line; ++temp){
      if(!cache_item[temp][30]){
        space = true;
        break;
      }
    } 
    if(space){
      current_line = temp; 
#ifndef NDEBUG 
      std::cout << "Read from Main Memory to Cache!" << std::endl;
#endif
      for(i = 31, j = 28; i > (31 - bit_tag); --i, --j){
        cache_item[current_line][j] = flags[i];
        assert(j > 0);
      } 
      //更新hit位为命中
      cache_item[current_line][30] = true;
      if(LRU == t_replace)
        LruUnhitSpace();
    } else {
      GetReplace(flags);
    }
  } else if (set_associative == t_assoc){
    //TODO:暂时先不做什么
  }
}

//模拟写入内存
void GetWrite(){
#ifndef NDEBUG 
  std::cout << "Write to the Main Memory" << std::endl;
#endif
  cache_item[current_line][29] = false; //设置dirty位为false
  cache_item[current_line][30] = false; //设置hit位为false
}

//获取cache访问率
void GetHitRate(){
  assert(i_num_access != 0);
  assert(i_num_load != 0);
  assert(i_num_store != 0);
  //average cache hit rate
  f_ave_rate = ((double)i_num_hit) / i_num_access;
  //cache hit rate for loads
  f_load_rate = ((double)i_num_load_hit) / i_num_load;
  //cache hit rate for store 
  f_store_rate = ((double)i_num_store_hit) / i_num_store; 
}


