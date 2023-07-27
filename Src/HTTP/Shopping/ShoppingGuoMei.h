
#ifndef HTTP_SHOPING_GUOMEI_H_H_H
#define HTTP_SHOPING_GUOMEI_H_H_H

#include "Shopping.h"

namespace shopping
{
	class GuoMei : public Shopping
	{
	public:
		GuoMei(HttpSession *http_session, const string &pro_name);
		~GuoMei();		
		static int IsGuoMei(HttpSession *http_session);
		int Process(const char *packet,int action);		
	private:	
		int PCWebAddAddress(const char *packet);
		int PCWebModifyAddress(const char *packet);
		int PCWebAccountInsertAddress(const char *packet);
		int PCWebAccountUpdateAddress(const char *packet);

		enum
		{
			kPCWebAddAddress = 1,
            kPCWebModifyAddress,
            kPCWebAccountInsertAddress,
			kPCWebAccountUpdateAddress,
		};
	};
}
#endif