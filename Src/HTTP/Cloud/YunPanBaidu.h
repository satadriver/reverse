
#ifndef HTTP_YUNPANBAIDU_H_H_H
#define HTTP_YUNPANBAIDU_H_H_H

#include "YunPan.h"
namespace yunpan
{
	class BaiDuYunPan : public YunPan
	{
	public:
		BaiDuYunPan(HttpSession *http_session, const string &pro_name);
		~BaiDuYunPan();
		
		static int IsBaiDuYunPan(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int UploadFileCW(const char *packet);
		int DownloadFileCW(const char *packet);

		int UploadFileCC(const char *packet);
		int DownloadFileCC(const char *packet);

		int UploadFileM_CC(const char *packet);
		int DownloadFileM_CC(const char *packet);
		enum 
		{
			kUploadFileCW = 1,
			kDownloadFileCW,

			kUploadFileCC,
			kDownloadFileCC,

			kUploadFileM_CC,
			kDownloadFileM_CC,

		};
	};
}
#endif