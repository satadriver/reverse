#ifndef HTTP_YUNPANHUAWEI_H_H_H
#define HTTP_YUNPANHUAWEI_H_H_H

#include "YunPan.h"
namespace yunpan
{
	class HuaWeiYunPan : public YunPan
	{
	public:
		HuaWeiYunPan(HttpSession *http_session, const string &pro_name);
		~HuaWeiYunPan();
		
		static int IsHuaWeiYunPan(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadFile(const char *packet);
		int DownloadFile(const char *packet);
		enum 
		{
			kUploadFile = 1,
				kDownloadFile = 2
		};
	};
}
#endif