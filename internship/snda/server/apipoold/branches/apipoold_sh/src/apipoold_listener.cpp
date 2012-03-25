
#include "apipoold_listener.h"

#include "apipoold_handler.h"
#include "dispatcher.h"
#include "common.h"
#include "options.h"

#include "net_manager.h"
#include "log.h"


// �ؽ�����ͨ��ʱ����
#define APIPOOLD_RECONNECT_INTERVAL 10


//����MiddleAgent��APP�ṩ�����������Ϣ��0 Ϊ�����˿ڣ�����λ��Ϊ�����ӵ�APP
ListenInfo ApipooldListener::apipoold_longtcp_info;
ListenInfo ApipooldListener::apipoold_shorttcp_info;

String_Splitter ApipooldListener::m_packet_splitter;

void ApipooldListener::NetEventHandler(Net_Event& net_event)
{
	switch (net_event.net_event_type)
	{
	case TYPE_EXCEPTION:
	case TYPE_CLOSE:
		onNetError(net_event);
		break;

	case TYPE_ACCEPT:
		onNetAccept(net_event);
		break;

	default:
		LOG_FATAL("ApipooldListener::NetEventHandler error, unknown net_event_type:%d", net_event.net_event_type);
		break;
	}
}

void ApipooldListener::TimerTickHandler(time_t current_time)
{
	// ����ͨ��״̬
	if ((0 != apipoold_longtcp_info.reconnect_time) 
		&& (current_time >= apipoold_longtcp_info.reconnect_time))
	{
		start();
	}

	// ����ͨ��״̬
	if ((0 != apipoold_shorttcp_info.reconnect_time) 
		&& (current_time >= apipoold_shorttcp_info.reconnect_time))
	{
		start();
	}	
}

int ApipooldListener::initialize()
{
	LOG_INFO("ApipooldListener::initialize, longtcpip:%s, longtcpport:%d, shorttcpip:%s, shorttcpport:%d",
		Options::apipoold.apipoold_longtcp_ip.c_str(), 
		Options::apipoold.apipoold_longtcp_port,
		Options::apipoold.apipoold_shorttcp_ip.c_str(), 
		Options::apipoold.apipoold_shorttcp_port);

	apipoold_longtcp_info.net_id = 0;
	strncpy(apipoold_longtcp_info.ip, 
			Options::apipoold.apipoold_longtcp_ip.c_str(),
			23);
//		apipoold_longtcp_info.ip = Options::apipoold.apipoold_longtcp_ip;
	apipoold_longtcp_info.port = Options::apipoold.apipoold_longtcp_port;
	apipoold_longtcp_info.reconnect_time = 0;
	
	apipoold_shorttcp_info.net_id = 0;
//		apipoold_shorttcp_info.ip = Options::apipoold.apipoold_shorttcp_ip;
	strncpy(apipoold_shorttcp_info.ip, 
			Options::apipoold.apipoold_longtcp_ip.c_str(),
			23);
	apipoold_shorttcp_info.port = Options::apipoold.apipoold_shorttcp_port;
	apipoold_shorttcp_info.reconnect_time = 0;
	return 0;
}

int ApipooldListener::start()
{
	LOG_INFO("ApipooldListener::start long begin");

	if (0 != apipoold_longtcp_info.net_id) {			// ͨ���Ѿ�����
		return 0;
	}

	Net_Manager *net_manager = Dispatcher::getNetManager();
	apipoold_longtcp_info.net_id = net_manager->create_tcp_server(apipoold_longtcp_info.ip, apipoold_longtcp_info.port, &m_packet_splitter,
					NetEventHandler, ApipooldHandler::NetEventHandler);
	if (0 == apipoold_longtcp_info.net_id) {			// ��������ͨ��ʧ��
		apipoold_longtcp_info.reconnect_time = time(NULL) + APIPOOLD_RECONNECT_INTERVAL;	// �������½���ͨ��ʱ��
		LOG_FATAL("ApipooldListener::start error, create_tcp_server error");
		return -1;
	}

	apipoold_longtcp_info.reconnect_time = 0;

	LOG_INFO("ApipooldListener::start ok, net_id:%d", apipoold_longtcp_info.net_id);


	LOG_INFO("ApipooldListener::start short tcp begin");

	if (0 != apipoold_shorttcp_info.net_id) {			// ͨ���Ѿ�����
		return 0;
	}

	apipoold_shorttcp_info.net_id = net_manager->create_tcp_server(apipoold_shorttcp_info.ip, apipoold_shorttcp_info.port, &m_packet_splitter,
					NetEventHandler, ApipooldHandler::NetEventHandler);
	if (0 == apipoold_shorttcp_info.net_id) {			// ��������ͨ��ʧ��
		apipoold_shorttcp_info.reconnect_time = time(NULL) + APIPOOLD_RECONNECT_INTERVAL;	// �������½���ͨ��ʱ��
		LOG_FATAL("ApipooldListener::start error, create_tcp_server error");
		return -1;
	}

	apipoold_shorttcp_info.reconnect_time = 0;

	LOG_INFO("ApipooldListener::start ok, net_id:%d", apipoold_shorttcp_info.net_id);

	return 0;
}

int ApipooldListener::stop()
{
	LOG_INFO("ApipooldListener::stop long tcp begin, net_id:%d", apipoold_longtcp_info.net_id);

	if (0 == apipoold_longtcp_info.net_id) {		// û��ͨ��
		return 0;
	}

	Dispatcher::getNetManager()->delete_net(apipoold_longtcp_info.net_id);
	Dispatcher::removeNetEventHandler(apipoold_longtcp_info.net_id);

	apipoold_longtcp_info.net_id = 0;
	apipoold_longtcp_info.reconnect_time = 0;

	LOG_INFO("ApipooldListener::stop long tcp ok");

	
	LOG_INFO("ApipooldListener::stop short tcp begin, net_id:%d", apipoold_shorttcp_info.net_id);

	if (0 == apipoold_longtcp_info.net_id) {		// û��ͨ��
		return 0;
	}

	Dispatcher::getNetManager()->delete_net(apipoold_shorttcp_info.net_id);
	Dispatcher::removeNetEventHandler(apipoold_shorttcp_info.net_id);

	apipoold_shorttcp_info.net_id = 0;
	apipoold_shorttcp_info.reconnect_time = 0;

	LOG_INFO("ApipooldListener::stop short tcp ok");
	return 0;
}

void ApipooldListener::onNetError(Net_Event& net_event)
{
	LOG_FATAL("ApipooldListener::onNetError, net_id:%d", net_event.id);
	if(net_event.id == apipoold_longtcp_info.net_id)
	{
		apipoold_longtcp_info.net_id = 0;
		apipoold_longtcp_info.reconnect_time = time(NULL) + APIPOOLD_RECONNECT_INTERVAL;
	}
	else
	{
		apipoold_shorttcp_info.net_id = 0;
		apipoold_shorttcp_info.reconnect_time = time(NULL) + APIPOOLD_RECONNECT_INTERVAL;
	}
	return;
}

void ApipooldListener::onNetAccept(Net_Event& net_event)
{
	LOG_INFO("ApipooldListener::onNetAccept, net_id:%d, new_net_id:%d", net_event.id, net_event.new_id);

	ApipooldHandler::onNetConnect(net_event);
}




