#include <sys/time.h>
#include "fcgiclient.h"
#include "tokenMessage.pb.h"
static MyMap defMap;
stringstream ss;
typedef hash_map<unsigned int,FCGI_AGENT_HDR*> HashMap;
HashMap hdrHashMap;
int totallength;
char totalbuf[1024];
static FastCgiClient fcc;
const size_t AGENT_HDR_SIZE = sizeof(FCGI_AGENT_HDR);

int FastCgiClient::sendFastCgiPack(const char * buf, int len){
	if(open == 1){
		int sendSize = 0;
		sendSize = send(fd,buf,len,0);
//cout<<"sendSize= "<<sendSize<<endl;
		if(sendSize == - 1){
			error = string(strerror(errno));
			closeConnection();
			return RET_FAIL;
		}
		++seq;
		return RET_OK;
	}
	else {
		return RET_FAIL;	
	}
}

void FastCgiClient::makeBeginRequestBody(unsigned short  role,unsigned char  keepConnection,unsigned char * bin )
{
	role = htons(role);
	memcpy(bin,(void*)&role,sizeof(unsigned short));
	bin[2] = keepConnection;
	for(int i = 3;i < 8;++i){
			bin[i] = 0;	
	}		
}
	
void FastCgiClient::makeHeader(unsigned char type , unsigned short requestID, unsigned short contentLength, unsigned char paddingLength,unsigned char * header)
{
	header[0] = FCGI_VERSION_1;
	header[1] = type;
	requestID = htons(requestID);
	contentLength = htons(contentLength);
	memcpy(header+2,(void*)&requestID,sizeof(unsigned short));
	memcpy(header+4,(void*)&contentLength,sizeof(unsigned short));
	header[6] = paddingLength;
	header[7] = 0;
}
		
unsigned short FastCgiClient::makeNameValueHeader(const string & name, const string & value ,unsigned char * tmpBuf)
{
	int nameLength = name.length();
	int valueLength = value.length();
	
	int index = 0;
	if(nameLength < 0x80)
	{
		tmpBuf[index++] = nameLength;
	}else{
		tmpBuf[index++] = ((valueLength >> 24) & 0x7f | 0x80);
		tmpBuf[index++] = ((valueLength >> 16) & 0xff);
		tmpBuf[index++] = ((valueLength >> 8) & 0xff);
		tmpBuf[index++] = (valueLength & 0xff);
	}

	if(valueLength < 0x80)
	{
		tmpBuf[index++] = valueLength; 
	}else{
		tmpBuf[index++] = ((valueLength >> 24) & 0x7f | 0x80);
		tmpBuf[index++] = ((valueLength >> 16) & 0xff);
		tmpBuf[index++] = ((valueLength >> 8) & 0xff);
		tmpBuf[index++] = (valueLength & 0xff);
	}

	memcpy(tmpBuf + index, name.c_str(),name.length());
	index += name.length();
	memcpy(tmpBuf + index, value.c_str(),value.length());
	index += value.length();
	return (unsigned short)index;
}	
	
void FastCgiClient::addAgentHeader(char * header,unsigned short length,unsigned short cmd,unsigned int seq)
{	
	FCGI_AGENT_HDR * request_hdr = (FCGI_AGENT_HDR * )malloc(sizeof(FCGI_AGENT_HDR));
	request_hdr->len =  length +sizeof(FCGI_AGENT_HDR) ;
	request_hdr->cmd =  cmd;
	request_hdr->seq = seq;
	request_hdr->ver =1;
	//hdrHashMap.insert(HashMap::value_type(seq,request_hdr));
		
	length  = htons(request_hdr->len);
	cmd = htons(request_hdr->cmd);
	seq = htonl(request_hdr->seq);
	unsigned short ver = htons(request_hdr->ver);
	
	memcpy(header,(void*)&length,2);
	memcpy(header+2,(void*)&cmd,2);
	memcpy(header+4,(void*)&ver,2);
	memcpy(header+6,(void*)&seq,4);
}
	

