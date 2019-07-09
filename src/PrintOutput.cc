#include "base.h"

void PrintOutput(void){
  std::cout << std::endl;
  std::cout << "Cache Size:" << i_cache_size << "KB" << std::endl;
  std::cout << "Cacheline Size:" << i_cache_line_size << "B" << std::endl;

  switch(t_assoc)
  {
    case 1:
      std::cout << "Way of Associativity:direct_mapped" << std::endl;
      break;

    case 2:
      std::cout << "Way of Associativity:set_associative" << std::endl;
      break;

    case 3:
      std::cout << "Way of Associativity:full_associative" << std::endl;
      break;

    default:
      std::cerr << "ERROR ASSOCIATIVITY";
      break;
  }

  switch(t_replace)
  {
    case 0:
      std::cout << "Way of Replacement:NONE" << std::endl;
      break;

    case 1:
      std::cout << "Way of Replacement:FIFO" << std::endl;
      break;

    case 2:
      std::cout << "Way of Replacement:LRU" << std::endl;
      break;

    case 3:
      std::cout << "Way of Replacement:LFU" << std::endl;
      break;

    case 4:
      std::cout << "Way of Replacement:Random" << std::endl;
      break;

    default:
      std::cerr << "ERROR REPLACEMENT";
      break;
  }

  switch(t_write)
  {
    case 1:
      std::cout << "Way of Write:write_through" << std::endl;
      break;

    case 2:
      std::cout << "Way of Write:write_back" << std::endl;
      break;

    default:
      std::cerr << "ERROR WRITE";
      break;
  }

  std::cout << std::endl;
  std::cout << "Number of cache access:" << i_num_access << std::endl;
  std::cout << "Number of cache load:" << i_num_load << std::endl;
  std::cout << "Number of cache store:" << i_num_store << std::endl;
  std::cout << std::endl;
  std::cout << "Average cache hit rate:" << f_ave_rate*100 << "%" << std::endl;
  std::cout << "Cache hit rate for loads:" << f_load_rate*100 << "%" << std::endl;
  std::cout << "Cache hit rate for stores:" << f_store_rate*100 << "%" << std::endl;
  std::cout << std::endl;
}

