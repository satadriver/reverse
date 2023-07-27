#ifndef HTTP_EMPLOY_ZHONGHUAYINGCAI_H_H_H
#define HTTP_EMPLOY_ZHONGHUAYINGCAI_H_H_H

#include "Employ.h"

namespace employ
{
	class ZhongHuanYingCai : public Employ
	{
	public:
		ZhongHuanYingCai(HttpSession *http_session, const string &pro_name);
		~ZhongHuanYingCai();
		static int IsZhongHuanYingCai(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		//个人信息
		int PcWebInfo(const char *packet);
		//修改个人信息
		int PCWebModiy(const char *packet);
		enum
		{
			//个人信息
			kPcWebInfo = 1,
			//修改信息
			kPCWebModiy,
		};
	};
}

#endif