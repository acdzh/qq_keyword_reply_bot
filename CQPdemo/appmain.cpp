/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "stdlib.h"
#include "string"
#include "cqp.h"
# include "time.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����
#include<vector>
#include<iostream>
#include <fstream>
#include<algorithm>
#include<sstream>
using namespace std;
//*******************************************************************************************************************
//*******************************************************************************************************************
//*******************************************************************************************************************

#define MY_GROUP 5566445566//Ĭ��Ⱥ�ţ�ʵ�������ɾ��ҲûʲôӰ��
#define MY_SELF_QQ 123456789//������QQ

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;
//const char *cstr = str.c_str();how to make str to char*


class my_replys {
public:
	string keyword;
	vector<string> replys;
	my_replys(string txt, string reply) {
		keyword = txt;
		replys.clear();
		replys.push_back(reply);
	}
	void add(string reply) {
		if(find(replys.begin(), replys.end(), reply) == replys.end() )
		replys.push_back(reply);
	}
	void remove(string reply) {
		vector<string>::iterator ret;
		ret = std::find(replys.begin(), replys.end(), reply);
		if (ret != replys.end())
			ret = replys.erase(ret);
	}
};
vector<my_replys> key_words;

void push_in(string &a, string &b) {
	if (key_words.empty()) {
		my_replys temp(a, b);
		key_words.push_back(temp);
	}
	else {
		int qqq = -1;
		for (int i = 0; i < key_words.size(); i++) {
			if (key_words[i].keyword == a) {
				qqq = 0;
				key_words[i].add(b);
				break;
			}
		}
		if (qqq == -1) {
			my_replys temp(a, b);
			key_words.push_back(temp);
		}
	}
}

void remove_out(string &a, string &b) {
	if (!key_words.empty()) {
		for (int i = 0; i < key_words.size(); i++) {
			if (key_words[i].keyword == a) {
				key_words[i].remove(b);
			}

			if (key_words[i].replys.empty()) {
				key_words.erase(key_words.begin() + i);
			}
		}
	}
}

void back_up() {
	ofstream outfile;
	outfile.open("test.txt", ios::out| ios::trunc);
	outfile << '&';//���ڶ��ļ�ʱ�����жϸ��ļ��Ƿ�Ϊ��
	if (!key_words.empty()) {
		string a;
		string b;
		for (int i = 0; i < key_words.size(); i++) {
			a = key_words[i].keyword;
			if (!key_words[i].replys.empty()) {
				for (int j = 0; j < (key_words[i].replys).size(); j++) {
					b = key_words[i].replys[j];
					outfile << a << "&|&" << b << endl;
				}
			}
		}
	}
	outfile.close();
	CQ_sendPrivateMsg(ac, MY_SELF_QQ, "���ݳɹ���");
}

void read_in() {
	ifstream infile;
	string temp;
	string a;
	string b;
	infile.open("test.txt", ios::in);
	char ch = infile.get();
	if (ch != EOF && ch == '&') {
		while(getline(infile,temp)){
			a = "";
			b = "";
			for (unsigned int i = 0; i < temp.size(); i++) {
				if (temp[i] == '|') {
					a = temp.substr(0, i - 1);
					b = temp.substr(i + 2);
					push_in(a, b);
					//CQ_sendPrivateMsg(ac, MY_SELF_QQ, "��ȡ���ݳɹ���11111");
				}
			}
		}
	}
	infile.close();
	CQ_sendPrivateMsg(ac, MY_SELF_QQ, "��ȡ���ݳɹ���");
}

string my_print(vector<my_replys> &a) {
	string temp = "";
	for (int i = 0; i < a.size(); i++) {
		temp = temp + a[i].keyword + ":\n    ";
		for (int j = 0; j < a[i].replys.size(); j++) {
			temp = temp + a[i].replys[j] + "   ";
		}
		temp = temp + "\n";
	}
	return temp;
}
string my_print(string a) {
	int have_found  = 0;
	string out = "";
	if (!key_words.empty()) {
		for (int i = 0; i < key_words.size(); i++) {
			if (key_words[i].keyword == a) {
				have_found = 1;
				for (int j = 0; j < key_words[i].replys.size(); j++) {
					out = out + key_words[i].replys[j] + "\n";
				}
				return out;
			}
		}
	}
	if (have_found == 0) {
		out = "δ�ҵ��ùؼ��ʡ�";
		return out;
	}
	else {
		out = "���ݿ�Ϊ�գ�";
		return out;
	}
}

