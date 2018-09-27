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
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
#include<vector>
#include<iostream>
#include <fstream>
#include<algorithm>
#include<sstream>
using namespace std;
//*******************************************************************************************************************
//*******************************************************************************************************************
//*******************************************************************************************************************

#define MY_GROUP 5566445566//默认群号，实际上这个删了也没什么影响
#define MY_SELF_QQ 123456789//调试者QQ

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
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
	outfile << '&';//用于读文件时方便判断该文件是否为空
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
	CQ_sendPrivateMsg(ac, MY_SELF_QQ, "备份成功！");
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
					//CQ_sendPrivateMsg(ac, MY_SELF_QQ, "读取数据成功！11111");
				}
			}
		}
	}
	infile.close();
	CQ_sendPrivateMsg(ac, MY_SELF_QQ, "读取数据成功！");
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
		out = "未找到该关键词。";
		return out;
	}
	else {
		out = "数据库为空！";
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
	if (msg[0] == '!' || msg[0] == '！') {
		stringstream ss(msg);
		string temp;
		string temp2;
		string temp3;
		string out;
		if ((strstr(msg.c_str(), "!list ") || strstr(msg.c_str(), "！list ")) && if_right(msg)) {
			ss >> temp;
			ss >> temp;
			ss.clear();
			out = my_print(temp);
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();

		}
		else if ((strstr(msg.c_str(), "!add ") || strstr(msg.c_str(), "！add ")) && if_right(msg)) {
			ss >> temp;
			ss >> temp;
			ss >> temp2;
			ss.clear();
			push_in(temp, temp2);
			out = "添加成功，你说 " + temp + " 我说 " + temp2 + " .";
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((strstr(msg.c_str(), "!remove ") || strstr(msg.c_str(), "！remove ")) && if_right(msg)) {
			ss >> temp;
			ss >> temp;
			ss >> temp2;
			ss.clear();
			remove_out(temp, temp2);
			out = "清除成功，你说 " + temp + " 我不会再说 " + temp2 + " .";
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((msg == "!LIST_ALL") || (msg == "！LIST_ALL")) {
			out = my_print(key_words);
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((strstr(msg.c_str(), "!edit ") || strstr(msg.c_str(), "！edit "))&& if_right(msg)) {
			ss >> temp;
			ss >> temp;
			ss >> temp2;
			ss >> temp3;
			ss.clear();
			remove_out(temp, temp2);
			push_in(temp, temp3);
			out = "修改成功，你说 " + temp + " 我不会再说 " + temp2 + " ,而是说 " + temp3 + " .";
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((msg == "!help") ||( msg == "！help")) {
			out = "!list 关键词: 列出关键词对应的所有回复;\n!add 关键词1 关键词2 : 为关键词1增加一个为关键词二的回复;\n!edit 关键词1 关键词2 关键词3 : 当说出关键词1时，不再回复关键词2，而是回复关键词3;\n!remove 关键词1 关键词2 : 解除二者关联;\n!help : 查看帮助;\n!backup : 备份当前词库.";
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((msg == "!backup") ||( msg == "！backup")) {
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
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
*/
CQEVENT(int32_t, __eventExit, 0)() {
	return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	read_in();
	enabled = true;
	return 0;
}


/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	//如果要回复消息，请调用酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息

	//return EVENT_BLOCK;
	return EVENT_IGNORE;
}
 

/*
* Type=2 群消息
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
	

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=4 讨论组消息
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=101 群事件-管理员变动
* subType 子类型，1/被取消管理员 2/被设置管理员
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=201 好友事件-好友已添加
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

/*
* 菜单，可在 .json 文件中设置菜单数目、函数名
* 如果不使用菜单，请在 .json 及此处删除无用菜单
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "这是menuA，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "这是menuB，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}
