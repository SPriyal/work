
#ifndef _APIPOOLD_LISTENER_H
#define _APIPOOLD_LISTENER_H

#include <string>

#include "net_manager.h"

#include "string_splitter.h"
#include "common.h"

using namespace std;


//! apipoold������
class ApipooldListener
{
public:
	//! �����¼�������
	static void NetEventHandler(Net_Event& net_event);

	//! ʱ���¼�������
	static void TimerTickHandler(time_t current_time);

public:
	//! ��ʼ��
	static int initialize();

	//! ��������
	static int start();

	//! ֹͣ����
	static int stop();

private:
	//! ��������¼�
	static void onNetError(Net_Event& net_event);

	//! ���������¼�
	static void onNetAccept(Net_Event& net_event);

public:
	//! apipoold��������Ϣ
	static ListenInfo apipoold_longtcp_info;
	static ListenInfo apipoold_shorttcp_info;

private:
	//! tcp�����
	static String_Splitter m_packet_splitter;
};




#endif  /* _APIPOOLD_LISTENER_H */

