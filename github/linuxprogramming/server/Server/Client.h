#ifndef CLIENT
#define CLIENT
#define LENGTH 1024
class Client{
private:
	int client_sock;
	bool isAlive;
	bool isOver;
	bool isConnected;
	header hdr;
	char buf[LENGTH];
	struct sockaddr_in client_addr;
public:
	Client(){}
	Client(const int client_sock,const struct sockaddr_in client_addr){
		this->client_sock = client_sock;
		this->client_addr = client_addr;
		this->isOver = false;
		this->isConnected = true;
		this->isAlive = true;
	}
	
	bool getIsAlive()
	{
		return isAlive;
	}
	
	void setIsAlive(bool isAlive){
		this->isAlive = isAlive;	
	}
	
	bool getIsOver(){
		return isOver;
	}
	
	bool getIsConnected(){
		return isConnected;	
	}
	
	void setIsConnected(bool isConnected){
		this->isConnected = isConnected;	
	}
	
	~Client(){
		close(client_sock);
	}
	
	
	void showAddress(){
		char abuf[INET_ADDRSTRLEN];
		printf("address is %s, port is %u",inet_ntop(AF_INET,&this->client_addr.sin_addr,abuf,INET_ADDRSTRLEN),ntohs(this->client_addr.sin_port));
	}
	void startRunning();
	static void * recvThread(void * argv);
	int recvData(char * buf,int length);
	void sendPluse();
};

#endif

