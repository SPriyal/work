#include "../header/header.h"
#include <map>
#include <queue>
char * buf;
int fd;
unsigned int recordid = DATA_RECORD_ID;

class Request{
public:
	uint32_t length;
	uint32_t recordid;
	char buf[LENGTH];
};

class Response{
public:
	uint32_t length;
	uint32_t recordid;
	char buf[LENGTH];
};

class Client{
	typedef map<uint32_t,pthread_cond_t* > RequestMap;
	typedef queue<Request*> RequestQueue;
	typedef map<uint32_t, Response * >ResponseMap;
private:
	static uint8_t stop;
	static ResponseMap responseMap;
	static RequestQueue requestQueue;
	static RequestMap requestMap;
	static pthread_mutex_t requestLock;
	static pthread_mutex_t responseLock;
	static int fd;		
	static int recordid;
public:
	Client(){}
	static void destroy();
	static int initialize();
	static Response *  request(const char * buf ,int length);
	static void* sendThread(void * argv);
	static void* recvThread(void * argv);
	static int recvData(char * buf, int length);
};

pthread_mutex_t Client::requestLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Client::responseLock = PTHREAD_MUTEX_INITIALIZER;
int Client::fd = 0;
int Client::recordid = DATA_RECORD_ID;
uint8_t Client::stop = 0;
Client::ResponseMap Client::responseMap;
Client::RequestQueue Client::requestQueue;
Client::RequestMap Client::requestMap;

int Client::initialize(){
	struct sockaddr * caddr = NULL;
	int len = 0;
#ifndef AFUNIX
 	fd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	caddr = (struct sockaddr *)&addr;
	len = sizeof(struct sockaddr_in);
#else
	fd = socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un unix_addr;
	unix_addr.sun_family = AF_UNIX;
	strcpy(unix_addr.sun_path , unix_socket);
	len = offsetof(struct sockaddr_un,sun_path) + strlen(unix_socket);
	caddr = (struct sockaddr *)&unix_addr;
#endif

	if(fd == -1){
		sock_err("socket error",fd);	
	}
	
	if(connect(fd,caddr,len) == -1){
		sock_err("connect error",fd);
	}
	
	pthread_t threadid;
	int ret = 0 ;
	ret = pthread_create(&threadid,NULL,&Client::sendThread,NULL);
	if(ret == -1){
		sock_err("create thread error",fd);
	}
	ret = pthread_create(&threadid,NULL,&Client::recvThread,NULL);
	if(ret == -1){
		sock_err("create thread error",fd);
	}	
}

Response *  Client::request(const char * buf, int length)
{
	Request * req = new Request();
	req->length = length;
	req->recordid = recordid++;
	memcpy(req->buf,(void*)&length,sizeof(uint32_t));
	memcpy(req->buf + sizeof(uint32_t),(void*)&req->recordid,sizeof(uint32_t));
	memcpy(req->buf + sizeof(header),buf,length);
	pthread_cond_t pthread_cond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_lock(&requestLock);
	requestMap.insert(make_pair(req->recordid,&pthread_cond));
	requestQueue.push(req);
	pthread_cond_wait(&pthread_cond,&requestLock);
	pthread_mutex_unlock(&requestLock);
cerr<<"after cond wait"<<endl;
	pthread_mutex_lock(&responseLock);
	ResponseMap::iterator it = responseMap.find(req->recordid);
	if(it == responseMap.end()){
		return NULL;
	}
	Response * resp = it->second;
	responseMap.erase(it);
	pthread_mutex_unlock(&responseLock);
	return resp;
}

int Client::recvData(char * buf,int length){
	int recvSize = recv(fd,buf,length,MSG_WAITALL);
	if(recvSize == -1){
		destroy();
		return 0;
	}
	else if(recvSize == 0){
		destroy();
		return 0;
	}else if(recvSize > 0){
		return recvSize;
	}
}

void * Client::sendThread(void * argv){
	char buf[LENGTH];
	while(stop == 0){
		pthread_mutex_lock(&requestLock);	
		Request * req  = NULL;
		if(requestQueue.size()>0){
			req = requestQueue.front();
			requestQueue.pop();
		}
		pthread_mutex_unlock(&requestLock);
		if(req == NULL){
			continue;	
		}
		send(fd,req->buf,req->length+sizeof(header),0);
		sleep(1);
	}
}

void * Client::recvThread(void * argv){
	char buf[LENGTH];
	Response * resp =NULL;
	while(stop == 0){
		int recvSize = recvData(buf,sizeof(header));
		if(recvSize > 0){
				header hdr ;
				getHeader(buf,&hdr);
				if(hdr.recordid == PLUSE_RECORD_ID){
					send(fd,buf,recvSize,0);
					continue;
				}
				if(hdr.length > 0){
					resp = new Response();
					recvSize = recv(fd,buf,hdr.length,0);
					resp->length = hdr.length;
					resp->recordid = hdr.recordid;
	cerr<<"resp recordid is"<<resp->recordid<<endl;
					memcpy(resp->buf,buf,hdr.length);
					pthread_mutex_lock(&responseLock);
					responseMap.insert(make_pair(resp->recordid,resp));
					pthread_mutex_unlock(&responseLock);
					
					pthread_mutex_lock(&requestLock);
					RequestMap::iterator it= requestMap.find(resp->recordid);
					if(it == requestMap.end())continue;
					pthread_cond_t * cond = it->second;
					requestMap.erase(it);
					pthread_mutex_unlock(&requestLock);
					int err = 0 ;
					if((err = pthread_cond_signal(cond)!=0)){
						printf("signal error %s\n",strerror(err));	
					}
		cerr<<"signal"<<endl;
				}
		}
	}
}

