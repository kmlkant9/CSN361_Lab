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
	cin>>input; cin>>poly;
	for(int i=0; i<strlen(input); i++)
		output[i]=input[i];
	for(int i=0; i<strlen(poly)-1; i++)
		output[i+strlen(input)]='0';
		
	for(int i=0; i<strlen(input); i++) {
		if(output[i]=='1')
			modify(poly,output,i);
		else output[i]='0';
		
	}
	for(int i=0; i<strlen(input); i++)
		output[i]=input[i];
	output[strlen(input)+strlen(poly)-1]='\0';
	cout<<output<<endl;
	cout<<poly<<endl;
	return 0;
}

