#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>
#include <bitset>
using namespace std;

int locCtr = 0;

class SymTab
{
    public:
        string label;
        int addr;
        vector<int> forward_references;

        SymTab(string label,int addr)
        {
            this->label = label;
            this->addr = addr;
        }

        SymTab(string label,vector<int> ref)
        {
            this->label = label;
            this->addr = -1;
            this->forward_references = ref;
        }
};

class TextRecord
{
    public:
    
    /*Content of Text Record*/

     string s;

    /*Start,End Address in Intermediate File Corresponding to each Text Record*/

     int start,end,size;

    TextRecord()
    {
        /*Leaving space for start Addr and Record Sizes*/
        stringstream ss;
        ss<<setw(6)<<setfill('0')<<hex<<locCtr;
        ss>>start;
        
        s="T^"+ss.str()+"^  ";
        size = 0;
    }

    TextRecord(int locctr)
    {
        /*Leaving space for start Addr and Record Sizes*/
        stringstream ss;
        ss<<setw(6)<<setfill('0')<<hex<<locctr;
        ss>>start;
        
        s="T^"+ss.str()+"^  ";
        size = 0;
    }

    /*For adding start address and record length at right places when a TR is complete*/
    
    void addStartAndRecordLength()
    {

        stringstream ss;
        
        /*Replace with Record Length
        start and end are decimal addresses*/
        
        ss<<setw(2)<<setfill('0')<<hex<<end-start;
        string addr=ss.str();
        s.replace(9,2,addr);

        /*Replace with Starting Address*/

        stringstream temp;
        temp<<setw(6)<<setfill('0')<<hex<<start;
        s.replace(2,6,temp.str());
    }

    /*Checks if an OpCode can be appended*/

    bool isValidRecord(int next_opcode_length)
    {
        if(s.size()+next_opcode_length<=69)
            return 1;
        else
            return 0;
    }

    /*Append an OpCode*/

    void appendOpcode(string opCode,int len)
    {
        s+=("^"+opCode);
        size += len;
    }

    string finalTR()
    {
        stringstream ss;
        ss<<setw(2)<<setfill('0')<<hex<<size;
        s.replace(9,2,ss.str());
        return s;
    }
};

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

Line readLine(istream &file)    
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

string convert_to_hex_string(int addr)
{
    stringstream ss;
    ss<<hex<<addr;
    return ss.str();
}

/* opTab is a map */
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

vector<SymTab> symtab;
vector<TextRecord> textRecords;

int insertIntoSymtab(string label,int locctr,TextRecord &currentTR)
{
    for(int i = 0; i < symtab.size(); i++)
    {
        if(symtab[i].label == label)
        {
            if(symtab[i].addr == -1)
            {
                symtab[i].addr = locctr;

                if(currentTR.size > 0)
                {
                    textRecords.push_back(currentTR);
                    currentTR = *new TextRecord();
                }

                stringstream ss;
                ss<<setw(4)<<setfill('0')<<hex<<locctr;

                for(auto it: symtab[i].forward_references)
                {
                    TextRecord fwd = *new TextRecord(it);
                    fwd.appendOpcode(ss.str(),2);
                    textRecords.push_back(fwd);
                }
            }
            else
            {
                cout<<"\nError: Variable Reused --> "<<label<<endl;
                return 0;
            }
        }
    }
    symtab.push_back(*new SymTab(label,locctr));
    return 1;
    
}

int insertForwardReference(string label,int locctr)
{
    for(int i = 0; i < symtab.size(); i++)
    {
        if(symtab[i].label == label)
        {
            symtab[i].forward_references.push_back(locctr);
            return 1;
        }
    }
    vector<int> fwd;
    fwd.push_back(locctr);
    symtab.push_back(*new SymTab(label,fwd));
    return 1;
}

int getOperandValue(string label)
{
    for(int i = 0; i < symtab.size() ; i++)
    {
        if(symtab[i].label == label)
        {
            return symtab[i].addr;
        }
    }
    return -1;
}

