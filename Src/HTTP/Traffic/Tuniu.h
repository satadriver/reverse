// Tuniu.h: interface for the Tuniu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Tuniu_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
#define AFX_Tuniu_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../HttpPro.h"
#include "Traffic.h"

struct AirCityCode 
{
	int code;
	string city;
	string ita;
};

struct TrainCityCode 
{
	int code;
	string city;
};

class Tuniu : public Traffic
{
public:
	
	Tuniu(HttpSession *http_session, const string &pro_name);
	~Tuniu();
	int static Is(HttpSession *http_session);
	int Process(const char *packet,int action);

};

#endif // !defined(AFX_Tuniu_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
