#include <iostream>
#include <map>
#include <vector>
using namespace std;

vector<string> generate_address(string fileName)
{

}

int main()
{
    string fileName;
    cout<<"\nEnter Name of the object Program to Load: ";
    cin >> fileName;

    vector<string> address = generate_address(fileName);
    
    cout<<"\nLoaded Addresses: \n======================\n";
    for(auto it : address)
    {
        cout<<it<<endl;
    }
}