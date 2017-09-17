#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>
using namespace std;

class Line  
{
    public:
        string label,opCode,operand,loc;
        Line(string a,string b,string c,string d)
        {
            label=a;
            opCode=b;
            operand=c;
            loc=d;
        }
};

Line readLine(istream &file)    //Reading Each Line of Input.. Max 3 Items
{
    string line="",label,opCode,operand;

    //Just Read One Line
    while(line.length()==0)
    {
        getline(file,line);
    }

    stringstream ss(line);

    getline(ss,label,' ');
    getline(ss,opCode,' ');
    getline(ss,operand,' ');

    return Line(label,opCode,operand,"");
}

Line readLine_symTab(istream &file) //Reading Each Line of Intermediate File
{
    string line="",label,opCode,operand,loc;

    //Just Read One Line
    while(line.length()==0)
    {
        getline(file,line);
    }

    stringstream ss(line);
    getline(ss,loc,' ');
    getline(ss,label,' ');
    getline(ss,opCode,' ');
    getline(ss,operand,' ');

    return Line(label,opCode,operand,loc);
}

void init_optab(map<string,string> &map)
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

map<string,string> first_pass(string fileName,map<string,string> optab)
{
    string assembler_directives[]={"BYTE","WORD","RESB","RESW"};

    ifstream file(fileName);    //Reading Input File

    ofstream intermediateFile("intermediate.txt");  //Writing Intermediate File
    
    map<string,string> symtab;  //Map For SymTab

    int locCtr=0,startAddress;

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
        // cout<<"\nNo START Command";
        // exit(0);
        locCtr=0;
    }
    //Read the next Line

	line=readLine(file);

    int finAddr=0;

    while(line.label!="END")
    {
        //Not a comment or if the label is empty
        if(line.label=="" || line.label[0]!='.')
        {
            stringstream temp;
            //Prints Decimal Value ***1000H=4096D
            cout<<"\nLocCtr: "<<locCtr<<"\n"; 
            //To Store the Last Value of locCtr to find the ProgramSize
            finAddr=locCtr; 
            temp<<setw(4)<<setfill('0')<<hex<<locCtr;
            intermediateFile<<temp.str()<<" "+line.label+" "+line.opCode+" "+line.operand<<"\n";

            if(line.label!="")
            {
                if(symtab.find(line.label)==symtab.end())
                    symtab[line.label]=to_string(locCtr);
                else
                {
                    cout<<"\nError:Duplicate Label "<<line.label<<"\t("<<hex<<symtab[line.label]<<")";
                    exit(0);
                }
            }
            if(line.opCode=="WORD")
            {
                locCtr+=3;
            }
            else if(optab.count(line.opCode))   //If OpCode Exists in opTab
            {
                locCtr+=3;
            }
            else if(line.opCode=="RESW" || line.opCode=="RESB")
            {
                int inc;
                stringstream sizeTemp;
                sizeTemp<<hex<<line.operand;
                sizeTemp>>inc;
                if(line.opCode=="RESW")
                {
                    inc*=3;
                }
                locCtr+=inc;
            }
            else if(line.opCode=="BYTE")
            {
                int inc=line.operand.size()-3;  //Remove C,X or whatever and the two apostrophies = remove 3 characters
                // cout<<"\nByte"<<inc<<"\n";
                if(line.operand[0]=='X')
                    inc/=2;
                locCtr+=inc;
            }
            else
            {
                cout<<"\nError: Invalid OpCode "<<line.opCode;
                exit(0);
            }
        }
        line=readLine(file);
    }
    
    symtab["programSize"]=to_string(finAddr-startAddress+3);
    
    ofstream symtabFile("SYMTAB");
    for(auto it:symtab)
        symtabFile<<it.first<<" "<<it.second<<endl;

    symtabFile.close();
    file.close();

    cout<<"\nCompleted Pass 1 Successfully!!!";
    return symtab;

}

void second_pass(map<string,string> opTab,map<string,string> symTab,string fileName)
{

    //Reading From Intermediate File
    ifstream input(fileName);
    ifstream intermediate("intermediate.txt");

    //Output File object
    ofstream output("OUTPUT");

    int locCtr=0;

    Line line=readLine_symTab(intermediate);

    if(line.opCode=="START")
    {
        stringstream loc,progName,progSize;
        loc<<setw(6)<<setfill('0')<<hex<<line.loc;
        progName<<setw(6)<<setfill('_')<<line.label;

        if(line.label=="")
            line.label="OBJECT";
        
        int programSize=stoi(symTab["programSize"]);

        string start=loc.str();

        loc>>locCtr;

        string headerRecord="H^"+loc.str()+"^"+progName.str()+"^";
        progSize<<setw(6)<<setfill('0')<<programSize;

        output<<headerRecord<<progSize.str()<<"\n";

        line=readLine_symTab(intermediate);
    }
    





}

int main()
{
    string fileName;

    map<string,string> opTab;
    map<string,string> symTab;

    init_optab(opTab);

    cout<<"\nEnter File Name: ";
    cin>>fileName;

    symTab=first_pass(fileName,opTab);

    second_pass(opTab,symTab,fileName);

    return 0;
}