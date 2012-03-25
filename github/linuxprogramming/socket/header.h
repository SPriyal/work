#ifndef HEADER
#define HEADER

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdarg.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <list>
using namespace std;

#define BEGIN_RECORD_ID 1
#define END_RECORD_ID 2

extern unsigned int recordid;

struct header{
	unsigned int length;
	unsigned int recordid;
	header(){}
	header(unsigned int length,unsigned int recordid):length(length),recordid(recordid){}
};


void err_exit(const char * err){
	perror(err);
	exit(-1);
}

void sock_err(const char * err,int fd = -1){
	if(fd >=0)
		close(fd);
	err_exit(err);
}

void makeBuf(char ** buf, string & content,unsigned int recordid){
		header hdr;
		hdr.length = (unsigned int)content.length();
		hdr.recordid = recordid;
		memcpy(*buf,(char*)&hdr,sizeof(header));
		memcpy(*buf + sizeof(header),content.c_str(),content.length());
		(*buf)[sizeof(header)+hdr.length]=0;
}

void getHeader(const char * buf,header * pheader){
	header*phdr = (header*)buf;	
	pheader->recordid = phdr->recordid;
	pheader->length = phdr->length;
}

#endif

