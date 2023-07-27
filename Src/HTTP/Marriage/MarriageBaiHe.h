#ifndef HTTP_MARRIAGE_BAIHE_H_H_H
#define HTTP_MARRIAGE_BAIHE_H_H_H

#include "Marriage.h"

namespace marriage
{
	class BaiHeMarriage: public Marriage
	{
	public:
		BaiHeMarriage(HttpSession *http_session, const string &pro_name);
		~BaiHeMarriage();
		static int IsBaiHeMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//��ҳ��¼
		int PcWebLogin(const char *packet);
		//��ҳ�޸�����
		int PcWebModiy(const char *packet);
		//��ҳʵ����֤
		int PcRealNameCheck(const char *packet);
		//��׿��¼
		int AndroidLogin(const char *packet);
		//��׿�޸�����
		int AndroidModify(const char *packet);
		//��׿ʵ����֤	
		int AndroidNameCheck(const char *packet); 
		//��׿ѧ����֤
		int AndroidEduCheck(const char *packet);
		//��׿������֤
		int AndroidFinCheck(const char *packet);
		//��׿����
		int AndroidLocation(const char *packet);

		enum
		{
			kPcLogin = 1,
			kPcWebModiy,
			kPcRealNameCheck,
			kAndroidLogin,
			kAndroidModify,
			kAndroidNameCheck,
			kAndroidEduCheck,
			kAndroidFinCheck,
			kAndroidLocation,
		};
	};
}

#endif