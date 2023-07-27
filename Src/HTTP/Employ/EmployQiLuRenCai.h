#ifndef HTTP_EMPLOY_QILURENCAI_H_H_H
#define HTTP_EMPLOY_QILURENCAI_H_H_H

#include "Employ.h"

namespace employ
{
	class QiLuRenCai : public Employ
	{
	public:
		QiLuRenCai(HttpSession *http_session, const string &pro_name);
		~QiLuRenCai();
		static int IsQiLuRenCai(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//注册(邮箱注册)
		int PcWebRegisterEmail(const char *packet);
		//注册(手机号注册)
		int PcWebRegisterPhone(const char *packet);
		//登录
		int PcWebLogin(const char *packet);
		//基本信息
		int PcWebPaMain(const char *packet);
		//上传头像
		int PcWebPaPhoto(const char *packet);
		//上传简历
		int PcWebPaResume(const char *packet);
		enum
		{
			//注册(邮箱注册)
			kPcWebRegisterEmail = 1,
            //注册(手机号注册)
			kPcWebRegisterPhone,
			//登录
			kPcWebLogin,
			//基本信息
			kPcWebPaMain,
			//上传头像	
			kPcWebPaPhoto,
			//上传简历 
			kPcWebPaResume,
		};
	};
}

#endif