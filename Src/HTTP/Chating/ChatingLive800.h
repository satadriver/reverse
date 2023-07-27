
#ifndef HTTP_CHATING_CHATINGLIVE_H_H_H
#define HTTP_CHATING_CHATINGLIVE_H_H_H

#include "Chating.h"

namespace chating
{
	class Live : public Chating
	{
	public:
		Live(HttpSession *http_session, const string &pro_name);
		~Live();
		static int IsLive(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int BaiJiaLe(const char *packet);
		int XinShiJiYuLeCheng(const char *packet);
		int ShenBoTaiYangCheng(const char *packet);
		int TongShengGuoJi(const char *packet);
		enum
		{
			kBaiJiaLe = 1,
			kXinShiJiYuLeCheng,
			kShenBoTaiYangCheng,
			kTongShengGuoJi,
		};
	};
}

#endif