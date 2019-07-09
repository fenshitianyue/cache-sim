#include "base.h"

void CalcInfo(){
    assert(i_cache_line_size != 0);
    i_num_line = (i_cache_size << 10) / i_cache_line_size;
    temp = i_cache_line_size; //计算块内地址位数
    while(temp){
        temp >>= 1;
        ++bit_block;
    }
    --bit_block; 
    if(direct_mapped == t_assoc){
        bit_set = 0; // for direct_mapped,the bit_set is 0
        temp = i_num_line;
        while(temp){
            temp >>= 1;
            ++bit_line;
        }

        --bit_line; 
    }
    else if(full_associative == t_assoc){
        bit_line = 0; // for full_associative,the bit_line is 0
        bit_set = 0; // for full_associative,the bit_set is 0
    }
    else if(set_associative == t_assoc){
        bit_line = 0; // for set_associative,the bit_line is 0
        assert(i_cache_set != 0);
        assert(i_num_line > i_cache_set);
        i_num_set = i_num_line/i_cache_set;
        temp = i_num_set;

        while(temp){
            temp >>= 1;
            ++bit_set;
        }

        --bit_set;
    }

    bit_tag = 32ul - bit_block - bit_line - bit_set;
    assert(bit_tag <= 29); //32-valid-hit-dirty
    std::cout << "i_cache_line_size: " << i_cache_line_size << "B" << std::endl; // 显示块大小
    std::cout << "i_cache_size: " << i_cache_size << "KB" << std::endl; // 显示cache数据区总容量

    if(set_associative == t_assoc){ // 如果为组相联，显示是几路组相联
        std::cout << "i_cache_set: " << i_cache_set << " lines each set" << std::endl;
    }

    std::cout << "i_num_line: " << i_num_line << std::endl; // 显示共有多少行

    if(set_associative == t_assoc){ // 如果为组相联，显示共有几组
        std::cout << "i_num_set: " << i_num_set << std::endl;
    }

    std::cout << "bit_block: " << bit_block << std::endl; // 显示块内地址所需位数

    if(direct_mapped == t_assoc){ // 如果为直接映射，显示行号所需位数
        std::cout << "bit_line: " << bit_line << std::endl;
    }

    if(set_associative == t_assoc){ // 如果为组相联，显示组号所需位数
        std::cout << "bit_set: " << bit_set << std::endl;
    }

    std::cout << "bit_tag: " << bit_tag << std::endl; // 显示标志位所需位数
}
