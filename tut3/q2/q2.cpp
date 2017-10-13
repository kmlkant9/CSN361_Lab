#include <bits/stdc++.h>
using namespace std;

string lltoString(long long num) {
    stringstream sstream;
    sstream << std::hex << num;
    string result = sstream.str();
    //add 0 to single
    if(result.size()==1) result="0"+result;
    return result;
}

int main() {
    string ipv4;
    cin>>ipv4;
    string ipv6[8];

    int i=0;
    while(ipv4[i]!='.')
        i++;
    string ans=lltoString(stoi(ipv4.substr(0,i)));
    ipv6[6]="";
    ipv6[6]+=ans;
    int j=++i;
    while(ipv4[i]!='.')
        i++;
    ans=lltoString(stoi(ipv4.substr(j,i-j)));
    ipv6[6]+=ans;
    j=++i;
    while(ipv4[i]!='.')
        i++;
    ans=lltoString(stoi(ipv4.substr(j,i-j)));
    ipv6[7]="";
    ipv6[7]+=ans;
    j=++i;
    ans=lltoString(stoi(ipv4.substr(j,ipv4.size()-j)));
    ipv6[7]+=ans;

    // making rest 0
    for(int i=0; i<5; i++)
        ipv6[i]="0";
    ipv6[5]="ffff";

    //printing
    for(int i=0; i<8; i++) {
        cout<<ipv6[i];
        if(i<8-1) cout<<':';
    }
    cout<<endl;


    return 0;
}