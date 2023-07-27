//
// C++ Interface: Getmoniterweb
//
// Description: 
//
//
// Author: lp&tcf <netspy@netspy>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GETMONITERWEB_H
#define GETMONITERWEB_H
#include <string>
#include <map>

/**
	@author lp&tcf <netspy@netspy>
*/

typedef struct _MONITER_UNIT
{
	CString classid;
	CString  webname;
	CString  strUrl;
	CString matchtype;
	
/*	_MONITER_UNIT(){
		memset(this, 0, sizeof(_MONITER_UNIT));
	}
	void Reset(){
		memset(this, 0, sizeof(_MONITER_UNIT));
}*/
}sMoniterUnit;

class moniterweb{
public:
    moniterweb();

    ~moniterweb();
public:
		bool UpdateMap();
		int GetMoniterweb(char* buf,CString url);
		bool outputbcp(char* buf,int buflen);
private:
		int m_build;

		map<CString,sMoniterUnit> m_moniterdomainmap[2];
		map<CString,sMoniterUnit> m_moniterurlnmap[2];
};
CString SplitTopDomain(const char *domain);
CString GetUrl(CString url);
#endif
