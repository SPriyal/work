#ifndef _DEFINITION_H
#define _DEFINITION_H
#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <map>

#include <net_manager/src/net_manager.h>
#include "msg_queue.h"
using namespace std;

using namespace utils;

typedef MessageQueue<Net_Event*> Request_queue;


typedef enum
{
	NET_OFFLINE = 0,
	NET_LISTENED,
	NET_CONNECTTING,
	NET_CONNECTED,
	NET_TIMEOUT,
	NET_ERROR,
	NET_CLOSE,
	NET_LOCAL_CLOSE,
	NET_UNAVALIABLE,

	NET_STATUS_BUTT = 255,

}Net_Status;


//! Socket��Ϣ
typedef struct _NetInfo
{
	uint32_t net_id;					//��Ӧ��Net Managerͨ��id
	uint64_t updateTime;				//���һ��Ӧ�ø����ӵ�time
	char ip[24];					//!< ����ip
	uint16_t port;					//!< ����port
	Net_Status	 status;			//��ʶ������״̬
	int32_t processing;			//��ǰ���ڴ����������
#ifdef MULTI_WORKER
	pthread_mutex_t m_thread_mutex;
#endif
}NetInfo;

//��¼netid ����Net��ϸ��Ϣ�Ķ�Ӧ
typedef map<uint32_t, NetInfo> NetMap;


typedef enum
{
	NET_SOURCE_CONNECTD = 1,
	NET_SOURCE_WEB = 2,
	NET_ADMIN_SOURCE = 3,
	NET_SOURCE_BUTT = 255,
}NET_SOURCE;


#define MAX_PHP_PACK_LEN 204800
#endif  /* _DEFINITION_H */
