// MangGuo.h: interface for the Tongcheng class.
//芒果网
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MANGGUO_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
#define AFX_MANGGUO_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../HttpPro.h"
#include "Traffic.h"

class MangGuo: public Traffic
{
public:	
	MangGuo(HttpSession *http_session, const string &pro_name);
	~MangGuo();
	int static Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	//获取证件类型
	string GetCredentialType(string passengerIDCardTypeTemp);
};
#endif // !defined(AFX_MANGGUO_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