int FastCgiClient::makeFgiPack(const string & method, char * buf , const MyMap & args )
{
	int offset = 0;
	unsigned char body[8];
	unsigned short bodyLength =  0 ;
	unsigned char header[8];
	makeBeginRequestBody(FCGI_RESPONSE,0,body);
	makeHeader(FCGI_BEGIN_REQUEST, recordID, sizeof(body), 0,header);
	memcpy(buf+offset,header,8);
	offset+=8;
	memcpy(buf+offset,body,8);
	offset +=8;
	
	//file name and method name
	MyMap ar_pack;
	ar_pack["SCRIPT_FILENAME"] = rpcRoot;
	ar_pack["FRPC_METHOD"] = method;
	unsigned char nameValueBody[9+rpcRoot.length()+ method.length()];
	for(map<string,string>::iterator it = ar_pack.begin();it!=ar_pack.end();++it){
			bodyLength = makeNameValueHeader(it->first,it->second,nameValueBody);
			makeHeader(FCGI_PARAMS,recordID,bodyLength,0,header);
			memcpy(buf+offset,header,8);
			offset +=8;
			memcpy(buf+offset,nameValueBody,bodyLength);
			offset +=bodyLength;
	}
	
 	 if(args.size() > 0){
 	 	for(map<string,string>::const_iterator cit = args.begin();cit!= args.end();++cit){
        		bodyLength = makeNameValueHeader("FRPC_ARGS_"+cit->first , cit->second,nameValueBody);
      	 	makeHeader(FCGI_PARAMS,recordID, bodyLength, 0,header);
      	 	memcpy(buf+offset,header,8);
			offset +=8;
			memcpy(buf+offset,nameValueBody,bodyLength);
			offset +=bodyLength;
      	}
  	}
  	
  	//end of fastcgi protocol
    	makeHeader(FCGI_STDIN, recordID,0, 0,header);
    	memcpy(buf+offset,header,8);
	offset +=8;
	return offset;
}

int FastCgiClient::createSocket(){
	struct sockaddr * addr = NULL;
	int len  = 0;
	char * unixSockPath = "/tmp/php-fpm.sock";
#ifdef UNIX_SOCK
	struct sockaddr_un fpm_unix_addr;
	memset(&fpm_unix_addr,0,sizeof(struct sockaddr_un));
	fpm_unix_addr.sun_family = AF_UNIX;
	strcpy(fpm_unix_addr.sun_path , unixSockPath);
	fd = socket(AF_UNIX,SOCK_STREAM,0);
	len = offsetof(struct sockaddr_un,sun_path) + strlen(unixSockPath);
	addr = (struct sockaddr * )&fpm_unix_addr;
#else
	struct sockaddr_in iaddr;
	iaddr.sin_family = AF_INET;
	iaddr.sin_port = htons(port);
	iaddr.sin_addr.s_addr = inet_addr(host.c_str());
	fd = socket(AF_INET,SOCK_STREAM,0);
	len = sizeof(struct sockaddr_in);
	addr = (struct sockaddr *)&iaddr;
#endif

	if(fd == -1){
		return RET_FAIL;
	}
	if(connect(fd,addr,len) == -1){
		error = string(strerror(errno));
		closeConnection();
		return RET_FAIL;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)) == -1){
		close(fd);
		return RET_FAIL;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeout)) == -1){
		close(fd);
		return RET_FAIL;
	}
	int l = 0;
	if(getsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,(socklen_t *)&l) ==-1){
		close(fd);
		return RET_FAIL;
	}
	open = 1;
}
/*
int FastCgiClient::createSocket(){
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd == -1){
		this->error = string(strerror(errno));
		return RET_FAIL;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval)) == -1){
		error = string(strerror(errno));
		closeConnection();
		return RET_FAIL;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval)) == -1){
		error = string(strerror(errno));
		closeConnection();
		return RET_FAIL;
	}
	return RET_OK;
}
	*/
