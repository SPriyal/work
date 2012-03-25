/*
 * MyTime.cpp
 * ���ʿ������һЩ���߷���
 * Created on: 2010-8-25
 * Author: leo
 */

#include "AccessControlerUtils.h"

//ȥ���ַ�����߿��ַ�
string lTrim(const string& str) {
	return str.substr(str.find_first_not_of(" \n\r\t"));
}

//ȥ���ַ����ұ߿��ַ�
string rTrim(const string& str) {
	return str.substr(0, str.find_last_not_of(" \n\r\t") + 1);
}

//ȥ���ַ����������߿��ַ�
string trim(const string& str) {
	return lTrim(rTrim(str));
}

//���շָ����ָ��ַ�����������һ��string�͵�vector
vector<string> splitStrByDelim(string str, string delim) {
	int last = 0;
	vector<string> ret;
	for (int i = 0; i + delim.size() <= str.size(); i++) {

		bool ok = true;
		for (int j = 0; j < (int)delim.size() && ok; j++)
			ok = str[i + j] == delim[j];
		if (ok) {
			if (i - last) {
				ret.push_back(trim(str.substr(last, i - last)));
			}
			last = i + delim.size();
		}
	}
	if (last < (int)str.size())
		ret.push_back(trim(str.substr(last)));
	return ret;
}

//�ϲ�����vector�������غϲ����
vector<string> mergeVectors(vector<string> a, vector<string> b) {
	vector<string> ret = a;
	vector<string>::iterator iterB = b.begin();
	for (; iterB != b.end(); iterB++) {
		vector<string>::iterator iterRet = ret.begin();
		while (*iterRet != *iterB && iterRet != ret.end()) {
			iterRet++;
		}
		if (iterRet == ret.end()) {
			ret.push_back(*iterB);
		}
	}

	return ret;
}

//��ӡʱ������
void showTimeArray(std::list<time_t> lst) {
	cout << "TimeArray:  ";
	std::list<time_t>::iterator iter;
	for (iter = lst.begin(); iter != lst.end(); iter++) {
		cout << *iter << "  ";
	}
	cout << endl;
}

//����һ��ʱ��ֵ
void insertTimeArray(std::list<time_t> &lst, time_t t) {
	lst.push_back(t);
}

//����ʱ�����飬��һ������ǰ�ķ��ʼ�¼ɾ��
void resetTimeArray(std::list<time_t> &lst, time_t now, long period4Freq) {
	if (0 == lst.size()) {
		return;
	}
	std::list<time_t>::iterator iter;
	for (iter = lst.begin(); iter != lst.end();) {
		if ((now - *iter) > period4Freq) {
			std::list<time_t>::iterator iterTmp = iter;
			iter++;
			lst.erase(iterTmp);
		} else {
			break;
		}
	}
}

//�ҳ�vector<time_t>��һ������֮�ڵķ���Ƶ��
long getFreqPerPeriod(std::list<time_t> &lst, time_t now, long period4Freq) {
	if (0 == lst.size()) {
		return 0L;
	}
	resetTimeArray(lst, now, period4Freq);
//	long count = 0L;
//	std::list<time_t>::iterator iter;
//	for (iter = lst.begin(); iter != lst.end(); iter++) {
//		if ((now - *iter) <= period4Freq) {
//			count++;
//		}
//	}
//	return count;

	return lst.size();
}

