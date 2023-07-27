
#ifndef HTTP_YUNPANXINLANG_H_H_H
#define HTTP_YUNPANXINLANG_H_H_H

#include "YunPan.h"
namespace yunpan
{
	class XinLangWeiPan : public YunPan
	{
	public:
		XinLangWeiPan(HttpSession *http_session, const string &pro_name);
		~XinLangWeiPan();	
		static int IsXinLangWeiPan(HttpSession *http_session);
		int Process(const char *packet,int action);	
	private:	
		//ËÑË÷
		int SearchFileCW(const char *packet);
		//ÏÂÔØ
		int DownloadFileCW(const char *packet);
		enum 
		{
			//ËÑË÷
			kSearchFileCW = 1,
			//ÏÂÔØ
			kDownloadFileCW,
		};
	};
}
#endif