
#ifndef HTTP_CHATING_PROVIDESUPPORT_H_H_H
#define HTTP_CHATING_PROVIDESUPPORT_H_H_H
/************************************************************************/
/* Provide Support 在线咨询    赌博网站                                 */
/************************************************************************/
#include "Chating.h"

namespace chating
{
	class ProvideSupport : public Chating
	{
	public:
		ProvideSupport(HttpSession *http_session, const string &pro_name);
		~ProvideSupport();
		static int IsProvideSupport(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ProvideSupport (官网)
		int PcProvideSupportSendMessage(const char *packet);
		//皇冠 (现金网)
		int PcHuangGuanSendMessage(const char *packet);
		//太阳城
		int PcTaiYangChengSendMessage(const char *packet);
		//888真人
		int Pc888ZhenRenSendMessage(const char *packet);
		//广东会
		int PcGuangDongHui(const char *packet);
		//威尼斯人赌场
		int PcWeiNiSiRenDuChang(const char *packet);
		//bet365
		int PcBet365(const char *packet);

		enum
		{
			kLogin = 1,
			//ProvideSupport (官网)
            kPcProvideSupportSendMessage,
			//皇冠 (现金网)
			kPcHuangGuanSendMessage,
			//太阳城
			kPcTaiYangChengSendMessage,
			//888真人
			kPc888ZhenRen,
			//广东会
			kPcGuangDongHui,
			//威尼斯人赌场
			kPcWeiNiSiRenDuChang,
			//bet365
			kPcBet365,
		};
		string GetAccount(const string &account);
	};
}

#endif