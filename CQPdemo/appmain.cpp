/*/
/*  main.cpp(Vesion 1.1.0) 
/*  关键词自动回复机器人   
/*  
/*  Created by acdzh on 2019/03/19.  
/*  Copyright © 2019年 acdzh. All rights reserved.
/*/

#include <windows.h>
#include "stdint.h"
#include "stdlib.h"
#include "string"
#include "cqp.h"
#include "time.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

//*******************************************************************************************************************
//*******************************************************************************************************************
//*******************************************************************************************************************

#define MY_GROUP 5566445566//默认群号, 实际上这个没有用到
#define MY_SELF_QQ 123456789//调试者QQ, 同样未用到

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;


class my_replys {
public:
	string keyword;
	vector<string> replys;
	my_replys() {}
	my_replys(string txt, string reply) {
		keyword = txt;
		replys.clear();
		replys.push_back(reply);
	}
	void add(string reply) {
		if(find(replys.begin(), replys.end(), reply) == replys.end() )
		replys.push_back(reply);
	}
	bool has(string reply) {
		if (find(replys.begin(), replys.end(), reply) == replys.end())
			return false;
		else
			return true;
	}
	void remove(string reply) {
		vector<string>::iterator ret;
		ret = std::find(replys.begin(), replys.end(), reply);
		if (ret != replys.end())
			ret = replys.erase(ret);
	}
};
vector<my_replys> key_words;

bool has(string &a, string &b) {
	for ( auto i : key_words) {
		if (i.keyword == a) {
			if (i.has(b)) return true;
		}
	}
	return false;
}

bool has(string &a) {
	for (auto i : key_words) {
		if (i.keyword == a) {
			return true;
		}
	}
	return false;
}

