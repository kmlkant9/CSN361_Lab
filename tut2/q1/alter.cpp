#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
	char input[100],poly[10];
	cin>>input>>poly;
	
	int idx=atoi(argv[1]);
	input[idx]^=1;
	cout<<input<<endl;
	cout<<poly<<endl;
}