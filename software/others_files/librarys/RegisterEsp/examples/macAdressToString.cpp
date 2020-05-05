#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

#define count 5

static int8_t cliMacAddr[] = {0x74,0x69,0x69,0x2D,0x30,0x32};



string macAdressToString(int8_t addr[]){
    
    string mac1 = to_string(addr[0]);
    string mac2 = to_string(addr[1]);
    string mac3 = to_string(addr[2]);
    string mac4 = to_string(addr[3]);
    string mac5 = to_string(addr[4]);
    string mac = mac1 + mac2 + mac3 + mac4 + mac5;

    return mac;
     

}

int main(){
    cout << macAdressToString(cliMacAddr) + "\n" ;
    cout << cliMacAddr[0] ;
    cout << cliMacAddr[1];
    cout << cliMacAddr[2];
    cout << cliMacAddr[3];
    cout << cliMacAddr[4];
    
    return 0;
}