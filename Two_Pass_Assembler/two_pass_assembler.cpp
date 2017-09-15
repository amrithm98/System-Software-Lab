#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

class Line  
{
    public:
        string label,opCode,operand;
        Line(string a,string b,string c)
        {
            label=a;
            opCode=b;
            operand=c;
        }
};

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

void first_pass(string fileName)
{
    string assembler_directives[]={"BYTE","WORD","RESB","RESW"};

    ifstream file(fileName);

    unordered_map<string,string> symtab;

    string locCtr,programName;
    
    /*First Line of Program Contains a START Command Followed By Starting Address*/
    string start_line;
    getline(file,start_line);
    stringstream ss(start_line);
    string temp;
    vector<string> tokens;

    while(ss>>temp)
    {
        tokens.push_back(temp);
    }
    for(int i=0;i<tokens.size();i++)
    {
        
        if(tokens[i]=="START")
        {
            locCtr=tokens[i+1];
            break;
        }
        else
        {
            programName=tokens[i];
        }
    }

    for(string line;getline(file,line);)
    {
        stringstream ss(line);   

        while(ss>>temp)
        {
            tokens.push_back(temp);
        }

        int size=tokens.size();
    
        switch(size)
        {
            case 3:
                break;
            case 2:
                break;
            case 1:
                break;
        }
    }
}

int main()
{
    string fileName;

    unordered_map<string,string> optab;
    init_optab(optab);

    cout<<"\nEnter File Name: ";
    cin>>fileName;

    first_pass(fileName);

}