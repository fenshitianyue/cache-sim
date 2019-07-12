#include "base.h"

void FileTest(void){
  char filepath[100] = "./test_file/test_1.trace";
  bool flag;
  std::ifstream in_file;
  char address[13];
  std::cout << std::endl;
  std::cout << "Please input the path and name that you want to test!" << std::endl;
  std::cout << "\t" << "Use the default test file : 0" << std::endl;
  std::cout << "\t" << "Manually input the test file : 1" << std::endl;
  std::cout << ">";
  std::cin >> flag; 
  if(flag){
    std::cout << "input the path!" << std::endl;
    std::cout << ">";
    std::cin >> filepath;
  } 
  in_file.open(filepath, std::ios::in);
  while(in_file.fail()){
    std::cout << "Open error! Please check the path and name, and input again!" << std::endl; 
    std::cout << ">"; 
    std::cin >> filepath;
    in_file.open(filepath, std::ios::in);
  }

  while(!in_file.eof()){
    in_file.getline(address, 13);
    GetHitNum(address);
  }
  in_file.close();
  GetHitRate();
}

