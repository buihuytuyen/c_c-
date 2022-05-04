#pragma comment(lib, "ws2_32.lib")

#include<bits/stdc++.h>
#include<winsock2.h>

using namespace std;
//Step 1: Inittiate WinSock
WSADATA wsaData;
WORD wVersion = MAKEWORD(2, 2);
if ( WSAStartup(wVersion, &wsaData) != 0){
    printf("Version is not supported\n");
}


int main(){
    
    return 0;
}