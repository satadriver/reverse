// Lvmamalvyou.h: interface for the Tongcheng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LVMAMALVYOU_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
#define AFX_LVMAMALVYOU_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_
/************************************************************************/
/* ¿��������                                                           */
/************************************************************************/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../HttpPro.h"
#include "Traffic.h"

class Lvmamalvyou: public Traffic
{
public:
	
	Lvmamalvyou(HttpSession *http_session, const string &pro_name);
	~Lvmamalvyou();
	int static Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	//��ȡ֤������
	string GetCredentialType(string passengerIDCardTypeTemp);
	//pc ��¼��Ϣ (����Чhttps����)
	int PCWebLogin(const char *packet);
	
	//����
	void SplitString(const string& s, vector<string>& v, const string& c);

};

#endif // !defined(AFX_LVMAMALVYOU_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
