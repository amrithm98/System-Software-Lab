#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

void init_optab(unordered_map<string,string> &map)
{
    map["ADD"]="18";
	map["AND"]="40";
	map["COMP"]="28";
	map["DIV"]="24";
	map["J"]="3C";
	map["JEQ"]="30";
	map["JGT"]="34";
	map["JLT"]="38";
	map["JSUB"]="48";
	map["LDA"]="00";
	map["LDCH"]="50";
	map["LDL"]="08";
	map["LDX"]="04";
	map["MUL"]="20";
	map["OR"]="44";
	map["RD"]="D8";
	map["RSUB"]="4C";
	map["STA"]="0C";
	map["STCH"]="54";
	map["STL"]="14";
	map["STSW"]="E8";
	map["STX"]="10";
	map["SUB"]="1C";
	map["TD"]="E0";
	map["TIX"]="2C";
    map["WD"]="DC";
}

int main()
{
    string fileName;

    unordered_map<string,string> optab;
    init_optab(optab);

    cout<<"\nEnter File Name: ";
    cin>>fileName;

    ifstream file(fileName);
    for(string line;getline(file,line);)
    {
        stringstream ss(line);
        string temp;
        vector<string> tokens;
        while(ss>>temp)
        {
            tokens.push_back(temp);
        }
    }


}