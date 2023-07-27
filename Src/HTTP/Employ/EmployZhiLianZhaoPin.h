#ifndef HTTP_EMPLOY_ZHILIANZHAOPIN_H_H_H
#define HTTP_EMPLOY_ZHILIANZHAOPIN_H_H_H

#include "Employ.h"

namespace employ
{
	class ZhiLianZhaoPin : public Employ
	{
	public:
		ZhiLianZhaoPin(HttpSession *http_session, const string &pro_name);
		~ZhiLianZhaoPin();
		static int IsZhiLianZhaoPin(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//智联卓聘登录
        int PcWebLogin(const char *packet);
		//智联卓聘新建 修改 基本信息
		int PcWebInfo(const char *packet);
		//上传头像
		int PcWebPaPhoto(const char *packet);
		enum
		{
			//智联卓聘登录
			kPcWebLogin = 1,
			//个人信息
			kPcWebInfo,
			//上传头像	
			kPcWebPaPhoto,
		};
	};
}

#endif