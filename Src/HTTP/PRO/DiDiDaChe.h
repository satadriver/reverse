
#ifndef HTTP_DIDITACHE_H_H_H
#define HTTP_DIDITACHE_H_H_H

#include "../HttpPro.h"

class DiDiDaChe : public HttpPro 
{
public:
	DiDiDaChe();
	~DiDiDaChe();

	int Process(const char *packet,int action);
	int Is(HttpSession *http_session);
private:
	
	int PassengerLogin(const char *packet);
	int NewOrder(const char *packet);
	int TextNewOrder(const char *packet);
	int VoiceNewOrder(const char *packet);
	int PreOrder(const char *packet);
	
	int DriverRegister(const char *packet);
	
	enum
	{
		kPassengerLogin = 1,
			kNewOrder,
			kPreOrder,
			kDriverRegister
	};
	//<IMEI,PHONE>
	static map<string,string>imei_phone_;
};

#endif