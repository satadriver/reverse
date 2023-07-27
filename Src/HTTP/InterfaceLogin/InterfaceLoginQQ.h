#ifndef HTTP_INTERFACELOGINQQ_H_H_H
#define HTTP_INTERFACELOGINQQ_H_H_H
#include "InterfaceLogin.h"

namespace interfacelogin
{
	class InterfaceLoginQQ : public InterfaceLogin
	{
	public:
		InterfaceLoginQQ(HttpSession *http_session, const string &pro_name);
		~InterfaceLoginQQ();	
		static int IsInterfaceLoginQQ(HttpSession *http_session);
		int Process(const char *packet,int action);		
	private:		
		int AndroidInterfaceQQ(const char *packet);
		int AndroidInterFaceQQCheck(const char *packet);
		int AndroidInterfaceQQCheckSec(const char *packet);
		enum 
		{   
			kAndroidInterfaceQQCheck = 1,
			kAndroidInterfaceQQCheckSec,	
		    kAndroidInterfaceQQ,	
		};
	};
}
#endif