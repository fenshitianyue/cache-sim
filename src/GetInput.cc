#include "base.h"

void GetCacheSize(){
  std::cout << std::endl;
  std::cout << "Input the number of the cache size(Unit:KB)";
  std::cout << std::endl;
  std::cout << "\t" << "(for example:1,2,4,8...2^18)" << std::endl;
  std::cout << ">";
  std::cin >> i_cache_size;
  if(std::cin.fail()){
    std::cin.clear();
    std::cin.sync();
  }
  if(i_cache_size < 1 || i_cache_size >= 262144 || (i_cache_size & (~i_cache_size + 1)) != i_cache_size){
    GetCacheSize();
  }
}

void GetCacheLineSize(){
  std::cout << std::endl;
  std::cout << "Input the number of the cacheline size(Unit:Byte)";
  std::cout << std::endl;
  std::cout << "\t" << "(for example:1,2,4,8...2^18)" << std::endl;
  std::cout << ">";
  std::cin >> i_cache_line_size;
  if(std::cin.fail()){
    std::cin.clear();
    std::cin.sync();
  }
  if(i_cache_line_size < 1 || i_cache_line_size >= 262144 || (i_cache_line_size & (~i_cache_line_size + 1)) != i_cache_line_size){
    GetCacheLineSize();
  }
}

void GetMappingMethod(){
  short tmp = 0;
  std::cout << std::endl;
  std::cout << "Input the method of assoiativity between main memory and cache:" << std::endl;
  std::cout << "\t" << "directive_mapped:input 1" << std::endl;
  std::cout << "\t" << "set_associative:input 2" << std::endl;
  std::cout << "\t" << "full_associative:input 3" << std::endl;
  std::cout << ">";
  std::cin >> tmp;
	if(std::cin.fail()){
    std::cin.clear();
    std::cin.sync();
	}
  switch(tmp){
    case 1:
      t_assoc = direct_mapped;
      break;
    case 2:
      t_assoc = set_associative;
      break;
    case 3:
      t_assoc = full_associative;
      break;
    default:
      GetMappingMethod();
  }
}

void GetLineCountEachSet(){
  std::cout << std::endl;
  std::cout << "Input the how many lines in each set:" << std::endl;
  std::cout << "\t" << "(for example:1,2,4,8...2^18)" << std::endl;
  std::cout << ">";
  std::cin >> i_cache_set;
	if (std::cin.fail()){
    std::cin.clear();
    std::cin.sync();
	}
	if (i_cache_set < 1 || i_cache_set >= 262144 || (i_cache_set & (~i_cache_set + 1)) != i_cache_set){
		GetLineCountEachSet();
	}
}

void GetReplacePolicy(){
	short tmp;
  std::cout << std::endl;
  std::cout << "Input the replacement policy:" << std::endl;
  std::cout << "\t" << "FIFO:input 1" << std::endl;	
  std::cout << "\t" << "LRU :input 2" << std::endl;
  std::cout << "\t" << "LFU :input 3" << std::endl;
  std::cout << "\t" << "Random:input 4" << std::endl;
  std::cout << ">";
  std::cin >> tmp;
	if (std::cin.fail()){
    std::cin.clear();
    std::cin.sync();
	}
	switch(tmp)
	{
	case 1:
		t_replace = FIFO;
		break;
	case 2:
		t_replace = LRU;
		break;
	case 3:
		t_replace = LFU;
		break;
	case 4:
		t_replace = Random;
		break;
	default:
		GetReplacePolicy();
	}
}

void GetWritePolicy(){
	short tmp;
  std::cout << std::endl;
  std::cout << "Input write policy:" << std::endl;
  std::cout << "\t" << "Write through:input 1" << std::endl;
  std::cout << "\t" << "Write back:input 2" << std::endl;
  std::cout << ">";
  std::cin >> tmp;
	if(std::cin.fail()){
    std::cin.clear();
    std::cin.sync();
	}
	switch(tmp)
	{
	case 1:
		t_write = write_through;
		break;
	case 2:
		t_write = write_back;
		break;
	default:
		GetWritePolicy();
	}
}

void GetInput(void){
  GetCacheSize();
  GetCacheLineSize();
  GetMappingMethod();
  if(direct_mapped == t_assoc){
    t_replace = none; 
    GetWritePolicy();
  }else if(full_associative == t_assoc){
    GetReplacePolicy();
    GetWritePolicy();
  }else if(set_associative == t_assoc){
    GetLineCountEachSet();
    GetReplacePolicy();
    GetWritePolicy();
  }
}