bool if_right(string a) {
	if (strstr(a.c_str(), " ")) {
		if (a[a.size() - 1] == ' ') {
			a.erase(a.size() - 1);
			while (a[a.size() - 1] == ' ') {
				a.erase(a.size() - 1);
			}
			if (strstr(a.c_str(), " ")) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
		
	}
	else {
		return false;
	}
}

void my_operator(string msg, int64_t qqGroup = MY_GROUP) {
	if (msg[0] == '!' || msg[0] == '��') {
		stringstream ss(msg);
		string temp;
		string temp2;
		string temp3;
		string out;
		if ((strstr(msg.c_str(), "!list ") || strstr(msg.c_str(), "��list ")) && if_right(msg)) {
			ss >> temp;
			ss >> temp;
			ss.clear();
			out = my_print(temp);
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();

		}
		else if ((strstr(msg.c_str(), "!add ") || strstr(msg.c_str(), "��add ")) && if_right(msg)) {
			ss >> temp;
			ss >> temp;
			ss >> temp2;
			ss.clear();
			push_in(temp, temp2);
			out = "��ӳɹ�����˵ " + temp + " ��˵ " + temp2 + " .";
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((strstr(msg.c_str(), "!remove ") || strstr(msg.c_str(), "��remove ")) && if_right(msg)) {
			ss >> temp;
			ss >> temp;
			ss >> temp2;
			ss.clear();
			remove_out(temp, temp2);
			out = "����ɹ�����˵ " + temp + " �Ҳ�����˵ " + temp2 + " .";
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((msg == "!LIST_ALL") || (msg == "��LIST_ALL")) {
			out = my_print(key_words);
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((strstr(msg.c_str(), "!edit ") || strstr(msg.c_str(), "��edit "))&& if_right(msg)) {
			ss >> temp;
			ss >> temp;
			ss >> temp2;
			ss >> temp3;
			ss.clear();
			remove_out(temp, temp2);
			push_in(temp, temp3);
			out = "�޸ĳɹ�����˵ " + temp + " �Ҳ�����˵ " + temp2 + " ,����˵ " + temp3 + " .";
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((msg == "!help") ||( msg == "��help")) {
			out = "!list �ؼ���: �г��ؼ��ʶ�Ӧ�����лظ�;\n!add �ؼ���1 �ؼ���2 : Ϊ�ؼ���1����һ��Ϊ�ؼ��ʶ��Ļظ�;\n!edit �ؼ���1 �ؼ���2 �ؼ���3 : ��˵���ؼ���1ʱ�����ٻظ��ؼ���2�����ǻظ��ؼ���3;\n!remove �ؼ���1 �ؼ���2 : ������߹���;\n!help : �鿴����;\n!backup : ���ݵ�ǰ�ʿ�.";
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((msg == "!backup") ||( msg == "��backup")) {
			back_up();
		}
}	
	else {
		string temp;
		int ran;
		string out;
		for (int i = 0; i < key_words.size(); i++) {
			temp = key_words[i].keyword;
			if (strstr(msg.c_str(), temp.c_str())) {
				srand((unsigned int)(time(NULL)));
				ran = (rand() % (key_words[i].replys.size()));
				out = key_words[i].replys[ran];
				CQ_sendGroupMsg(ac, qqGroup, out.c_str());
				out.clear();
				break;
			}
		}
	}
}

//***************************************************************************************************************************
//***************************************************************************************************************************
//***************************************************************************************************************************



/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {
	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	read_in();
	enabled = true;
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ

	//return EVENT_BLOCK;
	return EVENT_IGNORE;
}
 

/*
* Type=2 Ⱥ��Ϣ
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {

	//string a = my_print(key_words);
	//CQ_sendGroupMsg(ac, MY_GROUP, a.c_str());
	if (fromGroup != MY_GROUP) {
		my_operator(msg,fromGroup);

	}
	else {
		my_operator(msg);
	}
	

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "����menuA�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "����menuB�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}
