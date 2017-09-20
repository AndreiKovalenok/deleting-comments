#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

void fnoncoments(string & onestr)
{
    ofstream fout;
    fout.open("code_after.C", ios::in);
    fout.seekp(0, ios::end);
    
    fout << onestr << endl;
    fout.close();
}


int main()
{
    vector<string> input_text;
    vector<string>::iterator it;
    fstream file;
    file.open("code_before.C",ios::in);
    
    while (!file.eof())
    {
        string tmp("");
        getline(file, tmp);
        input_text.push_back(tmp);
    }
    
    for (it = input_text.begin(); it < input_text.end(); it++)
    {
        if ((*it).find("//") != -1)
        {
            (*it).erase((*it).find("//"),(*it).size());
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
    
    for (it = input_text.begin(); it < input_text.end(); it++)
    {
        cout << *it << endl;
        fnoncoments(*it);
    }
    return 0; 
}