int FastCgiClient::connectServer(){
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host.c_str());
	if(connect(fd,(struct sockaddr * )&addr,sizeof(struct sockaddr)) == -1){
		error = string(strerror(errno));
		closeConnection();
		return RET_FAIL;
	}
	open = 1;
	struct sockaddr_in local_sock;
	socklen_t len = sizeof(sockaddr);
	getsockname(fd, (struct sockaddr*)&local_sock, &len);
	local_port = local_sock.sin_port;
	//cout<<"connect ok port:"<<local_sock.sin_port<<" pid:"<<getpid();
	return RET_OK;
}
	
int FastCgiClient::sendToFpm(const string & method,  const map<string,string> & args = defMap)
{
	char buf[1024];
	unsigned short length = makeFgiPack(method,buf,args);
	return sendFastCgiPack(buf,length);
}
	
int FastCgiClient::sendToAgent(const string & method,  const map<string,string> & args = defMap)
{
	char buf[1024];
	unsigned short length = makeFgiPack(method,buf+AGENT_HDR_SIZE,args);
	addAgentHeader(buf,length,CONNECTD_USER_MSG,seq);
	return sendFastCgiPack(buf,length+AGENT_HDR_SIZE);
}

int FastCgiClient::sendAliveMsg(){
	char buf[AGENT_HDR_SIZE];
	addAgentHeader(buf,AGENT_HDR_SIZE,CONNECTD_KEEP_ALIVE,seq);
	return sendFastCgiPack(buf,AGENT_HDR_SIZE);
}

int FastCgiClient::recvData(int recvLen,char * buffer){
	int tmp = 0;
	int recved = 0;
	while(true){
		tmp = recv(fd,buffer+recved,recvLen-recved, 0);
		if(tmp == -1)
		{
			if(errno == EAGAIN){
				cerr<<"timeout"<<strerror(errno)<<endl;
			}
			cerr<<"fd num"<<fd<<"recv fail"<<" errno:"<<errno<<endl;
			return RET_FAIL;
		}
		else if(tmp == 0){
			return RET_OK;
		}
		else if(tmp > 0)
		{
			recved += tmp;
			if(recved >= recvLen){
				return recved;	
			}
		}
	}
}

int FastCgiClient::recvAgentHeader(FCGI_AGENT_HDR * hdr){
	char buf[AGENT_HDR_SIZE];
	int recvSize = recvData(AGENT_HDR_SIZE,buf);
	if(recvSize !=AGENT_HDR_SIZE)return recvSize;
	FCGI_AGENT_HDR * phdr = (FCGI_AGENT_HDR*)buf;
	hdr->len = ntohs(phdr->len);
	hdr->cmd = ntohs(phdr->cmd);
	hdr->ver = ntohs(phdr->ver);
	hdr->seq = ntohl(phdr->seq);
	/*HashMap::iterator it = hdrHashMap.find(hdr->seq);
	if(it != hdrHashMap.end()){
		phdr  = it->second;
		delete phdr;
		hdrHashMap.erase(it);
	}*/
	return sizeof(buf);
}

int FastCgiClient::recvFcgiHeader(FCGI_HDR * hdr){
	char buf[8];
	int recvSize = recvData(8,buf);
	if(recvSize != 8)return recvSize;
	FCGI_HDR * phdr = (FCGI_HDR*)buf;
	hdr->version = phdr->version;
	hdr->type = phdr->type;
	hdr->version = ntohs(phdr->requestID);
	hdr->contentLength = ntohs(phdr->contentLength);
	hdr->paddingLength = phdr->paddingLength;
	hdr->reserved = phdr->reserved;
	return sizeof(buf);
}

