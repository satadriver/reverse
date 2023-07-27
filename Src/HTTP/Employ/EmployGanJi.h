#ifndef HTTP_EMPLOY_GANJI_H_H_H
#define HTTP_EMPLOY_GANJI_H_H_H

#include "Employ.h"

namespace employ
{
	class GanJi: public Employ
	{
	public:
		GanJi(HttpSession *http_session, const string &pro_name);
		~GanJi();
		static int IsGanJi(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//注册(手机号注册)
		int PcWebRegisterPhone(const char *packet);
		//基本信息
		int PcWebPaMain(const char *packet);
		//修改基本信息
		int PcWebResume(const char *packet);
		//上传头像
		int PcWebPaPhoto(const char *packet);
		enum
		{
			//注册(手机号注册)
			kPcWebRegisterPhone = 1,
			//新建基本信息
			kPcWebPaMain ,
			//修改基本信息
			kPcWebResume,
			//上传头像	
			kPcWebPaPhoto,
		};
	};
}

#endif