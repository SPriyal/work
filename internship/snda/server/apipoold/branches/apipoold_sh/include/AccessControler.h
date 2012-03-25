/*
 * AccessControler.h
 * ����Ȩ�޿�����
 * Created on: 2010-8-25
 * Author: leo
 */

#ifndef _ACCESSCONTROLER_H

#define _ACCESSCONTROLER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <stdlib.h>
#include "configure.h"
#include "AccessControlerUtils.h"
#include "log.h"

using namespace std;

typedef vector<string> IpList;

typedef map<string, IpList> VisitorIpMap;	//�����ļ��û���Ӧ�ĺϷ�IP�б�

typedef struct AllowInfo {
	long amount;
	long freq;
	vector<string> apiList;
} AllowInfo;

typedef map<string, AllowInfo> VisitorDstserviceAllowInfoMap;	//�����ļ��û���Ӧ�ĺϷ����ʴ���������Ƶ�ʺ�API����

typedef deque<time_t> TimeArray;

typedef struct VisitorHistory {
	bool banFlag;
	long curAmount;
	long freq;
	time_t firstVisitTime;
	time_t lastVisitTime;
	TimeArray timeArray;
} VisitHistory;

typedef map<string, VisitorHistory> VisitorHistoryMap;	//�û���̬��Ϣ

class AccessControler {
public:
	//�����ʴ���ʱ������
	static long period4Amount;
	//������Ƶ��ʱ������
	static long period4Freq;
	//������IP�б���ŷ����߶�Ӧ�ĺϷ�IP��ַ�б��������ļ���ȡ
	static VisitorIpMap visitorIpMap;
	//�����߷������������б���ŷ����߷���˶�Ӧ�ĺϷ����ʴ���������Ƶ�ʺ�API���ƣ��������ļ���ȡ
	static VisitorDstserviceAllowInfoMap visitorDstserviceAllowInfoMap;
	//�����߷���ʱ����ʷ��¼����̬ͳ��
	static VisitorHistoryMap visitorHistoryMap;

	//��ʼ��������Ϣ�ͷ�����ʷ��Ϣ��0-��ʼ���ɹ�������-ʧ��
	static int initialize();

	//���� ���ʿ���������ã�0-��������ɹ�������-ʧ��
	static int loadVisitorConfigure();

	//����Ƿ��ֹ���ʣ�0-������ʣ�����-��ֹ����
	static int check(const char* visitor, const char* dstService,
			const char* apiName, const char* visitorIp, string &reason);

	//�ֶ���ֹ���ʣ�0-��ֹ�ɹ�������-ʧ��
	static int ban(const char* visitor, const char* dstService);

	//�ֶ������0-����ɹ�������-ʧ��
	static int unban(const char* visitor, const char* dstService);

	//���û���̬��Ϣdump��������
	static int dumpVisitorHistory(char *buf, int len);

    //�����ʱ�dump��������
    static int dumpAccessTable(char* buf, int len);

    //
    static int dumpAccessTableToFile(fstream& out);

	//�ӻ���load�û���̬��Ϣ
	//param buf:dump�������û���̬��Ϣ
	static int loadVisitorHistory(char *buf);

	//��ӡVisitorIpMap
	static void showVisitorIpMap();

	//��ӡVisitorDstserviceAllowInfoMap
	static void showVisitorDstserviceAllowInfoMap();

	//��ӡVisitorHistoryMap
	static void showVisitorHistoryMap();
};

#endif  /* _ACCESSCONTROLER_H */