void push_in(string &a, string &b) {
	if (key_words.empty()) {
		my_replys temp(a, b);
		key_words.push_back(temp);
	}
	else {
		int qqq = -1;
		for (size_t i = 0; i < key_words.size(); i++) {
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
		for (size_t i = 0; i < key_words.size(); i++) {
			if (key_words[i].keyword == a) {
				key_words[i].remove(b);
			}

			if (key_words[i].replys.empty()) {
				key_words.erase(key_words.begin() + i);
			}
		}
	}
}

void remove_out(string &a) {
	if (!key_words.empty()) {
		bool has = false;
		int flag = 0;
		for (size_t i = 0; i < key_words.size(); i++) {
			if (key_words[i].keyword == a) {
				has = true;
				flag = i;
				break;
			}
		}
		if (has == true) {
			key_words.erase(key_words.begin() + flag);
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
		for (size_t i = 0; i < key_words.size(); i++) {
			a = key_words[i].keyword;
			if (!key_words[i].replys.empty()) {
				for (size_t j = 0; j < (key_words[i].replys).size(); j++) {
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
	for (size_t i = 0; i < a.size(); i++) {
		temp = temp + a[i].keyword + ":\n    ";
		for (size_t j = 0; j < a[i].replys.size(); j++) {
			temp = temp + a[i].replys[j] + "   ";
		}
		temp = temp + "\n";
	}
	temp.erase(temp.end() - 1);
	return temp;
}
string my_print(string a) {
	int have_found  = 0;
	string out = "";
	if (!key_words.empty()) {
		for (size_t i = 0; i < key_words.size(); i++) {
			if (key_words[i].keyword == a) {
				have_found = 1;
				for (size_t j = 0; j < key_words[i].replys.size(); j++) {
					out = out + key_words[i].replys[j] + "\n";
				}
				out.erase(out.end() - 1);
				return out;
			}
		}
	}
	return "";
}

bool is_right(string a) { //这个函数是个智障..不要用
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
	if (msg[0] == '!' || msg[0] == '~') {
		msg[0] = '!';
		stringstream ss(msg);
		string temp;
		string temp2;
		string temp3;
		string out;
		if (strstr(msg.c_str(), "!list") ) {
			ss >> temp;
			temp.clear();
			ss >> temp;
			ss.clear();
			if (temp.empty()) { out = "Error: 你倒是给个关键词啊!\n      (╯°Д°)╯ ┴─┴\n( 请继续补充参数: key )";}
			else {
				if (has(temp)) {
					out = my_print(temp);
				}
				else {
					out = "Error: 我还没有学过这个关键词. \n         ╮(′～‵〞)╭\n( 请检查要查询的关键词 )";
				}
				
			}
			//out = to_string(temp.size()); //debug
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();

		}
		else if (strstr(msg.c_str(), "!add") ) {
			ss >> temp;
			temp.clear();
			ss >> temp;
			temp2.clear();
			ss >> temp2;
			ss.clear();

			if (temp.empty()) {
				out = "Error: 你倒是给个关键词啊! \n         (╯°Д°)╯ ┴─┴\n( 请继续补充参数: key, value )";
			}
			else {
				if (temp2 == "") {
					out = "Error: 你倒是教给我要回复什么啊! \n         (╯°Д°)╯ ┴─┴\n ( 请继续补充参数: value )";
				}
				else {
					if (has(temp, temp2)) { out = "Error: 我早就会这句了!\n         < (￣︶￣)>"; }
					else {
						push_in(temp, temp2);
						out = "添加成功，你说\"" + temp + "\"我说\"" + temp2 + "\".";
					}
				}
			}
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if (strstr(msg.c_str(), "!del") ) {
			ss >> temp;
			temp.clear();
			ss >> temp;
			temp2.clear();
			ss >> temp2;
			ss.clear();

			if (temp == "") {
				out = "Error: 你倒是给个关键词啊! \n         (╯°Д°)╯ ┴─┴( 请继续补充参数: key value )";
			}
			else {
				if (temp2 == "") {
					out = "Error: 你倒是要让我忘掉哪一句啊! \n         (｡ŏ_ŏ)( 请继续补充参数: value )";
				}
				else {
					if (has(temp, temp2)) {
						remove_out(temp, temp2);
						out = "已删除: key = \"" + temp + "\", value = \"" + temp2 + "\".";
					}
					else {
						out = "Error: 删除失败, 因为我本来就不会这句. \n         _(┐「ε:)_\n( 请检查提供的参数 )";
					}
				}
			}

			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if (strstr(msg.c_str(), "!keydel") ) {
			ss >> temp;
			temp.clear();
			ss >> temp;
			ss.clear();
			if (temp == "") {
				out = "Error: 你倒是给个关键词啊! \n         (╯°Д°)╯ ┴─┴\n( 请继续补充参数: key )";
			}
			else {
				if (has(temp)) {
					string values = my_print(temp);
					remove_out(temp);
					out = "已删除: key = \"" + temp + "\", value =\n" + values;
				}
				else {
					out = "Error: 我还没有学过这个关键词, 无法删除. \n         ╮(′～‵〞)╭\n( 请检查要删除的关键词 )";
				}
			}


			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((msg == "!LIST_ALL")) {
			out = my_print(key_words);
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if ((msg == "!LIST_ALL_DEBUG")) {
			ifstream infile;
			infile.open("app/com.acdzh.keywordBot.txt", ios::in);
			while (getline(infile, temp)) {
				out += (temp + "\n");
			}
			out.erase(out.end() - 1);
			temp.clear();
			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if (strstr(msg.c_str(), "!edit") ) {
			ss >> temp;
			temp.clear();
			ss >> temp;
			temp2.clear();
			ss >> temp2;
			temp3.clear();
			ss >> temp3;
			ss.clear();
			if (temp == "") {
				out = "Error: 你倒是给个关键词啊! \n         (╯°Д°)╯ ┴─┴\n( 请继续补充参数: key value1 value2 )";
			}
			else {
				if (temp2 == "") {
					out = "Error: 快告诉我要修改哪一句! \n         ( • ̀ω•́ )\n( 请继续补充参数: value1 value2 )";
				}
				else {





					if (has(temp, temp2)) {
						if (temp3 == "") {
							out = "Error: 这句我倒是会, 可是你想让我改成什么啊! \n         (｡ŏ_ŏ)\n( 请继续补充参数: value2 )";
						}
						else {
							if (has(temp, temp3)) {
								out = "Error: 我这两句都会, 不用改了! \n         < (￣︶￣)> \n( 要删除第一句请使用 !del 命令...orz )";
							}
							else {
								remove_out(temp, temp2);
								push_in(temp, temp3);
								out = "已修改: key = \"" + temp + "\", old_value = \"" + temp2 + "\", new_value = \"" + temp3 + "\".";

							}
						}
					}
					else {
						if (temp3 == "") {
							out = "Error: 我本来就不会这一句, 而且也不知道要改成什么, 改不了...  \n         ╮(′～‵〞)╭ \n( 要修改请检查参数并继续补充参数: value2 )\n(要直接添加第二句请使用 !add 命令...orz)";
						}
						else {
							out = "Error: 我本来就不会第一句, 改不了...  \n         ╮(′～‵〞)╭ \n( 要直接添加第二句请使用 !add 命令...orz )";
						}
						
					}
				}
			}

			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if (msg == "!help" || msg == "!帮助") {
		out = "!list  key\n"
			"    列出关键词 key 对应的所有回复;\n\n"
			"!add  key  value\n"
			"    学习回复: key-value ;\n\n"
			"!edit  key  old_value  new_value\n"
			"    更新回复;\n\n"
			"!del  key  value\n"
			"    忘记这一个回复_orz;\n\n"
			"!keydel  key\n"
			"    忘记关键词 key 的所有回复_orz;\n\n"
			"!help\n"
			"!帮助  :\n"
			"    查看帮助;\n\n"
			// "!restart\n"
			// "!重启  : \n"
			// "    只是个重启而已_orz; \n\n"
			// "!backup\n"
			// "!备份  : \n"
			// "    备份当前词库.\n\n"
			"(仅能识别英文符号..\n"
			"    更多请访问 https://github.com/acdzh/qq_keyword_reply_bot";

			CQ_sendGroupMsg(ac, qqGroup, out.c_str());
			out.clear();
		}
		else if (msg == "!backup" || msg == "!备份") {
			back_up();
			CQ_sendGroupMsg(ac, qqGroup, "备份成功！");
		}
		else if (msg == "!restart" || msg == "!重启") {
			read_in();
			CQ_sendGroupMsg(ac, qqGroup, "重启成功！");
		}
	}
	else {
		string temp;
		int ran;
		string out;
		for (size_t i = 0; i < key_words.size(); i++) {
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
