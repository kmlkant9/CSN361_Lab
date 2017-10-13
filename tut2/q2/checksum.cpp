#include <bits/stdc++.h>
#define ll long long
using namespace std;


ll hexStringToLL(string st) {
    ll ans=0;
    for(int i=st.size()-1; i>=0; i--) {
        if(st[i]<='9')
            ans+=(st[i]-'0')*pow(16,(st.size()-1-i));
        else if(st[i]<='Z')
            ans+=(st[i]-'A'+10)*pow(16,(st.size()-1-i));
        else ans+=(st[i]-'a'+10)*pow(16,(st.size()-1-i));
    }
    return ans;
}

string lltoString(ll num) {
    string ans(4,' ');
    for(int i=3; i>=0; i--) {
        int rem=num%16;
        if(rem<10) ans[i]=(char)('0'+rem);
        else ans[i]=(char)('A'+rem-10);
        num=num/16;
    }
    return ans;
}

int main() {
    string ipheader; cin>>ipheader;

    cout<<"Calculating checksum . . ."<<endl;

    string checksum_field = ipheader.substr(20,4);
    ll ichecksum_field = hexStringToLL(checksum_field);
    cout<<"checksum_field = "<<checksum_field<<endl;
    
    ll sum=0;
    for(int i=0; i<10; i++) {
        if(i!=5)
            sum += hexStringToLL(ipheader.substr(i*4,4));
        }
    cout<<"Sum = "<<sum<<endl;//<<" "<<lltoString(sum)<<endl;
    //cout<<"Given Sum = "<<hexStringToLL("2AF4B")<<endl;

    ll maxSum=pow(16,4);
    while(sum>(maxSum-1)) {
        ll carry = sum/maxSum;
        sum = sum%maxSum + carry;
    }
    //cout<<"sum = "<<sum<<endl;
    string sum_hex =lltoString(sum); 
    cout<<"sum = "<<sum_hex<<endl;
    int sum_complement=INT_MAX-sum;
    cout<<"Checksum = "<<lltoString(sum_complement)<<endl;
    
}