void assembleProgram(string fileName,map<string,string> opTab)
{
    ifstream file(fileName);    //Reading Input File
    ofstream output("output");

    int startAddress;

    string programName,headerRecord;

    Line line=readLine(file);

    if(line.opCode=="START")
    {
        stringstream temp,loc,progName;
        temp<<setw(4)<<setfill('0')<<hex<<line.operand;
        temp>>locCtr;   
        startAddress=locCtr;
        programName=line.label;

        loc<<setw(6)<<setfill('0')<<hex<<line.loc;
        progName<<setw(6)<<setfill(' ')<<line.label;

        headerRecord="H^"+progName.str()+"^"+temp.str()+"^";
    }
    else
    {
        locCtr = 0;
    }

    line=readLine(file);
    int finAddr=0;
    TextRecord currentTR;
    while(line.opCode != "END")
    {   
        int new_record = 0;
        int entry_length = 0;
        if(line.label[0]!='.')
        {
            stringstream temp;
            finAddr = locCtr;
            string record = "";
            cout<<setw(4)<<setfill('0')<<hex<<locCtr<<": "<<line.label<<" "<<line.opCode<<" "<<line.operand<<endl;

            if(line.label != "")
            {
                cout<<"\nInsert symtab: "<<line.label<<endl;
                insertIntoSymtab(line.label,locCtr,currentTR);
            }

            if(opTab.find(line.opCode)!=opTab.end())
            {   
                entry_length = 3;
                record  += opTab[line.opCode];
                if(line.operand != "")
                {
                    int sym_addr = getOperandValue(line.label);
                    cout<<"\nLabel and sym_addr"<<line.label<<" "<<sym_addr<<endl;
                    if(sym_addr == -1)
                    {
                        //Create fwd ref
                        insertForwardReference(line.operand,locCtr+1);
                        cout<<"\nForward Reference: "<<line.label<<endl;
                        //add 0000 to TR
                        record += "0000";
                    }
                    else
                    {
                        stringstream ss;
                        ss<<setw(4)<<setfill('0')<<hex<<sym_addr;
                        record += ss.str();
                    }
                }
                else
                {
                    record += "0000";
                }
            }
            else if(line.opCode == "WORD")
            {
                stringstream ss;
                ss<<setw(6)<<setfill('0')<<hex<<line.operand;
                record += ss.str();
                entry_length = 3;
            }
            else if(line.opCode == "BYTE")
            {
                // cout<<"\nBYTE : "+line.operand<<endl;
                stringstream ss;
                string opCode="";
                int len = line.operand.length() - 3;
                if(line.operand[0]=='C')
                {
                    for(int i=2;i<line.operand.size()-1;i++)
                    {
                        ss<<hex<<(int)line.operand[i];
                    }
                }
                else if(line.operand[0]=='X')
                {
                    ss<<line.operand.substr(2,2);
                    len /= 2;
                }
                record += ss.str();
                entry_length = len;

            }
            else if(line.opCode == "RESW")
            {
                entry_length = 3*atoi(line.operand.c_str());
                new_record = 1;
            } 
            else if(line.opCode == "RESB")
            {
                entry_length = atoi(line.operand.c_str());
                new_record = 1;
            }
            else
            {
                cout<<"\nError: Invalid opCode "<<line.opCode<<endl;
                return;
            }
            // cout<<locCtr << " "<<entry_length <<endl;
            locCtr += entry_length;

            if(new_record)
            {
                textRecords.push_back(currentTR);
                currentTR = *new TextRecord();
            }
            else
            {
                if(currentTR.isValidRecord(entry_length))
                {
                    currentTR.appendOpcode(record,entry_length);
                }
                else
                {
                    textRecords.push_back(currentTR);
                    currentTR = *new TextRecord();
                    currentTR.appendOpcode(record,entry_length);
                }
            }
        }
        line = readLine(file);
    }
    textRecords.push_back(currentTR);

    for(int i = 0; i < textRecords.size(); i++)
    {
        if(textRecords[i].size>0)
            cout<<textRecords[i].finalTR()<<endl;
    }

    stringstream ss;
    ss<<hex<<locCtr-startAddress+1;
    headerRecord+=ss.str();

    cout<<headerRecord<<endl;
    stringstream es;
    es<<hex<<startAddress;
    string endRecord="E^"+es.str();

    output<<headerRecord<<endl;
    for(int i = 0; i < textRecords.size(); i++)
    {
        if(textRecords[i].size>0)
            output<<textRecords[i].finalTR()<<endl;
    }
    output<<endRecord;
}

int main()
{   
    string fileName;

    map<string,string> opTab;

    init_optab(opTab);

    cout<<"\nEnter File Name: ";
    cin>>fileName;

    assembleProgram(fileName,opTab);

    return 0;
}