
#ifndef HTTP_ACCOUNT_ACCOUNTBAIDU_H_H_H
#define HTTP_ACCOUNT_ACCOUNTBAIDU_H_H_H

#include "Account.h"

namespace account
{
	class BaiDu : public Account
	{
	public:
		BaiDu(HttpSession *http_session, const string &pro_name);
		~BaiDu();
		
		static int IsBaiDu(HttpSession *http_session);
		
		int Process(const char *packet,int action);

	private:
		enum
		{
			kwangyigongkaike = 1,
			kshipinliaotian9158,
			kcntv,
			kpptv,
			kbaidushipin,
			kbaiduyinyue,
			kdianyingwang,
			kdouban,
			kfengxing,
			kgangqinpu,
			kjipingangqin,
			klanrentingshu,
			kleshi,
			kmangguotv,
			kmeiyuediantai,
			ksouhushipin

		};

		int wangyigongkaike(const char *packet);
		int shipinliaotian9158(const char *packet);
		int cntv(const char *packet);
		int pptv(const char *packet);
		int baidushipin(const char *packet);
		int baiduyinyue(const char *packet);
		int dianyingwang(const char *packet);
		int douban(const char *packet);
		int fengxing(const char *packet);
		int gangqinpu(const char *packet);
		int jipingangqin(const char *packet);
		int lanrentingshu(const char *packet);
		int leshi(const char *packet);
		int mangguotv(const char *packet);
		int meiyuediantai(const char *packet);
		int souhushipin(const char *packet);
	};
}

#endif