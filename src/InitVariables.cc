#include "base.h"

void InitVariables(void){
  for(i = 0; i < i_num_line; ++i){
    cache_item[i].reset(); //[31]:valid,[30]:hit,[29]:dirty,[28]-[0]:data
    if(t_replace == LRU){
      LRU_priority[i] = 0; //For LRU policy's priority
    }
    i_cache_size = 64; //cache size
    i_cache_line_size = 32; // cacheline size
#ifdef SetAssociative_Random_WriteBack 
    i_cache_set = 4; //cache set
#else 
    i_num_line = 0; 
    i_num_set = 0; 
    i_cache_set = 0; //some changes have been made to the old project
#endif //SetAssociative_Random_WriteBack
#ifdef DirectMapped_None_WriteBack 
    t_assoc = direct_mapped; //associativity method -> default direct_mapped
    t_replace = none; //replacement plicy -> default none
    t_write = write_back; // write policy -> default write_back
#endif //DirectMapped_None_WriteBack
#ifdef FullAssociative_Random_WriteBack 
    t_assoc = full_associative; //associativity method -> default full -> default full_associative
    t_replace = Random; //replacement policy -> default Random
    t_write = write_back; //write policy -> default write_back
#endif //FullAssociative_Random_WriteBack
#ifdef SetAssociative_Random_WriteBack 
    t_assoc = set_associative; // associativity method -> default set_associative
    t_replace = Random; //replcement policy -> default Random
    t_write = write_back; //write policy -> default write_back
#endif //SetAssociative_Random_WriteBack
    bit_block = 0; //how many bits of the block
    bit_line = 0; //how many bits of the line
    bit_tag = 0; //how many bits of the tag
    bit_set = 0; //how many bits of the set
    i_num_access = 0; //number of cache access
    i_num_load = 0; // number of cache load
    i_num_store = 0; //number of space store
    i_num_space = 0; //number of space line 
    i_num_hit = 0; //number of cache hit 
    i_num_load_hit = 0; //number of load hit
    i_num_store_hit = 0; //number of store hit 
    f_ave_rate = 0.0; //average cache hit rate
    f_load_rate = 0.0; //cache hit rate for loads
    f_store_rate = 0.0; //cache hit rate for stores
    current_line = 0; //the line num which is processing
    current_set = 0; //the set num which is processing
    i = 0; j = 0; //For loop
  }
}
