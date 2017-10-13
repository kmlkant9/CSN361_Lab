#include <bits/stdc++.h>
using namespace std;

struct packet{
	int flag,offset;
	string data;
};

int main() {
	packet p1;	
	cout<<"Enter Message"<<endl;
	cin>>p1.data;
	int n=p1.data.size();
	p1.flag=0;
	p1.offset=0;


	int mtu; cout<<"Enter MTU"<<endl;
	cin>>mtu;
	//int noOfFragments; cout<<"enter no of fragments"<<endl;
	//cin>>noOfFragments;

	vector<packet> frags;
    //cout<<"Enter size of data in each fragment"<<endl;
	int sofData=mtu-20;//=n/noOfFragments; 
	//cin>>sofData;
    int i=0; int j=0;
	while(i<p1.data.size()) {
		packet temp;
		int ii=min(n-i,sofData);
		temp.data=p1.data.substr(i,ii);
		temp.offset=i;
        i=i+ii;
		if(i<n) temp.flag=1; else temp.flag=0;
        
        frags.push_back(temp);
	}

	for(int i=0; i<frags.size(); i++) {
		cout<<frags[i].flag<<" "<<frags[i].offset<<" "<<frags[i].data<<endl;
	}
}