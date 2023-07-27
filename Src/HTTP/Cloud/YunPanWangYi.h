#ifndef HTTP_YUNPANWANGYI_H_H_H
#define HTTP_YUNPANWANGYI_H_H_H

#include "YunPan.h"
namespace yunpan
{
	class WangYiWangPan : public YunPan
	{
	public:
		WangYiWangPan(HttpSession *http_session, const string &pro_name);
		~WangYiWangPan();
		
		static int IsWangYiWangPan(HttpSession *http_session);
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