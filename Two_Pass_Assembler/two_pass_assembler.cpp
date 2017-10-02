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
class TextRecord
{
    public:
     string s;
     int start,end; //Addresses

    TextRecord()
    {
        s="T^      ^  ";
    }

    void addStartAndRecordLength()
    {
        stringstream ss;
        ss<<setw(2)<<setfill('0')<<hex<<end-start;
        string addr=ss.str();
        s.replace(9,2,addr);

        stringstream temp;
        temp<<setw(6)<<setfill('0')<<hex<<start;
        s.replace(2,6,temp.str());
    }

    bool isValidRecord(int next_opcode_length)
    {
        if(s.size()+next_opcode_length<=69)
            return 1;
        else
            return 0;
    }

    void appendOpcode(string opCode)
    {
        s+=("^"+opCode);
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

string convert_to_hex_string(int addr)
{
    stringstream ss;
    ss<<hex<<addr;
    return ss.str();
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
        locCtr=0;
    }
    //Read the next Line

	line=readLine(file);

    int finAddr=0;

    while(line.opCode!="END")
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
                inc=stol(line.operand);
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


    intermediateFile<<hex<<locCtr<<" "<<line.label<<" "<<line.opCode<<" "<<line.operand<<"\n";

    cout<<locCtr<<"\t";

    stringstream ss;
    ss<<locCtr-startAddress+1;

    // symtab["programSize"]=(ss.str());
    symtab["programSize"]=(ss.str());
    
    ofstream symtabFile("SYMTAB");
    for(auto it:symtab)
        symtabFile<<it.first<<" "<<it.second<<" "<<hex<<stoi(it.second)<<endl;
    
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
        if(line.label=="")
            line.label="OBJECT";

        stringstream loc,progName,progSize;
        loc<<setw(6)<<setfill('0')<<hex<<line.loc;
        progName<<setw(6)<<setfill(' ')<<line.label;

        int programSize=stoi(symTab["programSize"]);

        string start=loc.str();

        loc>>locCtr;

        string headerRecord="H^"+progName.str()+"^"+loc.str()+"^";
        progSize<<setw(6)<<setfill('0')<<hex<<programSize;

        output<<headerRecord<<progSize.str()<<"\n";

        line=readLine_symTab(intermediate);
    }

    TextRecord TR[10];
    int textRecordIndex=0;  //Index of a Text Record..Increases whenever needed
    int start=0,finish=0;   //Update When Each Record Starts and Ends
    int next_record_start_flag=1;
    while(line.opCode!="END")
    {
        if(next_record_start_flag)
        {
            stringstream ss;
            ss<<line.loc;

            ss>>hex>>finish;        //Get's finish of textRecordIndex-1 
            
            if(textRecordIndex>0)
                TR[textRecordIndex-1].start=start,TR[textRecordIndex-1].end=finish;


            if(textRecordIndex>0)
                TR[textRecordIndex-1].addStartAndRecordLength();

            start=finish;
            locCtr=finish;
            next_record_start_flag=0;
        }

        cout<<"\nRecord Index: "<<textRecordIndex<<"\n"<<"Rec Size: "<<TR[textRecordIndex].s.size()<<"\n";
        if(line.label!=".")
        {   
            if(opTab.count(line.opCode))
            {
                if(line.operand.compare(""))
                {
                    size_t found = line.operand.find(',');
                    if(found!=string::npos)
                    {
                        //Indexed
                        //1010->9010
                        string hexAddr=convert_to_hex_string(stoi(symTab[line.operand.substr(0,found)]));
                        char firstBit=hexAddr[0];
                        bitset<4> b(firstBit-'0');
                        b[3]=1;
                        stringstream ss;
                        ss<<hex<<b.to_ulong();
                        hexAddr[0]=ss.str()[0];
                        string opCode="";
                        opCode+=(opTab[line.opCode]+hexAddr);
                        if(TR[textRecordIndex].isValidRecord(opCode.size()))
                            TR[textRecordIndex].appendOpcode(opCode);
                        else
                        {
                            textRecordIndex++;
                            TR[textRecordIndex].appendOpcode(opCode);
                            //Start Next Record Flag--> To set Start And Finish
                            next_record_start_flag=1;
                        }
                    }
                    else
                    {
                        string opCode="";
                        opCode+=(opTab[line.opCode]+convert_to_hex_string(stoi(symTab[line.operand])));
                        cout<<opCode<<"\n";
                        if(TR[textRecordIndex].isValidRecord(opCode.size()))
                            TR[textRecordIndex].appendOpcode(opCode);
                        else
                        {
                            textRecordIndex++;
                            TR[textRecordIndex].appendOpcode(opCode);
                            //Calculate Record Length

                            //Start Next Record Flag--> To set Start And Finish
                            next_record_start_flag=1;
                        }
                    }
                }
                else
                {
                    stringstream ss;
                    ss<<left<<setw(6)<<setfill('0')<<opTab[line.opCode];
                    if(TR[textRecordIndex].isValidRecord(ss.str().size()))
                            TR[textRecordIndex].appendOpcode(ss.str());
                    else
                    {
                        textRecordIndex++;
                        TR[textRecordIndex].appendOpcode(ss.str());

                        //Calculate Record Length
                        //Converting 1019->4121 (hex string to dec)
                        //Start Next Record Flag--> To set Start And Finish
                        next_record_start_flag=1;
                    }
                }
            }
            if(line.opCode=="WORD")
            {
                stringstream ss;
                ss<<setw(6)<<setfill('0')<<line.operand;
                string opCode=ss.str();
                if(TR[textRecordIndex].isValidRecord(opCode.size()))
                        TR[textRecordIndex].appendOpcode(opCode);
                else
                {
                    textRecordIndex++;
                    TR[textRecordIndex].appendOpcode(opCode);

                    //Calculate Record Length
                    //Converting 1019->4121 (hex string to dec)
                    //Start Next Record Flag--> To set Start And Finish
                    next_record_start_flag=1;
                }
            }
            else if(line.opCode=="BYTE")
            {
                cout<<"\nBYTE : "+line.operand;
                //Fix Ascii to opcode
                stringstream ss;
                string opCode="";
                if(line.operand[0]=='C')
                {
                    for(int i=2;i<line.operand.size()-1;i++)
                    {
                        ss<<hex<<(int)line.operand[i];
                    }
                    opCode=ss.str();
                }
                else if(line.operand[0]=='X')
                {
                    ss<<line.operand.substr(2,2);
                    opCode=ss.str();
                }

                if(TR[textRecordIndex].isValidRecord(opCode.size()))
                        TR[textRecordIndex].appendOpcode(opCode);
                else
                {
                    textRecordIndex++;
                    TR[textRecordIndex].appendOpcode(opCode);
                    //Calculate Record Length
                    //Converting 1019->4121 (hex string to dec)
                    //Start Next Record Flag--> To set Start And Finish
                    next_record_start_flag=1;
                }
            }
            else if(line.opCode=="RESW")
            {

            }
            else if(line.opCode=="RESB")
            {
                
            }
        }
        stringstream ss;
        ss<<line.loc;
        ss>>hex>>locCtr;
        line=readLine_symTab(intermediate);
    }

    TR[textRecordIndex].start=start,TR[textRecordIndex].end=locCtr;
    TR[textRecordIndex].addStartAndRecordLength();
    for(int i=0;i<textRecordIndex+1;i++)
        output<<TR[i].s<<endl;
    output<<"E^"<<setw(6)<<setfill('0')<<hex<<TR[0].start;



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