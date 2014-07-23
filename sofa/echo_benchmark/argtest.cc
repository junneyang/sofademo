#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
using namespace std;
using namespace boost::program_options;

int main(int argc, char* argv[]){
	options_description desc("necessary options");
	vector<int> arr;
	
	desc.add_options()
		("int,i", value<int>()->default_value(123), "designate a int")
		("str,s", value<string>()->default_value("string"), "designate a string")
		("arr,a", value<vector<int> >(&arr)->multitoken(), "designate a vector")
		("help,h", "help messages for program")
		("version,v", "version of program");
	variables_map vm;
	store(command_line_parser(argc, argv).options(desc).run(), vm);
	notify(vm);
	
	if(vm.count("help")){
		cout<<"Usage:"<<endl;
		cout << desc;
		return 0;
	}
	if(vm.count("version")){
		cout<<argv[0]<<"\t1.0"<<endl;
		return 0;
	}
	
	if(vm.count("int")){
		cout<<"int:"<<vm["int"].as<int>()<<endl;
	}else{
		cout<<"int not designate"<<endl;
		return -1;
	}
	
	if(vm.count("str")){
		cout<<"str:"<<vm["str"].as<string>()<<endl;
	}else{
		cout<<"str not designate"<<endl;
		return -1;
	}
	
	vector<int>::iterator it;
	if(vm.count("arr")){
		cout<<"arr:";
		for(it=arr.begin(); it!=arr.end(); it++){
			cout<<*it<<" ";
		}
		cout<<endl;
	}else{
		cout<<"int not designate"<<endl;
		return -1;
	}
	
	return 0;
}