void Client::destroy(){
	cerr<<"destroy"<<endl;
	if(fd > 0){
		close(fd);	
	}
	stop = 1;
	pthread_mutex_lock(&requestLock);
	Request * req = NULL;
	while(requestQueue.size()>0){
		req = requestQueue.front();
		requestQueue.pop();
		delete  req;
	}
	pthread_mutex_unlock(&requestLock);

	pthread_mutex_lock(&responseLock);
	Response * response = NULL;
	for(ResponseMap::iterator it = responseMap.begin();it!=responseMap.end();){
			response = it->second;
			delete response;
			responseMap.erase(it++);
	}
	pthread_mutex_unlock(&responseLock);
}


void tcp_client()
{
	int retContSize = 0 ;
	int sendSize = 0;
	int recvSize = 0;
	buf = (char*)malloc(LENGTH);
	if(buf == NULL)exit(-1);
	char retBuf[LENGTH];
	struct sockaddr * caddr = NULL;
	int len = 0;
#ifndef AFUNIX
 	fd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	caddr = (struct sockaddr *)&addr;
	len = sizeof(struct sockaddr_in);
#else
	fd = socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un unix_addr;
	unix_addr.sun_family = AF_UNIX;
	strcpy(unix_addr.sun_path , unix_socket);
	len = offsetof(struct sockaddr_un,sun_path) + strlen(unix_socket);
	caddr = (struct sockaddr *)&unix_addr;
#endif

	if(fd == -1){
		sock_err("socket error",fd);	
	}
	
	if(connect(fd,caddr,len) == -1){
		sock_err("connect error",fd);
	}
	
	string content  = "BEGIN SEND";
	makeBuf(&buf,content,BEGIN_RECORD_ID);
	header * phd = (header*)buf;
	sendSize = send(fd,buf,sizeof(header)+phd->length,0);
	debug("send size is %d\n",sendSize);
	sleep(1);
	content = "send record ";
	makeBuf(&buf,content,recordid++);
	sendSize = send(fd,buf,sizeof(header)+phd->length,0);
		debug("send size is %d\n",sendSize);
	sleep(1);
	content = "send record 2";
	makeBuf(&buf,content,recordid++);
	sendSize = send(fd,buf,sizeof(header)+phd->length,0);
		debug("send size is %d\n",sendSize);
	sleep(1);
	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	if(setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval)) == -1){
		perror("set opt error");
		close(fd);
		exit(0);	
	}
	while(1){
		int recvSize = recv(fd,buf,sizeof(header),MSG_WAITALL);
		debug("recvSize = %d\n",recvSize);
		if(recvSize == -1){
			if(errno == EAGAIN){
				perror("in the process");
				sleep(1);
				continue;
			}
			debug("connect close\n");
			close(fd);
			exit(0);
		}else if(recvSize == 0){
			debug("connection close\n");
			close(fd);
			exit(0);
		}
		debug("recv size is %d\n",recvSize);
		header hdr;
		getHeader(buf,&hdr);
		debug("record id is %d\n",hdr.recordid);
		debug("record length is %d\n",hdr.length);
		if(hdr.recordid==END_RECORD_ID){
			//close(fd);
			//exit(0);	
		}else if(hdr.recordid == PLUSE_RECORD_ID){
			debug("recv pluse record \n");
			send(fd,buf,sizeof(header),0);	
			continue;
		}
		recvSize = recv(fd,buf,hdr.length,MSG_WAITALL);
		debug("ret buf is %s\n",buf);
	}
}

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t  cond_mutex = PTHREAD_MUTEX_INITIALIZER;
queue<pthread_cond_t * >condQueue;
void * test1(void * argv){
	pthread_mutex_lock(&cond_mutex);
	condQueue.push(&cond);
	cerr<<"before wait"<<endl;
	pthread_cond_wait(&cond,&cond_mutex);
	cerr<<"after wait"<<endl;
	pthread_mutex_unlock(&cond_mutex);
}

void * test2(void * argv){
	pthread_mutex_lock(&cond_mutex);
	pthread_cond_t *local_cond;
	if(condQueue.size()>0){
		local_cond = condQueue.front();
		condQueue.pop();	
	}
	pthread_cond_signal(local_cond);
	pthread_mutex_unlock(&cond_mutex);
}


void * requestThread(void * argv){
	cerr<<"thread id is "<<pthread_self()<<endl;
	Response * resp = Client::request("hello world\n",sizeof("hello world\n"));
	if(resp != NULL){
		cout<<"resp buf = "<<resp->buf<<endl;	
	}		
}

void longConnect(){
	Client::initialize();
	int i = 0;
	pthread_t threadid;
	int ret = 0;
	for(;i<1;i++){
		ret = pthread_create(&threadid,NULL,requestThread,NULL);
		if(ret != 0){
				printf("create thread error : %s\n",strerror(ret));
		}
	}	
	sleep(1);	
}

int main()
{	

	/*pthread_t id;
	pthread_create(&id,NULL,test1,NULL);
	usleep(1);
	pthread_create(&id,NULL,test2,NULL);
	sleep(1);*/
	tcp_client();
	char c;
	while(cin>>c){
		if(c == 's'){
			Client::destroy();
			break;
		}	
	}
	
	return 0;
}

