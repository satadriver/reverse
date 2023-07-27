
#ifndef HTTP_YUNPANTENGXUN_H_H_H
#define HTTP_YUNPANTENGXUN_H_H_H

#include "YunPan.h"
namespace yunpan
{
	class TengXunWeiYun : public YunPan
	{
	public:
		TengXunWeiYun(HttpSession *http_session, const string &pro_name);
		~TengXunWeiYun();	
		static int IsTengXunWeiYun(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:		
		int DownloadFileCW(const char *packet);
		int DownloadFileCC(const char *packet);
		enum 
		{
			kDownloadFileCW = 1,
			kDownloadFileCC
		};
	};
}
#endif