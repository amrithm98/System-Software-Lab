#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <iomanip>
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

Line readLine(istream &file)
{
    string line="",label,opCode,operand;

    while(line.length()==0)
    {
        getline(file,line);
    }

    stringstream ss(line);

    getline(ss,label,' ');
    getline(ss,opCode,' ');
    getline(ss,operand,' ');

    return Line(label,opCode,operand);
}

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

    ofstream intermediateFile("intermediate.txt");
    
    unordered_map<string,string> symtab;

    int locCtr,startAddress;

    string programName;

    /*First Line of Program Contains a START Command Followed By Starting Address*/
    Line line=readLine(file);

    if(line.opCode=="START")
    {
        stringstream temp;
        
        temp<<setw(4)<<setfill('0')<<hex<<line.operand;
        temp>>locCtr;

        startAddress=locCtr;
        programName=line.label;
        intermediateFile<<temp.str()<<" "<<line.label<<" "<<line.opCode<<" "<<line.operand<<"\n";
    }
    else
    {
        cout<<"\nNo START Command";
        exit(0);
    }

    // while(line.opCode!="END")
    // {
    //     if(line.label=="" || line.label[0]!='.')
    //     {

    //     }
    // }
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