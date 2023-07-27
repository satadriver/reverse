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
		//网页登录
		int PcWebLogin(const char *packet);
		//网页修改资料
		int PcWebModiy(const char *packet);
		//网页实名认证
		int PcRealNameCheck(const char *packet);
		//安卓登录
		int AndroidLogin(const char *packet);
		//安卓修改资料
		int AndroidModify(const char *packet);
		//安卓实名认证	
		int AndroidNameCheck(const char *packet); 
		//安卓学历认证
		int AndroidEduCheck(const char *packet);
		//安卓金融认证
		int AndroidFinCheck(const char *packet);
		//安卓附近
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