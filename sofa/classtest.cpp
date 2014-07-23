#include <iostream>
#include <string>
#include <vector>
using namespace std;

class classTest{
	private:
		string name;
		int age;
		vector<int> data;
	public:
		classTest(string name, int age, vector<int> data);
		void disinfo();
};

classTest::classTest(string name, int age, vector<int> data){
	this->name=name;
	this->age=age;
	this->data=data;
}

void classTest::disinfo(){
	cout<<"name:"<<this->name<<endl;
	cout<<"age:"<<this->age<<endl;
	//cout<<"data:"<<this->data.size()<<endl;
	cout<<"data:";
	vector<int>::iterator it;
	for(it=this->data.begin(); it!=this->data.end(); it++){
		cout<<*it<<" ";
	}
	cout<<endl;
}

int main(){
	int arr[]={1,2,3,4,5};
	vector<int> data(arr,arr+5);
	data.push_back(6);
	data.push_back(7);
	data.push_back(8);
	
	classTest test=classTest("YangJun", 24, data);
	test.disinfo();
	
	return 0;
}

