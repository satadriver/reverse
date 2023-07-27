#ifndef HTTP_EMPLOY_SHUZIYINGCAI_H_H_H
#define HTTP_EMPLOY_SHUZIYINGCAI_H_H_H

#include "Employ.h"

namespace employ
{
	class ShuZiYingCai : public Employ
	{
	public:
		ShuZiYingCai(HttpSession *http_session, const string &pro_name);
		~ShuZiYingCai();
		static int IsShuZiYingCai(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);

	private:
		//注册(用户名注册)
		int PcWebRegisterUser(const char *packet);
		//登录
		int PcWebLogin(const char *packet);
		//基本信息(标准型简历)
		int PcWebPaMain(const char *packet);
		//基本信息(简述型简历)
		int PcWebPaSimple(const char *packet);
		//基本信息(微简历)
		int PcWebPaPrivate(const char *packet);
		//上传简历(附件简历) 
		int PcWebPaResume(const char *packet);

		//上传头像
		int PcWebPaPhoto(const char *packet);

		//android注册(用户名注册)
		int AndroidRegisterUser(const char *packet);
		//android登录
		int AndroidLogin(const char *packet);
		enum
		{
            //注册(用户名注册)
			kPcWebRegisterUser = 1,
			//登录
			kPcWebLogin,
			//基本信息(标准型简历)
			kPcWebPaMain,
			//基本信息(简述型简历)
			kPcWebPaSimple,
			//基本信息(微简历)
			kPcWebPaPrivate,
			//上传简历(附件简历) 
			kPcWebPaResume,
			//上传头像	
			kPcWebPaPhoto,
            //android注册(用户名注册)
			kAndroidRegisterUser = 1,
			//android登录
			kAndroidLogin,
		};
	};
}

#endif