int FastCgiClient::recvPackData(string & content){
	FCGI_AGENT_HDR hdr;
	FCGI_HDR fcgi_hdr;
	char buf[8012];
	int tmp = 0;
	int ret = RET_OK;
	int recvLength = 0;
	ret = recvAgentHeader(&hdr);
	if(ret == RET_FAIL){
		closeConnection();
		return RET_FAIL;
	}
	recvLength += ret;
	if(recvLength >= hdr.len){
		return RET_OK;	
	}
	while(true){
		ret  = recvFcgiHeader(&fcgi_hdr);
		if(ret != sizeof(FCGI_HDR)){
			closeConnection();
			return ret;
		}

		recvLength += ret;

		if(recvLength >= hdr.len){
			return RET_OK;	
		}

		
		ret  = recvData(fcgi_hdr.contentLength,buf);
		if(ret != fcgi_hdr.contentLength){
			closeConnection();
			return ret;	
		}
		recvLength +=ret;
		
		if(recvLength >= hdr.len){
			return RET_OK;	
		}

		
		if(fcgi_hdr.paddingLength > 0){
			char paddingBuf[fcgi_hdr.paddingLength];
			int padding = recvData(fcgi_hdr.paddingLength,paddingBuf);
			if( padding != sizeof(paddingBuf)){
				closeConnection();
				return  padding;	
			}	
			recvLength +=  padding;
		}
		if(fcgi_hdr.type == FCGI_STDOUT 	||	fcgi_hdr.type == FCGI_STDERR) {
			content += string(buf,ret);
		}
			
		if(recvLength >= hdr.len){
			return RET_OK;	
		}
	}
}
/*
//for fpm version;
int FastCgiClient::recvPackData(string & content){
	FCGI_AGENT_HDR hdr;
	FCGI_HDR fcgi_hdr;
	char buf[8012];
	int tmp = 0;
	int ret = RET_OK;
	int recvLength = 0;
	int length = 0;
	while(true){
		ret  = recvFcgiHeader(&fcgi_hdr);
		if(ret != sizeof(FCGI_HDR)){
			closeConnection();
			return ret;
		}

		recvLength += ret;

		if(recvLength >= hdr.len){
			return RET_OK;	
		}

		length = fcgi_hdr.contentLength + fcgi_hdr.paddingLength;
		
		ret  = recvData(fcgi_hdr.contentLength,buf);
		if(ret != fcgi_hdr.contentLength){
			closeConnection();
			return ret;	
		}
		recvLength +=ret;
		if(recvLength >= hdr.len){
			return RET_OK;	
		}
		if(fcgi_hdr.paddingLength > 0){
			char paddingBuf[fcgi_hdr.paddingLength];
			int padding = recvData(fcgi_hdr.paddingLength,paddingBuf);
			if( padding != sizeof(paddingBuf)){
				closeConnection();
				return  padding;	
			}	
			recvLength +=  padding;
            if(recvLength > hdr.len)return padding;
		}
		if(fcgi_hdr.type == FCGI_STDOUT 	||	fcgi_hdr.type == FCGI_STDERR) {
			content += string(buf,ret);
		}else{
            return RET_OK;
        }
			
		if(recvLength >= hdr.len){
			return RET_OK;	
		}
//		return RET_OK;
	}
}
*/

string getresult(const string & content){
	string::size_type pos = content.find("\r\n\r\n",0);
	if(string::npos == pos) {
		cerr<<"invalid protocol"<<endl;
  		return "";	
	}
	pos += 4;

	if(pos == content.length()) {
		return "";
	}else{
		return content.substr(pos);
	}	
}

static int analyzeResult(string & result,string & info){
		string::size_type pos = result.find("success",0);
		if(pos != string::npos){
			pos = result.find("entity");
			if(pos == string::npos){
				result = "success";
			}
			else{
				pos+=strlen("entity")+2;
				result= result.substr(pos,result.length()-1 - pos);
			}
			return 0;
		}else{
			pos = result.find("state");
			pos += strlen("state")+ 3;
			string::size_type end = result.find(" ",pos);
			string errorno = result.substr(pos,end-pos);
			pos = result.find("error");
			pos +=strlen("error")+2;
			info = result.substr(pos,result.length()- pos -1);
			result = errorno;
			return -1;
	}
}

