#include "stdafx.h" 
#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <iterator> 

using namespace std; 

int main() 
{ 
  vector<string> input_text; 
  vector<string>::iterator it; 
  fstream file; 
  file.open("D:/codeNomUpgrade.c",ios::in); 
  while (!file.eof()) 
  { 
    string tmp(""); 
    getline(file, tmp); 
    input_text.push_back(tmp); 
  } 
  for (it = input_text.begin(); it < input_text.end(); it++) cout « *it « endl; 
  for (it = input_text.begin(); it < input_text.end(); it++) 
  { 
    if ((*it).find("//") != -1) 
    { 
      (*it).erase(); 
    } 
    else if ((*it).find("/*") != -1) 
    { 
      do 
      { 
        (*it).erase(); 
        it++; 
      }
      while ((*it).find("*/") == -1); 
      (*it).erase(); 
    } 
  } 

  cout « "___________________________" « endl; 

  for (it = input_text.begin(); it < input_text.end(); it++) cout « *it « endl; 

  file.open("D:/codeNomUpgrade_result.c", ios::out); 


  return 0; 
}
