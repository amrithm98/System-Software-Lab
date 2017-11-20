#include <iostream>
#include <fstream>
using namespace std;
class HashMap
{
    public:
        vector<string> arr;
        vector<pair<string,int>> indices;
        int index;
        int size;
        HashMap()
        {
            index = 0;
            size = 10;
            arr = new vector<string(size);
        }
        HashMap(int i,int s)
        {
            index = i;
            size = s;
            arr = new vector<string(size);
        }
        void insert(string key,string val)
        {
            int ins_pos = (index + 1)%size;
            arr[ins_pos] = val;
            indices.push_back(make_pair(key,ins_pos));
        }
        string get(string key)
        {
            return arr[indices[key]];
        }
};

void generateSymtab(string fileName)
{
    HashMap symtab;
    symtab.insert("A","B");
    symtab.insert("amrith","10");
    cout<<symtab.get("amrith")<<endl;
}

int main()
{
    string s;
    cout<<"\nEnter Source File Name: ";
    cin >> s;
    generateSymtab(s);
}