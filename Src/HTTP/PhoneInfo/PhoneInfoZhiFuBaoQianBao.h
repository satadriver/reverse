#ifndef HTTP_PHONEINFO_PHONEINFOZHIFUBAOQIANBAO_H_H_H
#define HTTP_PHONEINFO_PHONEINFOZHIFUBAOQIANBAO_H_H_H

#include "PhoneInfo.h"

namespace phoneinfo
{
	class ZhiFuBaoQianBao : public PhoneInfo
	{
	public:
		ZhiFuBaoQianBao(HttpSession *http_session, const string &pro_name);
		~ZhiFuBaoQianBao();
		
		static int IsZhiFuBaoQianBao(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadPhoneInfo(const char *packet);
		enum 
		{
			kUploadPhoneInfo = 1
		};
	};
}

#endif