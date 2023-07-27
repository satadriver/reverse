
#ifndef HTTP_KUAIDI_SHENTONG_H_H_H
#define HTTP_KUAIDI_SHENTONG_H_H_H

#include "KuaiDi.h"

namespace kuaidi
{
	class Shentong : public KuaiDi
	{
	public:
		Shentong(HttpSession *http_session, const string &pro_name);
		~Shentong();
		static int IsShenTong(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//pc 国内快件查询
		int ShenTong(const char *packet);
		//pc 国际快件查询
		int ShenTongInt(const char *packet);
		//手机申通app
		int AndroidST(const char *packet);
		int PClogin(const char *packet);
		enum
		{ 
			//国内快件查询
			kShenTong = 1,
			//国际快件查询
			kShenTongInt,
			kAndroidST,
			kPClogin,
		};
	};
}

#endif