///////////////////////////////////////////////////////
// base.h instruction
// 1.几乎包含所有全局变量的声明
// 2.几乎包含所有结构体的定义
// 3.描述更复杂的选项
// 4.几乎包含所有函数的声明
///////////////////////////////////////////////////////

#define NDEBUG // for ndebug pattern
#define QUICK // 快速测试程序的选项

//#define OUTPUT //用于将信息写入日志文件test.log

#define DirectMapped_None_WriteBack //直接映射无回写
#define FullAssociative_Random_WriteBack //全相联随机回写
#define SetAssociative_Random_WriteBack //组相联随机回写

//cpp
#include <iostream>
#include <fstream>
#include <bitset>

//c 
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <ctime>

#ifdef QUICK 
#define MAX_CACHE_LINE 65536 //(2^16) -> cache最大行数
#endif 
#ifndef QUICK 
#define MAX_CACHE_LINE 268435436 //the max num of gcc array support (2^28)
#endif

#ifndef STRUCT_TYPE 
#define STRUCT_TYPE

//内存地址与cache地址的关联方式：直接映射，组相联，全相联
enum associativity_way { direct_mapped = 1, set_associative, full_associative };
//替换策略:直接替换，FIFO，LRU，LFU，Random
enum replacement_way { none, FIFO = 1, LRU, LFU, Random };
//写策略：全写法，写回法
enum write_way { write_through = 1, write_back };
#endif

typedef enum associativity_way ASSOC;
typedef enum replacement_way REPLACE;
typedef enum write_way WRITE;
///////////////////////////////////////////////////////
extern unsigned int long i_cache_size; //cache size
extern unsigned int long i_cache_line_size; //cacheline size
extern unsigned int long i_cache_set; //cache set

extern unsigned int long i_num_line; //How many lines of the cache.
extern unsigned int long i_num_set; //How many sets of the cache.

extern ASSOC t_assoc; //associativity method
extern REPLACE t_replace; // replacement policy
extern WRITE t_write; //write policy
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
extern short unsigned int bit_block; //How many bits of the block.
extern short unsigned int bit_line; //How many bits of the line.
extern short unsigned int bit_tag; //How many bits of the tag.
extern short unsigned int bit_set; //How many bits of the set.

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
extern unsigned long int i_num_access; //Number of cache access
extern unsigned long int i_num_load; //Number of cache load
extern unsigned long int i_num_store; //Number of cache store
extern unsigned long int i_num_space; //Number of space line

extern unsigned long int i_num_hit; //Number of cache hit
extern unsigned long int i_num_load_hit; //Number of load hit
extern unsigned long int i_num_store_hit; //Number of store hit

extern float f_ave_rate; //Average cache hit rate
extern float f_load_rate; //Cache hit rate for loads
extern float f_store_rate; //Cache hit rate for stores
///////////////////////////////////////////////////////


extern std::bitset<32> cache_item[MAX_CACHE_LINE]; // [31]:valid,[30]:hit,[29]:dirty,[28]-[0]:data
extern unsigned long int LRU_priority[MAX_CACHE_LINE]; //For LRU policy's priority ,标识cache中行的优先级
extern unsigned long int current_line; // The line num which is processing
extern unsigned long int current_set; // The set num which is processing
extern unsigned long int i,j; //For loop
extern unsigned long int temp; //A temp varibale

bool GetHitNum(char *address);
void GetHitRate(void);
bool IsHit(std::bitset<32> flags);
void GetReplace(std::bitset<32> flags);
void GetRead(std::bitset<32> flags);
void GetWrite();

void InitVariables(void); // -> InitVariables.cpp
void GetInput(void); 
void CalcInfo(void);
void CreateCache(void);
void FileTest(void);
void PrintOutput(void);

void LruHitProcess();
void LruUnhitSpace();
void LruUnhitUnspace();

