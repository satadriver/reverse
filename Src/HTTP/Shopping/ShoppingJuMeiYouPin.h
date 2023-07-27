#ifndef SHOPPINGJUMEI_H_H_H
#define SHOPPINGJUMEI_H_H_H

#include "Shopping.h"

namespace shopping
{
	class JuMeiYouPin : public Shopping
	{
	public:
		JuMeiYouPin(HttpSession *http_session, const string &pro_name);
		~JuMeiYouPin();
		
		static int IsJuMeiYouPin(HttpSession *http_session );
		
		int Process(const char *packet,int action);
		
	private:
		
		int PCWebAddAddress(const char *packet);
		int PCWebLogin(const char *packet);

		int AndroidAddAddress(const char *packet);
		enum
		{
			kPCWebAddAddress = 1,
			kPCWebLogin,
			kAndroidAddAddress
		};
		string password_;
	};
}

#endif