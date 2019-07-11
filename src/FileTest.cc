#include "base.h"

void FileTest(void){
  char filepath[100];
  std::ifstream in_file;
  char address[13];
  std::cout << std::endl;
  std::cout << "Please input the path and name that you want to test!" << std::endl;
  std::cout << "(eg:C:\\temp\\myfile.trace" << std::endl;
  std::cout << "myfile.trace" << std::endl;
  std::cin >> filepath;
  in_file.open(filepath, std::ios::in);
  while(in_file.fail()){
    std::cout << "Open error! Please check the path and name, and input again!" << std::endl; 
    std::cin >> filepath;
    in_file.open(filepath, std::ios::in);
  }
#ifdef OUTPUT 
  int i_line_proceded = 0;
  ofstream out_put;
  output.open("test.log", std::ios::end);
#endif
  while(!in_file.eof()){
    in_file.getline(address, 13);
    bool __attribute__((unused)) in_success = GetHitNum(address); 
    assert(in_success);
#ifdef OUTPUT 
    ++i_line_proceded;
    out_put << i_line_proceded << std::endl;
    std::cout << address << std::endl;
#endif

#ifdef OUTPUT 
    out_put.close();
#endif
    in_file.close();
    GetHitRate();
  }
}
