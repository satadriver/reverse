#ifndef HTTP_MARRIAGE_JIAYUAN_H_H_H
#define HTTP_MARRIAGE_JIAYUAN_H_H_H

#include "Marriage.h"

namespace marriage
{
	class JiaYuanMarriage: public Marriage
	{
	public:
		JiaYuanMarriage(HttpSession *http_session, const string &pro_name);
		~JiaYuanMarriage();
		static int IsJiaYuanMarriage(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//网页修改资料
		int PcWebModiy(const char *packet);
		//安卓注册
		int AndroidRegiste(const char *packet);

		enum
		{
			kPcWebModiy = 1,
			kAndroidRegiste,
		};
	};
}

#endif