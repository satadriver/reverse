#ifndef HTTP_INTERFACELOGINRENREN_H_H_H
#define HTTP_INTERFACELOGINRENREN_H_H_H
#include "InterfaceLogin.h"

namespace interfacelogin
{
	class InterfaceLoginRenRen : public InterfaceLogin
	{
	public:
		InterfaceLoginRenRen(HttpSession *http_session, const string &pro_name);
		~InterfaceLoginRenRen();	
		static int IsInterfaceLoginRenRen(HttpSession *http_session);
		int Process(const char *packet,int action);		
	private:		
		int AndroidInterfaceRenRen(const char *packet);
		enum 
		{   
			kAndroidInterfaceRenRen = 1,	
		};
	};
}
#endif