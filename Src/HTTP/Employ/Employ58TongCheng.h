#ifndef HTTP_EMPLOY_TongCheng58_H_H_H
#define HTTP_EMPLOY_TongCheng58_H_H_H

#include "Employ.h"

namespace employ
{
	class TongCheng58 : public Employ
	{
	public:
		TongCheng58(HttpSession *http_session, const string &pro_name);
		~TongCheng58();
		static int IsTongCheng58(HttpSession *http_session);
		int Process(const char *packet,int action);
		//全职简历新建 修改 基本信息
		int PcWebPaMain(const char *packet);
		//兼职简历新建 修改 基本信息
		int PcWebPaMain2(const char *packet);
		//android基本信息
		int AndroidPaMain(const char *packet);
	private:
		enum
		{
			//全职简历新建 修改 基本信息
			kPcWebPaMain = 1,
		    //兼职简历新建 修改 基本信息
			kPcWebPaMain2,
			//android基本信息
			kAndroidPaMain,
		};
	};
}

#endif