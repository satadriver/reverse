#ifndef HTTP_EMPLOY_ZHUOBO_H_H_H
#define HTTP_EMPLOY_ZHUOBO_H_H_H

#include "Employ.h"

namespace employ
{
	class ZhuoBo : public Employ
	{
	public:
		ZhuoBo(HttpSession *http_session, const string &pro_name);
		~ZhuoBo();
		static int IsZhuoBo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//×¢²á(ÓÊÏä×¢²á)
		int PcWebRegisterEmail(const char *packet);
		//µÇÂ¼
		int PcWebLogin(const char *packet);
		//ÐÞ¸Ä¸öÈËÐÅÏ¢
		int PCWebModiy(const char *packet);
		enum
		{
			//×¢²á(ÓÊÏä×¢²á)
			kPcWebRegisterEmail = 1,
			//µÇÂ¼
			kPcWebLogin,
			//ÐÞ¸ÄÐÅÏ¢
			kPCWebModiy,
		};
	};
}

#endif