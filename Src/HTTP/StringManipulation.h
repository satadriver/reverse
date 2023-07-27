/************************************************************************/
/* 字符串处理相关函数                                                   */
/************************************************************************/
#pragma warning(disable: 4786)
#ifndef STRING_MANIPULATAION_H_H_H
#define STRING_MANIPULATAION_H_H_H

#include <map>
#include <set>
#include <string>
#include <vector>

using std::string;
using std::map;
using std::set;
using std::vector;

#include "../Compress.h"
class TransformEncoding
{
public:
	TransformEncoding();
	~TransformEncoding();	
	string UTF8ToGBK(const string& utf8);
private:
};

class StringManipulation
{
public:
	StringManipulation();
	~StringManipulation();

public:
	void SplitStringToMap(const string &src_string ,char sep_item,char sep_value,const string &to_trim = "");
	void SplitStringToMap(const string &src_string ,const string &sep_item,const string &sep_value,const string &to_trim = "");
	string GetValueFromMapByKey(const string &key_string,bool need_decode = false);
	string GetValueBetweenBeginEnd(const string &src_string,const string &begin_string,const string &end_string,bool need_decode = false);
	string UrlDecode(const string& url);
	string UTF8ToGBK(const string& utf8);
	string UnicodeToGBK(const string &unicode);
	string UnicodeToGBKPrefix_u(const string &src_string);
	//huang &#20170;&#22825;&#22825;&#27668;&#22909;&#26497;&#20102;,have a good time!! "今天天气好极了"
	string UnicodeToZhongWen(const string &src_string); 
	string HexstringToAsciistring(const string &hexstring);
	string Base64Decode(const string &base64,int &out_byte);
	string ToLower(const string &src_string);
	int ToInt(const string &src_string);
	string Trim(string &src_string ,const string &need_trim = " ");
	//字符串替换函数
	void Replace(string &src_string,const string &old_string ,const string &new_string);
	//收件人 格式化 2016-08-26
 	string SplitToFormat( string src_string , string sep_begin,  string sep_end);
private:
	map<string, string>temp_map_;
};

#endif