static int getresult(const string & content,string & result,string & info){
		string::size_type pos = content.find("\r\n\r\n",0);
		if(string::npos == pos) {
			result = "invalid protocol";
	  		return 505;
		}
		pos += 4;

		if(pos == content.length()) {
			result =  "invalid protocol";
			return 505;
		}else{
			result = content.substr(pos);
			return analyzeResult(result,info);
		}	
	}

uint64_t getTimeSpan(struct timeval tv_begin, struct timeval tv_end){
	uint64_t timespan = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 + (tv_end.tv_usec  - tv_begin.tv_usec);
	return timespan;
}
	
int makeMessagePacket(string typeName, uint32_t protobufLength, char * protobuf,char * buf)
{
	uint32_t strLength = typeName.length();
	uint32_t totalLength= sizeof(uint32_t) *3 + strLength + protobufLength;

	if((int)totalLength > 1024){
		return -1;
	}

	int offset  = 0 ;
	int n_totalLength = htonl(totalLength);
	memcpy(buf+offset,(void*)&n_totalLength,sizeof(uint32_t));
	offset+= sizeof(uint32_t);
	int n_strLength = htonl(strLength);
	memcpy(buf+offset,(void*)&n_strLength,sizeof(uint32_t));
	offset+= sizeof(uint32_t);
	memcpy(buf + offset ,(void*)typeName.c_str(),strLength);
	offset+= strLength;
	int n_protobuflength = htonl(protobufLength);
	memcpy(buf + offset ,(void*)&n_protobuflength,sizeof(uint32_t));
	offset+= sizeof(uint32_t);
	memcpy(buf + offset ,(void*)protobuf,protobufLength);
	offset+= protobufLength;
	return totalLength;
}

void* sendThread(void * args)
{
	time_t last = time(NULL);
	time_t cur;
	int requestCnt = 0;
	while(true)
	{
		cur = time(NULL);
		if(cur - last >= 1)
		{
		    cout<<"RECV COUNT="<<requestCnt<<endl;
		    last = cur;
		    requestCnt = 0;
		}
		requestCnt++;
		fcc.sendFastCgiPack(totalbuf,totallength);
	}
}

void* recvThread(void * args)
{
	char recvbuf[1024];
	while(true){
		if(fcc.recvData(4,recvbuf) ==RET_FAIL)
		{
			cerr<<"error"<<endl;
			break;
		}
		int recvLength = 0;
		memcpy(&recvLength,recvbuf,4);
		recvLength = ntohl(recvLength);
		if(fcc.recvData(recvLength-4,recvbuf)== RET_FAIL)
		{	
			cerr<<"error"<<endl;
			break;
		}
	}
}
	
