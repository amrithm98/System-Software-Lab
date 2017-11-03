#include <iostream>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

int locCtr = 0;

void generate_address(string fileName)
{
    ifstream objectProg(fileName);
    vector<string> address;
    string progName,startAddress,size;
    int startAddr = 0;
    if(objectProg.is_open())
    {
        string line;
        getline(objectProg,line);
        while(line[0]!='E')
        {
            // cout<<line<<endl;
            if(line[0] == 'H')
            {   
                progName = line.substr(2,6);
                startAddress = line.substr(9,6);
                size = line.substr(16);
                //Remove trailing 0s
                progName.erase(0, progName.find_first_not_of('0'));
                startAddress.erase(0, startAddress.find_first_not_of('0'));
                size.erase(0, size.find_first_not_of('0'));

                cout<<"\nProgram Name: "<<progName<<endl;
                cout<<"\nProgram size: "<<size<<endl;
                cout<<"\nProgram Start: "<<startAddress<<endl;

                //Store starting address
                startAddr = stoi(startAddress);
            }
            else
            {
                int cur_rec_address = stoi(line.substr(2,6));
                string s = "";
                for(char c : line.substr(12))
                {
                    if(c != '^')
                    {
                        s += c;
                    }
                    else
                    {
                        address.push_back(s);
                        s = "";
                    }
                }
                if(s!="")
                    address.push_back(s);
            }
            getline(objectProg,line);
        }
    }
    else
    {
        cout<<"\nUnable To Open File!!!";
    }
    map<int,string> loaded_prog;
    loaded_prog[startAddr] = "";
    int string_size_so_far = 0,curr_addr = startAddr;
    for(auto it : address)
    {
        int curr_length = it.size();

        if(loaded_prog[curr_addr].size() + curr_length > 32)
        {
            int reqd_length_of_opCode = 32 - loaded_prog[curr_addr].size();
            loaded_prog[curr_addr] += (it.substr(0,reqd_length_of_opCode));
            curr_addr += 10;
            loaded_prog[curr_addr] = it.substr(reqd_length_of_opCode);
        }
        else
        {
            loaded_prog[curr_addr] += (it);
        }
    }
    
    cout<<"\nLoaded Addresses: \n===================="<<endl;
    for(auto it:loaded_prog)
    {
        cout<<it.first<<" "<<it.second<<" (Size of Object Code: "<<it.second.size()<<")"<<endl;
    }
}

int main()
{
    string fileName;
    cout<<"\nEnter Name of the object Program to Load: ";
    cin >> fileName;

    generate_address(fileName);
    return 0;
}