#ifndef HTTP_EMPLOY_51JOB_H_H_H
#define HTTP_EMPLOY_51JOB_H_H_H

#include "Employ.h"

namespace employ
{
	class QianChengWuYou : public Employ
	{
	public:
		QianChengWuYou(HttpSession *http_session, const string &pro_name);
		~QianChengWuYou();
		static int IsQianChengWuYou(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		//注册个人信息
		int PcWebInfo(const char *packet);
		//修改个人信息
		int PCWebModiy(const char *packet);
		//登录
		int PcLogin(const char *packet);
		//上传头像
		int PcWebPaPhoto(const char *packet);
		enum
		{
			//个人信息
			kPcWebInfo = 1,
			//修改信息
			kPCWebModiy,
			//登录
			kPcLogin,
			//上传头像	
			kPcWebPaPhoto
		};
	};
}

#endif