#ifndef HTTP_SHOPPING_PAIPAI_H_H_H
#define HTTP_SHOPPING_PAIPAI_H_H_H


#include "Shopping.h"

namespace shopping
{
	class PaiPai : public Shopping
	{
	public:
		PaiPai(HttpSession *http_session, const string &pro_name);
		~PaiPai();
		
		static int IsPaiPai(HttpSession *http_session );
		
		int Process(const char *packet,int action);
		
	private:
		
		int PCWebAddAddress(const char *packet);
		int AndroidAddAddress(const char *packet);
		enum
		{
			kPCWebAddAddress = 1,
			kAndroidAddAddress
		};
		
	};
}

#endif