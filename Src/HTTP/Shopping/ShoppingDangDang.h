
#ifndef HTTP_SHOPPING_SHOPPINGDANGDANG_H_H_H
#define HTTP_SHOPPING_SHOPPINGDANGDANG_H_H_H

#include "Shopping.h"

namespace shopping
{
	class DangDang : public Shopping
	{
		public:
			DangDang(HttpSession *http_session, const string &pro_name);
			~DangDang();	
			static int IsDangDang(HttpSession *http_session);	
			int Process(const char *packet,int action);		
		private:
			int PcWebAddAddress(const char *packet);
			int PcWebModifyAddress(const char *packet);
			int AndroidAddAddress(const char *packet);
			int AndroidLogin(const char *packet);
			enum
			{
				kPcWebAddAddress = 1,
				kPcWebModifyAddress,
				kAndroidAddAddress,
				kAndroidLogin,
			};		
	};
}

#endif // !defined(AFX_PHONESHOPJINGDONG_H__653E5D95_7641_4B4F_B3F6_9B17ABF3816E__INCLUDED_)
