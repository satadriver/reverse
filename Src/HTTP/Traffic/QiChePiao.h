// QiChePiao.h: interface for the Tongcheng class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QICHEPIAO_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
#define AFX_QICHEPIAO_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/************************************************************************/
/* 12308 Æû³µÆ±															*/
/************************************************************************/
#include "../HttpPro.h"
#include "Traffic.h"

class QiChePiao : public Traffic
{
public:
	QiChePiao(HttpSession *http_session, const string &pro_name);
	~QiChePiao();	
	static int Is(HttpSession *http_session);	
	int Process(const char *packet,int action);	
private:
	
	int PassengerLogin(const char *packet);
	int NewTextOrder(const char *packet);
	int NewVoiceOrder(const char *packet);
	int PreOrder(const char *packet);
	
	int DriverRegister(const char *packet);
	int DriverLogin(const char *packet);
	int DriverWorking(const char *packet);
	int MobileBaoChe(const char *packet);
	int MobileTrip(const char *packet);
	int MobileYueChe(const char *packet);
	int MobileChengJI(const char *packet);
	int MobileJIChang(const char *packet);
	int MobileGangA0(const char *packet);

	void SplitString(const string& s, vector<string>& v, const string& c);

	enum
	{
		kPassengerLogin=1,
		kNewTextOrder,
		kNewVoiceOrder,
		kPreOrder,
		kDriverRegister,
		kDriverLogin,
		kDriverWorking,
		kMobileBaoChe,
		kMobileTrip,
		kMobileYueChe,
		kMobileChengJI,
		kMobileJIChang,
		kMobileGangA0,
	};
};

#endif // !defined(AFX_QUNAER_H__682B66B2_FE96_49CC_B6E7_9D83F26A1418__INCLUDED_)
