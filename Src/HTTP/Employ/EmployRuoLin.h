#ifndef HTTP_EMPLOY_RUOLIN_H_H_H
#define HTTP_EMPLOY_RUOLIN_H_H_H

#include "Employ.h"

namespace employ
{
	class RuoLin: public Employ
	{
	public:
		RuoLin(HttpSession *http_session, const string &pro_name);
		~RuoLin();
		static int IsRuoLin(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//注册(邮箱注册)
		int PcWebRegisterEmail(const char *packet);
		//登录
		int PcWebLogin(const char *packet);
		//基本信息
		int PcWebPaMain(const char *packet);
		//修改基本信息
		int PcWebResume(const char *packet);
		//上传头像
		int PcWebPaPhoto(const char *packet);
		//android登录
		int AndroidLogin(const char * packet);
		//android修改基本信息
		int AndroidResume(const char *packet);
		enum
		{
			//注册(邮箱注册)
			kPcWebRegisterEmail = 1,
			//登录
			kPcWebLogin,
			//新建基本信息
			kPcWebPaMain ,
			//修改基本信息
			kPcWebResume,
			//上传头像	
			kPcWebPaPhoto,
			//android登录
			kAndroidLogin,
			//android修改基本信息
			kAndroidResume,
		};
	};
}

#endif