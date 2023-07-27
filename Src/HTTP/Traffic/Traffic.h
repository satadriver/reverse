// Traffic.h: interface for the Traffic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Traffic_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
#define AFX_Traffic_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../HttpPro.h"
#include "../json/json.h"		//使用json 提取字符串


class Traffic : public HttpPro
{
public:
	enum {PC_UNKOWN, PC_LOGIN, PC_LOGIN_DYNAMIC,PC_BUS, PC_TRAIN, PC_AIRPLANE, PC_AIRPLANE_INTER, SP_LOGIN,SP_LOGIN_DYNAMIC,SP_BUS,SP_BUSSECOND,SP_TRAIN, SP_AIRPLANE};

	Traffic(HttpSession *http_session, const string &pro_name);
	virtual ~Traffic();
	static int ProcessTraffic(HttpSession *http_session ,const char *packet);
	void Write();
public:
	string middle_text(const char* szSrc, const char* szBegin, const char* szEnd, char** p);
protected:
	string number_;
	string price_;
	string start_;
	string end_;
	string name_;
	string cardid_;
	string tel_;
	string time_;
	string type_;
};

#endif // !defined(AFX_Traffic_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
