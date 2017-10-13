#include <bits/stdc++.h>
using namespace std;

void modify(const char* poly,char* output,const int n) {
	for(int i=n; i<n+strlen(poly); i++) {
		if(output[i]==poly[i-n])
			output[i]='0';
		else output[i]='1';
	}
}

int main() {
	char input[100],poly[10],output[100];
	cin>>output; cin>>poly;
		
	for(int i=0; i<strlen(output)-strlen(poly)+1; i++) {
		if(output[i]=='1')
			modify(poly,output,i);
		else output[i]='0';
		//cout<< output<<endl;
	}
	
	bool flag=false;
	for(int i=0; i<strlen(output); i++)
		if(output[i]!='0')
			flag=true;
	if(flag) cout<<"Wrong\n";
	else cout<<"Correct\n";
	return 0;
}

