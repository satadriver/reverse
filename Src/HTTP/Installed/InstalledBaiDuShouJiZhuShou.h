#ifndef HTTP_INSTALLED_INSTALLEDBAIDUSHOUJIZHUSHOU_H_H_H
#define HTTP_INSTALLED_INSTALLEDBAIDUSHOUJIZHUSHOU_H_H_H
/************************************************************************/
/* �ٶ��ֻ����� �� ��׿�г���һ����                         
�ڰ�װӦ�õĳ��� ��                                   */
/************************************************************************/
#include "Installed.h"
namespace installed
{
	class BaiDuShouJiZhuShou : public Installed
	{
	public:
		BaiDuShouJiZhuShou(HttpSession *http_session, const string &pro_name);
		~BaiDuShouJiZhuShou();
		
		static int IsBaiDuShouJiZhuShou(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:	
		int SoftwareUpdate2(const char *packet);
		enum 
		{
			kSoftwareUpdate1 = 1,
			kSoftwareUpdate2 = 2,			
		};
	};
}

#endif