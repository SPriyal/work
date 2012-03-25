#ifndef _PROTOCOL_H
#define _PROTOCOL_H

//�û���Ϣ��󳤶�Ϊ800byte���ѺͿͻ����̶�
#define MAX_USER_MESSAGE_LEN (800+100)
//�������ڵı��뷽����һ��byte�ᱻ���һ��2λ��16������
#define MAX_MESSAGE_STRUCT_LEN (MAX_USER_MESSAGE_LEN*2+400)
//һ�����ȡ����Ϣ�ĸ���
#define MAX_FETCH_MSG_COUNT	50
//һ����󷢻ص���Ӧ�Ĵ�С
#define MAX_SEND_DATA_LEN (MAX_MESSAGE_STRUCT_LEN*MAX_FETCH_MSG_COUNT)

#pragma pack(1)
typedef enum
{
	RET_OK = 0,
	RET_CONFIG_LOAD_FAIL = 1,
	RET_NET_LISTEN_FAIL  = 2,
	RET_NET_CONNECT_FAIL  = 3,
	RET_NET_MANAGER_START_FAIL  = 4,
	RET_NO_MEMORY  = 5,
	RET_CONTINUE	= 6,
	RET_ILLEGAL_CLIENT,
	RET_UNKOWN_SOURCE,
	RET_UKNOWN_CMD,
	RET_APP_NET_SEND_ERROR,
	RET_APP_NET_RECV_ERROR,
	RET_UNKNOW_PARAM,
	RET_SYS_BUSY,
	RET_FAIL,
	RET_QUEUE_PUSH_FAIL,
	RET_QUEUE_POP_FAIL,
	RET_DB_ERROR,
	RET_DATA_NOT_SATISFIED,
}ERR_CODE;


//! Э��������
enum WEB_CMD
{
	WEB_GET_USER_MSG = 1,
	WEB_GET_SPECIED_USER_MSG = 2,
	WEB_DEL_USER_MSG = 3,
	WEB_DEL_SPECIED_USER_MSG = 4,
	WEB_GET_SUM = 5,
	WEB_GET_SPECIED_USER_SUM = 6,
};

//! Э��������
enum CONNECTD_CMD
{
	CONNECTD_USER_MSG = 1,
	CONNECTD_KEEP_ALIVE = 0,
};

/*
typedef struct 
{
	uint16_t len;
	uint16_t cmd;
	uint16_t ver;
	uint32_t uin;		//��KeepAlive��ʱ��Ϊռλ��
}CONNECTD_HDR;*/

struct FCGI_AGENT_HDR
{
	uint32_t len;
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
/*
typedef struct 
{
	uint16_t length;
	uint16_t cmd;
	uint16_t record_id;
	
}CONNECTD_HDR;
*/

enum
{
	SAVE_FLAG_DISABLE = 0,	//0�������û�򿪹�������Ϣ��¼����
	SAVE_FLAG_ENABLE = 1,	//1����ǰ������Ϣ��¼�����Ǵ򿪵�
	SAVE_FLAG_DISABLE_AGAIN = 2,	//2���������򿪹�������Ϣ��¼���ܣ��������ǹرյġ�
};

enum
{
	REC_STATUS_AVAILABE = 0,
	REC_STATUS_UNAVAILABE = 1,
	REC_STATUS_DELETED = 2,
};

/*bitmask(uint32)+ ver(uint16) + msg_seq(uint32) + msg*/
#define SKIP_BYTE_USER_MSG	(sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint32_t)+sizeof(uint16_t))


#pragma pack()

#endif  /* _PROTOCOL_H */
