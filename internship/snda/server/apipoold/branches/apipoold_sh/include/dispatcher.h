
#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_


#include "types.h"
#include "net_manager.h"
#include "net_event.h"
#include "log.h"

#include <map>
#include <set>

using namespace std;


//! �����¼�������
typedef void (*pfnNetEventHandler)(Net_Event& net_event);


//! ʱ���¼�������
typedef void (*pfnTimerTickHandler)(time_t current_time);


//! ���¼�ѭ��������
class Dispatcher
{
public:
	//! ע�������¼�������
	static void registerNetEventHandler(uint32_t net_id, pfnNetEventHandler handler);

	//! ע��ʱ���¼�������
	static void removeNetEventHandler(uint32_t net_id);

	//! ע�������¼�������
	static void registerTimerTickHandler(pfnTimerTickHandler handler);

	//! ע��ʱ���¼�������
	static void removeTimerTickHandler(pfnTimerTickHandler handler);

	//! ȡ���������
	static Net_Manager* getNetManager();

	//! �����¼�ѭ��
	static void run();

	//! ֹͣ�¼�ѭ��
	static void stop();

private:
	typedef map<uint32_t, pfnNetEventHandler> NetEventHandlerMap;

	typedef set<pfnTimerTickHandler> TimerTickHandlerSet;

	static NetEventHandlerMap m_net_event_handler_map;

	static TimerTickHandlerSet m_timer_tick_handler_set;

	static Net_Manager m_net_manager;

	static bool m_stop;
};


#endif // _DISPATCHER_H_
