#include "fcgiclient.h"
static MyMap defMap;
stringstream ss;
int FastCgiClient::sendFastCgiPack(string & buffer){
	int sendSize = 0;
	sendSize = send(fd,buffer.c_str(),buffer.length(),0);
cout<<"sendSize= "<<sendSize<<endl;
	if(sendSize == - 1){
		error = string(strerror(errno));
		close(fd);
		return RET_FAIL;
	}
	++requestID;
	return RET_OK;
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
	
void FastCgiClient::addAgentHeader(string & buffer,unsigned short cmd,unsigned short requestID)
{	
	char header[6];
	unsigned short length  = htons(buffer.length());
	cmd = htons(cmd);
	requestID = htons(requestID);
		
	memcpy(header,(void*)&length,2);
	memcpy(header+2,(void*)&cmd,2);
	memcpy(header+4,(void*)&requestID,2);
	buffer = string(header,6) + buffer;
}

void FastCgiClient::makeFgiPack(const string & method, string & buffer , const map<string,string> & args )
{
	unsigned char body[8];
	unsigned short bodyLength =  0 ;
	unsigned char header[8];
	makeBeginRequestBody(FCGI_RESPONSE, 0,body);
	makeHeader(FCGI_BEGIN_REQUEST, requestID, sizeof(body), 0,header);
	buffer += string((char*)header,8);
	buffer += string((char*)body,8);
//cout<<"request header length is :"<<buffer.length()<<endl;
	//文件名和方法名
	map<string,string>ar_pack;
	ar_pack["SCRIPT_FILENAME"] = rpcRoot;
	ar_pack["FRPC_METHOD"] = method;
	unsigned char nameValueBody[9+rpcRoot.length()+ method.length()];
	for(map<string,string>::iterator it = ar_pack.begin();it!=ar_pack.end();++it){
			bodyLength = makeNameValueHeader(it->first,it->second,nameValueBody);
//cout<<"bodylength is "<<bodyLength<<endl;
			makeHeader(FCGI_PARAMS,requestID,bodyLength,0,header);
			buffer += string((char*)header,8);
			buffer +=string((char*)nameValueBody,bodyLength);
	}
//cout<<"method  length is :"<<buffer.length()<<endl;
	
   	 	if(args.size() > 0){
       	 	for(map<string,string>::const_iterator cit = args.begin();cit!= args.end();++cit){
            			bodyLength = makeNameValueHeader("FRPC_ARGS_"+cit->first , cit->second,nameValueBody);
          	 	 	makeHeader(FCGI_PARAMS,requestID, bodyLength, 0,header);
           		 	buffer += string((char*)header,8);
					buffer +=string((char*)nameValueBody,bodyLength);
        		}
    		}
//cout<<"args  length is :"<<buffer.length()<<endl;
      //结束符   
      	makeHeader(FCGI_STDIN, requestID,0, 0,header);
      	buffer += string((char*)header,8);
//cout<<"end length is :"<<buffer.length()<<endl;
}

int FastCgiClient::createSocket(){
	fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd == -1){
		this->error = string(strerror(errno));
		return RET_FAIL;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval)) == -1){
		error = string(strerror(errno));
		close(fd);
		return RET_FAIL;
	}
	if(setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval)) == -1){
		error = string(strerror(errno));
		close(fd);
		return RET_FAIL;
	}
	return RET_OK;
}
	
int FastCgiClient::connectServer(){
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host.c_str());
	if(connect(fd,(struct sockaddr * )&addr,sizeof(struct sockaddr)) == -1){
		error = string(strerror(errno));
		close(fd);
		return RET_FAIL;
	}
	return RET_OK;
}
	
int FastCgiClient::sendToFpm(const string & method,  const map<string,string> & args = defMap)
{
	string buffer;
	makeFgiPack(method,buffer,args);
	return sendFastCgiPack(buffer);
}
	
int FastCgiClient::sendToAgent(const string & method,  const map<string,string> & args = defMap)
{
	string buffer;
	makeFgiPack(method,buffer,args);
	addAgentHeader(buffer,CONNECTD_USER_MSG,requestID);
	return sendFastCgiPack(buffer);
}

