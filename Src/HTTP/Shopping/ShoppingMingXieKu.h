
#ifndef HTTP_SHOPPING_MingXieKu_H_H_H
#define HTTP_SHOPPING_MingXieKu_H_H_H

#include "Shopping.h"


namespace shopping
{
	class MingXieKu : public Shopping
	{
	public:
		MingXieKu(HttpSession *http_session, const string &pro_name);
		~MingXieKu();
		
		static int IsMingXieKu(HttpSession *http_session);	
		int Process(const char *packet,int action);
		
	private:
		
		int PCWebOrderSubmit(const char *packet);
		int PCLogin(const char *packet);
		enum
		{
			kPCWebOrderSubmit = 1,
			kPCLogin
		};
	};
}

#endif