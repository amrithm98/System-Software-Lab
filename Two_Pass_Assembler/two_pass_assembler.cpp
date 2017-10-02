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

/********
 * This Program is Created By Amrith M (S5 CSE)
 * 1. Program uses string streams for frequent hex->dec->string conversions
 * 2. first_pass function returns a symtab map and writes to an intermediate file
 * 3. second_pass function writes to output
 * 4. both first_pass and second_pass is the implementation of the exact SIC algorithm
 * 5. implementation of certain operations(substring manipulation etc might seem odd)
 * 6  There are two classes Line and TextRecord which are for handling multiple objects of same kind
 * 7. This is not an efficient Implementation. There exist some code redundancy.
 * 8. setw(),setfill() etc are used for maintaning the size of each block
 * 9. Supports Indexed Addressing as well
 * 
 *********/

/*To read a Line from File... Compatible for source and Intermediate files*/

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

/*To Create and Mange a Tex Record. There may be many. Length of record and all should be found*/

class TextRecord
{
    public:
    
    /*Content of Text Record*/

     string s;

    /*Start,End Address in Intermediate File Corresponding to each Text Record*/

     int start,end;

    TextRecord()
    {
        /*Leaving space for start Addr and Record Sizes*/

        s="T^      ^  ";
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

    void appendOpcode(string opCode)
    {
        s+=("^"+opCode);
    }
};

/*Reading Each Line of Input.. Max 3 Items*/

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

/*Reading Each Line of Intermediate File*/

Line readLine_symTab(istream &file)
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

/*Converting decimal to hexadecimal string as it can't be stored anywhere as hexadecimal*/

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

map<string,string> first_pass(string fileName,map<string,string> optab)
{

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

        /* Write First Line to intermediate File */

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
                /* Remove C,X or whatever and the two apostrophies = remove 3 characters */
                int inc=line.operand.size()-3;  
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

        //Read the next Line
        line=readLine(file);
    }

    //Write the last Line (END)

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

        /* Writes header record */

        output<<headerRecord<<progSize.str()<<"\n";

        line=readLine_symTab(intermediate);
    }
    /* 10 TextRecord objects and variables to store addresses */

    TextRecord TR[10];
    int textRecordIndex=0;  //Index of a Text Record..Increases whenever needed
    int start=0,finish=0;   //Update When Each Record Starts and Ends
    int next_record_start_flag=1;
    while(line.opCode!="END")
    {
        /**
         * When A Record reaches maximum size,Opcode is inserted to new record and this flag is set
         * When next_record_start_flag is set, the address and size of previous record is set.
         * Here we convert Hex String to decimal
         * */
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

        /* If this is not a comment Line */

        if(line.label!=".")
        {   
            /* If there exists an opCode . Excludes Directives */
            if(opTab.count(line.opCode))
            {
                /* If the operand is not empty */
                
                if(line.operand.compare(""))
                {
                    /* Checking if INDEX MODE is used */
                    size_t found = line.operand.find(',');
                    if(found!=string::npos)
                    {
                        //Indexed
                        //1010->9010
                        string hexAddr=convert_to_hex_string(stoi(symTab[line.operand.substr(0,found)]));
                        char firstBit=hexAddr[0];

                        /* Uses Bitset to Manipulate MSB bit */
                        /* firstbit will be char '1' or '2' or '0' etc... */
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
                    /* Not INDEXED MODE*/
                    else
                    {
                        string opCode="";
                        opCode+=(opTab[line.opCode]+convert_to_hex_string(stoi(symTab[line.operand])));
                        cout<<opCode<<"\n";

                        /*Checking if an opCode can be inserted,
                        if not sets the flag and starts with new record*/

                        if(TR[textRecordIndex].isValidRecord(opCode.size()))
                            TR[textRecordIndex].appendOpcode(opCode);
                        else
                        {
                            textRecordIndex++;  //Starts new record
                            TR[textRecordIndex].appendOpcode(opCode);
                            //Start Next Record Flag--> To set Start And Finish
                            next_record_start_flag=1;
                        }
                    }
                }
                /*if Operand is Empty, Pad with Zeros */
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
                    next_record_start_flag=1;
                }
            }
            else if(line.opCode=="BYTE")
            {
                cout<<"\nBYTE : "+line.operand;
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
                    next_record_start_flag=1;
                }
            }
            else if(line.opCode=="RESW" || line.opCode=="RESB")
            {
                int inc;
                stringstream ss;
                ss<<hex<<line.operand;
                ss>>inc;
                if(line.opCode=="RESW")
                    inc*=3;
                locCtr+=inc;
            }
        }
        /* Keeps Track of the last location */
        stringstream ss;
        ss<<line.loc;
        ss>>hex>>locCtr;
        line=readLine_symTab(intermediate);
    }

    /* Set start and size of last used text record */

    TR[textRecordIndex].start=start,TR[textRecordIndex].end=locCtr;
    TR[textRecordIndex].addStartAndRecordLength();

    /*Write TextRecords to File*/

    for(int i=0;i<textRecordIndex+1;i++)
        output<<TR[i].s<<endl;
    
    /*Write End Record*/

    output<<"E^"<<setw(6)<<setfill('0')<<hex<<TR[0].start;

    cout<<"\nSUCCESSFULLY ASSEMBLED THE PROGRAM!!";

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