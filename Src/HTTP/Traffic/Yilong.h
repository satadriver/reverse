// Yilong.h: interface for the Tongcheng class.
//艺龙
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YILONG_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
#define AFX_YILONG_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../HttpPro.h"
#include "Traffic.h"

class Yilong : public Traffic
{
public:	
	Yilong(HttpSession *http_session, const string &pro_name);
	~Yilong();
	int static Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	//获取证件类型
	string GetCredentialType(string passengerIDCardTypeTemp);
};

#endif // !defined(AFX_YILONG_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
