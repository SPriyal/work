#ifndef FASTCGI
#define FASTCGI

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <sys/un.h>
#include <stddef.h>
#include <string>
#include <map>
#include <sstream>
#include <list>
#include <ext/hash_map>

using namespace __gnu_cxx;
using namespace std;

#define FCGI_VERSION_1  1
#define FCGI_BEGIN_REQUEST 1
#define FCGI_RESPONSE 1
#define FCGI_ABORT_REQUEST 2
#define FCGI_END_REQUEST 3
#define FCGI_PARAMS 4
#define FCGI_STDIN 5
#define FCGI_STDOUT 6
#define FCGI_STDERR 7
#define FCGI_DATA 8
#pragma pack(1)

enum CONNECTD_CMD
{
	CONNECTD_USER_MSG = 1,
	CONNECTD_KEEP_ALIVE = 2,
};

typedef struct 
{
	uint16_t length;
	uint16_t cmd;
	uint16_t requestID;
	
}CONNECTD_HDR;

struct FCGI_AGENT_HDR
{
	uint16_t len;
	uint16_t  cmd;
	uint16_t  ver;
	uint32_t seq;
};

typedef struct 
{
	char version;
	char type;
	unsigned short requestID;
	unsigned short contentLength;
	char paddingLength;
	char reserved;
	
}FCGI_HDR;
#pragma pack()

#define RET_OK 0 
#define RET_FAIL 1
#define RET_SOCK_CLOSE 2

typedef map<string,string>MyMap;

class FastCgiClient{
public:
	int fd;
	unsigned int seq;
	unsigned short recordID;
	string rpcRoot;
	string error;
	string host;
	unsigned short port;
	struct timeval timeout;
	unsigned char open;
	
	int sendFastCgiPack(string & buffer);
	
	void makeBeginRequestBody(unsigned short  role,unsigned char  keepConnection,unsigned char * bin );
	
	void makeHeader(unsigned char type , unsigned short recordID, unsigned short contentLength, unsigned char paddingLength,unsigned char * header);
		
	unsigned short makeNameValueHeader(const string & name, const string & value ,unsigned char * tmpBuf);
	void addAgentHeader(char * header , unsigned short length,unsigned short cmd,unsigned int seq) ;
	int sendFastCgiPack(const char * buf, int len);
	int makeFgiPack(const string & method, char * buf, const MyMap & args);
public:
	FastCgiClient():fd(0),recordID(1),seq(1),rpcRoot("/opt/app/rpcserver/rpcserver.php"),host("127.0.0.1"),port(0){
		timeout.tv_sec = 0;
		timeout.tv_usec = 500000;
	}
	
	void closeConnection(){
		if(fd > 0){
			open = 0;
			close(fd);
			fd = 0;
		}
	}
	
	unsigned char isOpen(){
		return open;	
	}
	
	~FastCgiClient(){
		if(fd > 0){
			open = 0;
			close(fd);	
			fd = 0;
		}
	}
	
	void displayTimeout()
	{
		cout<<"timeout tv_sec = "<<timeout.tv_sec<<endl;
		cout<<"timeout tv_usec= "<<timeout.tv_usec<<endl;
	}

	string & getLastError(){
		return error;	
	}
 	void setRPCRoot(const string & root){
 		this->rpcRoot = root;	
 	}
	void setServer(const string & host , const unsigned short port){
		this->host = host;
		this->port = port;	
	}
	
	int setTimeout(const long timeout_sec = 0, const long timeout_usec = 5000000){
		this->timeout.tv_sec = timeout_sec;
		this->timeout.tv_usec = timeout_usec;	
	}
	int sendAliveMsg();
	int recvData(int recvLen,char * buffer);
	int recvAgentHeader(FCGI_AGENT_HDR * hdr);
	int recvFcgiHeader(FCGI_HDR * hdr);
	int createSocket();
	int connectServer();
	int sendToFpm(const string & method,  const map<string,string> & args);
	int sendToAgent(const string & method,  const map<string,string> & args);
//	int recvData(string & recvBuffer);

	int recvPackData(string & content);
public:
	unsigned short local_port;
};

#endif

