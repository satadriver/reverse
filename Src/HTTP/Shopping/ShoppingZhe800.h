#ifndef HTTP_SHOPPING_ZHE800_H_H_H
#define HTTP_SHOPPING_ZHE800_H_H_H
/************************************************************************/
/* ��800                                                                     */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class Zhe800: public Shopping
	{
	public:
		Zhe800(HttpSession *http_session, const string &pro_name);
		~Zhe800();	
		static int IsZhe800(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc������ַ
		int PCWebAddAddress(const char *packet);
		//pc�޸ĵ�ַ
		int PCWebModifyAddress(const char *packet);	

		//android������ַ 
		int AndroidAddAddress(const char *packet);  
		//android�޸ĵ�ַ
		int AndroidModifyAddress(const char *packet); 
		//android�ύ����
		int AndroidNewOrder(const char *packet);		
		enum
		{
            //pc������ַ
			kPCWebAddAddress = 1,
			//pc�޸ĵ�ַ
			kPCWebModifyAddress,
			//android������ַ
			kAndroidAddAddress,
			//android�޸ĵ�ַ
			kAndroidModifyAddress,
			//android�ύ����
			kAndroidNewOrder,
		};	
	};
}
#endif