int FastCgiClient::sendAliveMsg(){
	string buffer;
	addAgentHeader(buffer,CONNECTD_KEEP_ALIVE,requestID);
	return sendFastCgiPack(buffer);
}

int FastCgiClient::recvData(int recvLen,char * buffer){
	int tmp = 0;
	while(true){
		tmp = recv(fd,buffer,recvLen,MSG_WAITALL);
		if(tmp == -1){
			error = strerror(errno);
			return RET_FAIL;
		}else if(tmp == 0){
			return RET_OK;
		}
		else if(tmp > 0){
			if(tmp >= recvLen){
				return tmp;	
			}
		}
	}
}

int FastCgiClient::recvAgentHeader(CONNECTD_HDR * hdr){
	char buf[6];
	int recvSize = recvData(6,buf);
	if(recvSize != 6)return recvSize;
	CONNECTD_HDR * phdr = (CONNECTD_HDR*)buf;
	hdr->length = ntohs(phdr->length);
	hdr->cmd = ntohs(phdr->cmd);
	hdr->requestID = ntohs(phdr->requestID);
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

int FastCgiClient::recvData(string & content){
	CONNECTD_HDR hdr;
	FCGI_HDR fcgi_hdr;
	char buf[1024];
	int tmp = 0;
	int ret = RET_OK;
	int recvLength = 0;
	/*ret = recvAgentHeader(&hdr);
	if(ret == RET_FAIL){
		cout<<"read agent"<<endl;
		close(fd);
		return RET_FAIL;
	}
	*/
	while(true){
		ret  = recvFcgiHeader(&fcgi_hdr);
		if(ret != sizeof(FCGI_HDR)){
			close(fd);
			return ret;
		}
		recvLength += ret;
		
		ret  = recvData(fcgi_hdr.contentLength,buf);
		if(ret == sizeof(CONNECTD_HDR)){
			close(fd);
			return ret;	
		}
		recvLength +=ret;
		
		if(fcgi_hdr.paddingLength > 0){
			char paddingBuf[fcgi_hdr.paddingLength];
			int padding = recvData(fcgi_hdr.paddingLength,paddingBuf);
			if( padding != sizeof(paddingBuf)){
				close(fd);
				return  padding;	
			}	
			recvLength +=  padding;
		}
		if(fcgi_hdr.type == FCGI_STDOUT 	||	fcgi_hdr.type == FCGI_STDERR) {
			content += string(buf,ret);
		}
		/*	
		if(recvLength >= hdr.length){
			break;	
		}*/
	}
}
	
int main()
{	
	FastCgiClient fcc;
	fcc.setServer("127.0.0.1",9000);
	fcc.setTimeout(0,0);//50ms
	if(fcc.createSocket() == RET_FAIL){
		cout<<"create socket error : "<<fcc.getLastError()<<endl;
		return RET_FAIL;	
	}
	
	if(fcc.connectServer() == RET_FAIL){
		cout<<"connect error: "<<fcc.getLastError()<<endl;	
		return RET_FAIL;
	}
	
	string method = "Foo.rpclist";
	MyMap args;
	args.insert(MyMap::value_type("userid","12345"));
	if(fcc.sendToFpm(method) == RET_FAIL){
		cout<<"send to Fpm error: "<<fcc.getLastError()<<endl;
		return RET_FAIL;
	}
	
	string recvBuf;
	int recvSize = fcc.recvData(recvBuf);
	if(recvSize == RET_FAIL){
		cout<<"recv error: "<<fcc.getLastError()<<endl;	
	}
	
	/*
	if(fcc.sendToAgent(method) == RET_FAIL){
		cout<<"send to agent error: "<<fcc.getLastError()<<endl;	
		return RET_FAIL;
	}
	
	recvBuf= "";
	recvSize = fcc.recvData(recvBuf);
	if(recvSize == RET_FAIL){
		cout<<"recv error: "<<fcc.getLastError()<<endl;	
	}
	*/
	return RET_OK;
}

