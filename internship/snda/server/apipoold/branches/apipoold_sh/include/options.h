
#ifndef _OPTIONS_H_
#define _OPTIONS_H_


#include <string>
#include <vector>

using namespace std;



struct IpPort
{
	string ip;
	int port;
};


// �����������
struct AdminServer
{
	string admin_ip;
	int admin_port;
	int reconnect_interval;
};




// ֪ͨ��������
struct NotifyServer
{
	vector<IpPort> notify_list;
	int connect_timeout;
	int reconnect_interval;
	int keepalive_interval;
};



// apipoold��������
struct Apipoold
{
	string apipoold_longtcp_ip;
	int apipoold_longtcp_port;
	int longtcp_timeout;

	string apipoold_shorttcp_ip;
	int apipoold_shorttcp_port;
	int shorttcp_timeout;
	string rpcRoot;


	uint32_t shm_key;

};

// Webagent��������
struct Webagent
{
	string webagent_ip;
	int webagent_port;
};



// ϵͳ����
struct System
{
	string log_level;
	bool log_enable_pack_print;
};

struct ServcieInfo
{
	string serviceName;
	string ip;
	int port;
	int index;		
};

struct PhpService
{
	vector<ServcieInfo>	service_list;	
};

struct ProtobufService
{
	vector<ServcieInfo>	service_list;	
};



//! ���ӷ�������������
struct Options
{
	// �����������
	static AdminServer admin_server;

	// apipoold��������
	static Apipoold apipoold;
	
	// ϵͳ����
	static System system;

	static PhpService phpservice;

	static ProtobufService stateServer;
};


#endif // _OPTIONS_H_
