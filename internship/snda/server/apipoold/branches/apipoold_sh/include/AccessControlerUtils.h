/*
 * MyTime.h
 * ���ʿ������һЩ���߷���
 * Created on: 2010-8-25
 * Author: leo
 */

#ifndef ACCESSCONTROLERUTILS_H_
#define ACCESSCONTROLERUTILS_H_

#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>

using namespace std;

//ȥ���ַ�����߿��ַ�
string lTrim(const string& str);

//ȥ���ַ����ұ߿��ַ�
string rTrim(const string& str);

//ȥ���ַ����������߿��ַ�
string trim(const string& str);

//���շָ����ָ��ַ���
vector<string> splitStrByDelim(string s, string delim);

//�ϲ�����vector�������غϲ����
vector<string> mergeVectors(vector<string> a, vector<string> b);

//����һ��ʱ��ֵ
void insertTimeArray(std::list<time_t> &lst, time_t t);

//����ʱ�����飬��һ������ǰ�ķ��ʼ�¼ɾ��
void resetTimeArray(std::list<time_t> &lst, time_t now);

//�ҳ�vector<time_t>��һ�������ڵķ���Ƶ��
long getFreqPerPeriod(std::list<time_t> &lst, time_t now, long period4Freq);

//��ӡʱ������
void showTimeArray(std::list<time_t> lst);

#endif /* MYTIME_H_ */