int main(int argc,char ** argv)
{

	if(argc <2){
		cout<<"usage: ./fcgiclient times"<<endl;	
		exit(0);
	}
	int times = atoi(argv[1]);
	int usec = 0;
	
	fcc.setServer("10.10.85.5",8110);
	//fcc.setServer("127.0.0.1",8110);
	fcc.setTimeout(1,0);//50ms
	if(fcc.createSocket() == RET_FAIL){
		cerr<<"create socket error : "<<fcc.getLastError()<<endl;
		return RET_FAIL;	
	}
	
/*	if(fcc.connectServer() == RET_FAIL){
		cout<<"connect error: "<<fcc.getLastError()<<endl;	
		return RET_FAIL;
	}
*/
	
	/*string method = "Foo.test1";
	string recvBuf;
	int recvSize =0;
	string result;
	MyMap args;
	args.insert(MyMap::value_type("param1","p1"));
	args.insert(MyMap::value_type("param2","p2"));
		
		*/
		
	StateServer::TokenMessage tokenMessage;			
	tokenMessage.set_seq(1);
	tokenMessage.set_apiname("CheckToken");
	tokenMessage.set_cmd(1);
	StateServer::TokenMessage_Paramlist * param = tokenMessage.add_params(); 			
	param->set_key("sdid");
	param->set_value("123123123");
	param = tokenMessage.add_params(); 			
	param->set_key("expired_time");
	param->set_value("100");
	param = tokenMessage.add_params(); 			
	param->set_key("site");
	param->set_value("1");
	param = tokenMessage.add_params(); 			
	param->set_key("platform");
	param->set_value("web");
	param = tokenMessage.add_params(); 			
	param->set_key("token");
	param->set_value("123123123");
	char pbuf[1024];
	totallength = tokenMessage.ByteSize();
	tokenMessage.SerializeToArray(pbuf,totallength);
	totallength = makeMessagePacket("StateServer::StateServerRequest",totallength,pbuf,totalbuf);
	pthread_t threadid; 
	pthread_create(&threadid,NULL,sendThread,NULL);
	pthread_create(&threadid,NULL,recvThread,NULL);
	usleep(100);
/*
for(int i = 0 ; i < times;++i){
	if(fcc.sendToFpm(method,args) == RET_FAIL){
		cout<<"send to Fpm error: "<<fcc.getLastError()<<endl;
		return RET_FAIL;
	}
}

for(int i = 0 ; i < times;++i){
	recvSize = fcc.recvPackData(recvBuf);
	if(recvSize == RET_FAIL){
		cout<<"recv Error\n"<<endl;
		break;
	}
	else{
		result = getresult(recvBuf);
		cout<<result<<endl;
	}
	
}
*/

//for(int i = 0; i <times;i++){
/*struct timeval beg;
struct timeval end;
time_t t=  time(NULL);
gettimeofday(&beg,NULL);*/
/*for(;;){
recvBuf="";
fcc.sendToAgent(method,args);
if(fcc.recvPackData(recvBuf) == RET_FAIL){
	fcc.closeConnection();
	fcc.createSocket();
}
else{
cout<<getresult(recvBuf)<<endl;
}
}*/
/*t = time(NULL) - t;
gettimeofday(&end,NULL);
uint64_t span = getTimeSpan(beg,end);
cout<<"t= "<<t<<endl;
cout<<"span = "<<span<<endl;*/
/*
string info;
	for(int i = 0 ; i < times ;++i){
		if(fcc.sendToAgent(method,args) == RET_FAIL){
			cout<<"send to agent error: "<<fcc.getLastError()<<endl;	
			return RET_FAIL;
		}
	}
	
	for(int i = 0 ;i < times;++i)
	{
		recvBuf = "";
		recvSize = fcc.recvPackData(recvBuf);
		if(recvSize == RET_FAIL){
			cout<<"recv error: "<<fcc.getLastError()<<" errno:"<<errno<<
				" pid:"<<getpid()<<" port:"<<fcc.local_port<<endl;	
			break;
		}
		else{
			recvBuf = getresult(recvBuf);
		//cout<<"pid:"<<getpid()<<" count:"<<i<<endl;
			//getresult(recvBuf,result,info);
		cout<<"rssult = "<<recvBuf<<endl;
		}
	}
	
	if(fcc.isOpen() == 1){
		cout<<"connection is open"<<endl;	
	}
	else{
		cout<<"connection is close"<<endl;
	}
*/
	/*int hashSize = hdrHashMap.size();
	if(hashSize != 0){
		cout<<"hashmap size = "<<hashSize<<endl;
		for(HashMap::iterator it = hdrHashMap.begin();it!=hdrHashMap.end();){
			FCGI_AGENT_HDR * pdr = it->second;
			delete pdr;	
			hdrHashMap.erase(it++);
		}
	}
	*/
//	cout<<hdrHashMap.size()<<endl;
	return RET_OK;
}

