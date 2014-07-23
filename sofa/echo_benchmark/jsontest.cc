#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <vector>
#include <string>
#include <iostream>
using namespace boost::property_tree;
using namespace std;

int main(){
	try{
		ptree pt,pt1,pt2;
		string str;
		read_json("testdata.data", pt);
		pt1 = pt.get_child("detail");
		vector<string> arr;
		for(ptree::iterator it = pt1.begin(); it != pt1.end(); ++it){
			pt2 = it->second; //first为空
			//str = pt2.get<string>("name");
			str = pt2.get<string>("dept");
			arr.push_back(str);
		}
		
		cout<<"name:";
		vector<string>::iterator it;
		for(it=arr.begin(); it!=arr.end(); it++){
			cout<<*it<<" ";
		}
		cout<<endl;
		
		cout<<pt.get_child("info").get<string>("dept")<<endl;
		
		cout<<pt.get<string>("name")<<endl;
		
		vector<string> arr2;
		pt1=pt.get_child("data");
		for(ptree::iterator it = pt1.begin(); it != pt1.end(); ++it){
			//pt2 = it->second; //first为空
			//str = pt2.get<string>("name");
			//str = pt2.get<string>("dept");
			//arr.push_back(str);
			assert((*it).first.empty()); // array elements have no names
			arr2.push_back((*it).second.data());
		}
		for(it=arr2.begin(); it!=arr2.end(); it++){
			cout<<*it<<" ";
		}
		cout<<endl;
	}
	catch(exception const& e){
		cout<<e.what()<<endl;
	}
	
	return EXIT_SUCCESS;
}
