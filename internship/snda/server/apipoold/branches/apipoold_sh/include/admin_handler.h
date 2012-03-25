
#ifndef _ADMIN_HANDLER_H_
#define _ADMIN_HANDLER_H_


//
#include "types.h"
#include "net_event.h"
#include "line_splitter.h"

#include <string>

using namespace std;


//! �������
//!
//! ÿ�����ӷ���������ʵ��һ���˺���ǩ���ĺ���, ע�ᵽAdmin��������, �����û����͵Ĺ�������
typedef void (*pfnUserCmdHandler)(const char *cmd, int cmd_len, char *result_buf, int result_buf_len, int& result_len);


//! udp���������Ϣ
struct AdminUdpInfo
{
	uint32_t net_id;
	string admin_ip;			//!< ����ip
	int admin_port;		//!< ����port
	time_t reconnect_time;		//!< �´γ������½���ͨ����ʱ��
};

//! udp�����������
class AdminUdpHandler
{
public:
	//! �����¼�������
	static void NetEventHandler(Net_Event& net_event);

	//! ʱ���¼�������
	static void TimerTickHandler(time_t current_time);

public:
	//! ��ʼ��
	static int initialize();

	//! �����������
	static void setUserCmdHandler(pfnUserCmdHandler handler);

	//! ����
	static int start();

	//! ֹͣ
	static int stop();

private:
	//! ��������¼�
	static void onNetError(Net_Event& net_event);

	//! ���������¼�
	static void onNetData(Net_Event& net_event);

private:
	//! �������
	static pfnUserCmdHandler user_cmd_handler;

	//! udp���������Ϣ
	static AdminUdpInfo admin_udp_info;

	//! �ؽ���ͨ��ʱ����
	static int ADMIN_RECONNECT_INTERVAL;
};




//! tcp���������Ϣ
struct AdminTcpInfo
{
	uint32_t net_id;
	string admin_ip;			//!< ����ip
	int admin_port;		//!< ����port
	time_t reconnect_time;		//!< �´γ������½���ͨ����ʱ��
};

//! tcp������������
class AdminTcpListener
{
public:
	//! �����¼�������
	static void NetEventHandler(Net_Event& net_event);

	//! ʱ���¼�������
	static void TimerTickHandler(time_t current_time);

public:
	//! ��ʼ��
	static int initialize();

	//! �����������
	static void setUserCmdHandler(pfnUserCmdHandler handler);

	//! ����
	static int start();

	//! ֹͣ
	static int stop();

private:
	//! ��������¼�
	static void onNetError(Net_Event& net_event);

	//! ���������¼�
	static void onNetAccept(Net_Event& net_event);

public:
	//! �������
	static pfnUserCmdHandler user_cmd_handler;

private:
	//! tcp���������Ϣ
	static AdminTcpInfo admin_tcp_info;

	//! �ؽ���ͨ��ʱ����
	static int ADMIN_RECONNECT_INTERVAL;

	//! tcp�����
	static Line_Splitter m_packet_splitter;
};


//! tcp�����������
class AdminTcpHandler
{
public:
	//! �����¼�������
	static void NetEventHandler(Net_Event& net_event);

private:
	//! ��������¼�
	static void onNetError(Net_Event& net_event);

	//! ���������¼�
	static void onNetData(Net_Event& net_event);
};


#endif // _ADMIN_HANDLER_H_
