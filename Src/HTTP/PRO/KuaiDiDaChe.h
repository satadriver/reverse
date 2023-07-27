
#ifndef HTTP_KUAIDIDACHE_H_H_H
#define HTTP_KUAIDIDACHE_H_H_H

#include "../HttpPro.h"

class KuaiDiDaChe : public HttpPro
{
public:
	KuaiDiDaChe();
	~KuaiDiDaChe();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int PassengerLogin(const char *packet);
	int NewTextOrder(const char *packet);
	int NewVoiceOrder(const char *packet);
	int PreOrder(const char *packet);
	
	int DriverRegister(const char *packet);
	int DriverLogin(const char *packet);
	enum
	{
		kCMD_a = 1,
			kCMD_p,
			kCMD_d,
			kPassengerLogin,
			kNewTextOrder,
			kNewVoiceOrder,
			kPreOrder,
			kDriverRegister,
			kDriverLogin,
	};
	/*
	string phone_;
	
	string imei_;
	string imsi_;
	
	string longitude_;
	string latitude_;
	
	string from_;
		string to_;
	*/
	
};


#endif