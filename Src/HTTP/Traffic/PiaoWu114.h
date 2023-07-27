// PiaoWu114.h: interface for the PiaoWu114 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIAOWU114_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
#define AFX_PIAOWU114_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_
/************************************************************************/
/* 114Æ±ÎñÍø                                                          */
/************************************************************************/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../HttpPro.h"
#include "Traffic.h"

class PiaoWu114: public Traffic
{
public:
	
	PiaoWu114(HttpSession *http_session, const string &pro_name);
	~PiaoWu114();
	int static Is(HttpSession *http_session);
	int Process(const char *packet,int action);
};

#endif // !defined(AFX_PIAOWU114_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
