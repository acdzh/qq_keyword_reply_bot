/*/
/*  main.cpp(Vesion 1.0.0) 
/*  关键词自动回复机器人   
/*  
/*  Created by acdzh on 2018/09/28.  
/*  Copyright © 2018年 acdzh. All rights reserved.
/*/

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

#define MY_GROUP 5566445566//默认群号，实际上这个理论上是没什么用的
#define MY_SELF_QQ 123456789//调试者QQ，因为我不会写对话框，因此下面的相关功能我都是默认注释掉的，包括发布的cpk文件，有兴趣可以自行打开编译//135行

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;



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
	outfile.open("app/com.acdzh.keywordBot.txt", ios::out| ios::trunc);
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
}

void read_in() {
	
	string temp;
	string a;
	string b;

		ifstream infile;
		infile.open("app/com.acdzh.keywordBot.txt", ios::in);
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
					}
				}
			}
		}
		infile.close();
	
	
	//CQ_sendPrivateMsg(ac, MY_SELF_QQ, "读取数据成功！");
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
			out = "!list 关键词: 列出关键词对应的所有回复;\n!add 关键词1 关键词2 : 为关键词1增加一个为关键词二的回复;\n!edit 关键词1 关键词2 关键词3 : 当说出关键词1时，不再回复关键词2，而是回复关键词3;\n!remove 关键词1 关键词2 : 解除二者关联;\n!help : 查看帮助;\n!backup : 备份当前词库.\n别忘了经常备份！";
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((msg == "!backup") ||( msg == "！backup")) {
			back_up();
			CQ_sendGroupMsg(ac, qqGroup, "备份成功！");
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

CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}

CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}

CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}

CQEVENT(int32_t, __eventExit, 0)() {
	return 0;
}


CQEVENT(int32_t, __eventEnable, 0)() {
	read_in();
	enabled = true;
	return 0;
}
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {
	//return EVENT_BLOCK;
	return EVENT_IGNORE;
}
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {

	if (fromGroup != MY_GROUP) {
		my_operator(msg,fromGroup);
	}
	else {
		my_operator(msg);
	}
	return EVENT_IGNORE; 
}

CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; 
}

CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "请在群聊消息中发送!help查看。", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "https://github.com/acdzh", "" ,0);
	return 0;
}
