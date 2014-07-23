#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <pthread.h>

#include <sofa/pbrpc/pbrpc.h>
#include "echo_service.pb.h"

using namespace boost::property_tree;
using namespace std;

//全局函数声明
//获取测试数据列表
vector<string> get_testdata(string filepath);
//随机获取测试数据
string get_randomdata(vector<string> testdata);
//定义回调函数
void EchoCallback(sofa::pbrpc::RpcController* cntl, sofa::pbrpc::test::EchoRequest* request, sofa::pbrpc::test::EchoResponse* response, 
				sofa::pbrpc::test::EchoServer_Stub *stub, bool* params);
//定义数据请求函数
void EchoAsynCall(sofa::pbrpc::test::EchoServer_Stub *stub, vector<string> &testdata, bool* params);
//获取当前时间函数，返回单位ms
long getCurrentTime();

//全局变量声明
pthread_mutex_t mutex;
static int total_req_cnt=0;
static int total_res_cnt=0;
static int total_err_cnt=0;


long getCurrentTime(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

vector<string> get_testdata(string filepath){
	vector<string> testdata;
	try{
		ptree pt,pt1;
		read_json(filepath, pt);
		pt1 = pt.get_child("data");
		for(ptree::iterator it = pt1.begin(); it != pt1.end(); ++it){
			assert((*it).first.empty()); // array elements have no names
			testdata.push_back((*it).second.data());
		}
	}
	catch(exception const& e){
		cout<<e.what()<<endl;
	}
	
	return testdata;
}

string get_randomdata(vector<string> testdata){
	srand((unsigned)time(NULL));
	random_shuffle(testdata.begin(),testdata.end());
	return (*testdata.begin());
}

void EchoCallback(sofa::pbrpc::RpcController* cntl, sofa::pbrpc::test::EchoRequest* request, sofa::pbrpc::test::EchoResponse* response, 
				sofa::pbrpc::test::EchoServer_Stub *stub, vector<string> &testdata, bool* params){
	//SLOG(NOTICE, "RemoteAddress=%s", cntl->RemoteAddress().c_str());
	//SLOG(NOTICE, "IsRequestSent=%s", cntl->IsRequestSent() ? "true" : "false");
	/*int current = getCurrentTime();
	if(current >= end){
		*params = true;
	}*/
	
	if (cntl->IsRequestSent())
	{
		//SLOG(NOTICE, "LocalAddress=%s", cntl->LocalAddress().c_str());
		//SLOG(NOTICE, "SentBytes=%ld", cntl->SentBytes());
	}

	if (cntl->Failed()) {
		//SLOG(ERROR, "request failed: %s", cntl->ErrorText().c_str());
		pthread_mutex_lock(&mutex);
		total_err_cnt++;
		pthread_mutex_unlock(&mutex);
	}
	else {
		pthread_mutex_lock(&mutex);
		total_res_cnt++;
		pthread_mutex_unlock(&mutex);
		
		//SLOG(NOTICE, "request succeed: %s", response->message().c_str());
		EchoAsynCall(stub, testdata, params);
	}

	delete cntl;
	delete request;
	delete response;
	//*params = true;
}

void EchoAsynCall(sofa::pbrpc::test::EchoServer_Stub *stub, vector<string> &testdata, bool* params){
	string randomdata=get_randomdata(testdata);
	
	sofa::pbrpc::test::EchoRequest* request = new sofa::pbrpc::test::EchoRequest();
	request->set_message(randomdata);
	sofa::pbrpc::test::EchoResponse* response = new sofa::pbrpc::test::EchoResponse();

	// 创建RpcController
	sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
	cntl->SetTimeout(3000);

	// 创建回调函数，回调函数可以预绑定一些参数，譬如callbacked
	google::protobuf::Closure* done = sofa::pbrpc::NewClosure(
			&EchoCallback, cntl, request, response, stub, testdata, params);
	
	pthread_mutex_lock(&mutex);
	total_req_cnt++;
	pthread_mutex_unlock(&mutex);
	// 发起调用，最后一个参数非NULL表示为异步调用
	stub->Echo(cntl, request, response, done);
}

int main(int argc, char* argv){
	if(pthread_mutex_init(&mutex,NULL)!=0){
		return -1;
	}
	
	string filepath="test.data";
	vector<string> testdata=get_testdata(filepath);
	
	vector<string>::iterator it;
	for(it=testdata.begin(); it!=testdata.end(); it++){
		cout<<*it<<endl;
	}
	
	string randomdata=get_randomdata(testdata);
	cout<<"randomdata:"<<randomdata<<endl;
	
	
	
	SOFA_PBRPC_SET_LOG_LEVEL(NOTICE);

	// 定义RpcClient
	sofa::pbrpc::RpcClientOptions client_options;
	sofa::pbrpc::RpcClient *rpc_client = new sofa::pbrpc::RpcClient(client_options);

	// 定义RpcChannel
	sofa::pbrpc::RpcChannel *rpc_channel = new sofa::pbrpc::RpcChannel(rpc_client, "10.48.55.39:12321");

	// 定义EchoServer服务的桩对象
	sofa::pbrpc::test::EchoServer_Stub *stub = new sofa::pbrpc::test::EchoServer_Stub(rpc_channel);

	/*// 创建Request和Response消息
	sofa::pbrpc::test::EchoRequest* request = new sofa::pbrpc::test::EchoRequest();
	request->set_message("Hello from qinzuoyan01");
	sofa::pbrpc::test::EchoResponse* response = new sofa::pbrpc::test::EchoResponse();

	// 创建RpcController
	sofa::pbrpc::RpcController* cntl = new sofa::pbrpc::RpcController();
	cntl->SetTimeout(3000);

	// 创建回调函数，回调函数可以预绑定一些参数，譬如callbacked
	bool callbacked = false;
	google::protobuf::Closure* done = sofa::pbrpc::NewClosure(
			&EchoCallback, cntl, request, response, &callbacked);

	// 发起调用，最后一个参数非NULL表示为异步调用
	stub.Echo(cntl, request, response, done);*/
	int start = getCurrentTime();
	int test_time = 1*60*1000;
	int end = start + test_time;
	
	bool params = false;
	for(int i=0; i<200; i++){
		EchoAsynCall(stub, testdata, &params);
	}
	
	// 等待回调完成，此处为简单的变量探测方式，不推荐
	/*while (!params) {
		usleep(100000);
	}*/
	int current = getCurrentTime();
	while(current < end){
		usleep(1000000);
		current = getCurrentTime();
	}
	
	//避免出core，屏蔽
	//delete stub;
	//delete rpc_channel;
	//delete rpc_client;
	
	cout<<"test_time: "<<test_time<<" ms"<<endl;
	cout<<"total_req_cnt: "<<total_req_cnt<<endl;
	cout<<"total_res_cnt: "<<total_res_cnt<<endl;
	cout<<"total_err_cnt: "<<total_err_cnt<<endl;
	cout<<"query per second: "<<((double)total_res_cnt/(double)test_time)*1000<<endl;
	
	return 0;
}
