#ifndef HTTP_EMPLOY_ZHIYOUJI_H_H_H
#define HTTP_EMPLOY_ZHIYOUJI_H_H_H

#include "Employ.h"

namespace employ
{
	class ZhiYouJi: public Employ
	{
	public:
		ZhiYouJi(HttpSession *http_session, const string &pro_name);
		~ZhiYouJi();
		static int IsZhiYouJi(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//基本信息
		int PcWebPaMain(const char *packet);
		//修改基本信息
		int PcWebResume(const char *packet);
		enum
		{
			//新建基本信息
			kPcWebPaMain = 1,
			//修改基本信息
			kPcWebResume,
		};
	};
}

#endif