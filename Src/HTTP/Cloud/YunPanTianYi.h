
#ifndef HTTP_YUNPANTIANYI_H_H_H
#define HTTP_YUNPANTIANYI_H_H_H

#include "YunPan.h"
namespace yunpan
{
	class YunPanTianYi : public YunPan
	{
	public:
		YunPanTianYi(HttpSession *http_session, const string &pro_name);
		~YunPanTianYi();
		
		static int IsYunPanTianYi(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:	
		int UploadFileCW(const char *packet);
		int DownloadFileCW(const char *packet);
		int DownloadFileCWSecond(const char *packet);
		
		int UploadFileCC(const char *packet);
		int DownloadFileCC(const char *packet);
		
		int UploadFileM_CC(const char *packet);
		int DownloadFileM_CC(const char *packet);
		enum 
		{
				kUploadFileCW = 1,
				kDownloadFileCW,
				kDownloadFileCWSecond,
				
				kUploadFileCC,
				kDownloadFileCC,
				
				kUploadFileM_CC,
				kDownloadFileM_CC,			
		};
	};
}
#endif