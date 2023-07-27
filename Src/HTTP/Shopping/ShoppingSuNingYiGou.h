#ifndef HTTP_SHOPPING_SUNINGYIGOU_H_H_H
#define HTTP_SHOPPING_SUNINGYIGOU_H_H_H


#include "Shopping.h"
namespace shopping
{
	class SuNingYiGou : public Shopping
	{
	public:
		SuNingYiGou(HttpSession *http_session, const string &pro_name);
		~SuNingYiGou();
		
		static int IsSuNingYiGou(HttpSession *http_session );
		
		int Process(const char *packet,int action);
		
	private:
		
		int PCWebAddAddress(const char *packet);
		int AndroidAddAddress(const char *packet);
		int AndroidLocation(const char *packet);
		enum
		{
			kPCWebAddAddress = 1,
			kAndroidAddAddress,
			kAndroidLocation
		};
		
	};
}
#endif