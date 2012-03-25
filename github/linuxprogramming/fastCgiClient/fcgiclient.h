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

typedef struct 
{
	char version;
	char type;
	unsigned short requestID;
	unsigned short contentLength;
	char paddingLength;
	char reserved;
	
}FCGI_HDR;

#define RET_OK 0 
#define RET_FAIL 1
#define RET_SOCK_CLOSE 2

typedef map<string,string>MyMap;

class FastCgiClient{
private:
	int fd;
	unsigned short requestID;
	string rpcRoot;
	string error;
	string host;
	unsigned short port;
	struct timeval timeout;
	
	int sendFastCgiPack(string & buffer);
	
	void makeBeginRequestBody(unsigned short  role,unsigned char  keepConnection,unsigned char * bin );
	
	void makeHeader(unsigned char type , unsigned short requestID, unsigned short contentLength, unsigned char paddingLength,unsigned char * header);
		
	unsigned short makeNameValueHeader(const string & name, const string & value ,unsigned char * tmpBuf);
	void addAgentHeader(string & buffer,unsigned short cmd,unsigned short requestID) ;

	void makeFgiPack(const string & method, string & buffer , const MyMap & args);
public:
	FastCgiClient():fd(0),requestID(1),rpcRoot("/opt/app/rpcserver/rpcserver.php"),host("127.0.0.1"),port(0){
		timeout.tv_sec = 0;
		timeout.tv_usec = 50000;
	}
	
	void closeConnection(){
		if(fd > 0){
			close(fd);
		}
	}
	
	~FastCgiClient(){
		if(fd > 0){
			close(fd);	
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
	
	int setTimeout(const long timeout_sec = 0, const long timeout_usec = 500000){
		this->timeout.tv_sec = timeout_sec;
		this->timeout.tv_usec = timeout_usec;	
	}
	int sendAliveMsg();
	int recvData(int recvLen,char * buffer);
	int recvAgentHeader(CONNECTD_HDR * hdr);
	int recvFcgiHeader(FCGI_HDR * hdr);
	int createSocket();
	int connectServer();
	int sendToFpm(const string & method,  const map<string,string> & args);
	int sendToAgent(const string & method,  const map<string,string> & args);
	int recvData(string & recvBuffer);
};

#endif

