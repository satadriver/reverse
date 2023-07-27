
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
		//����
		int SearchFileCW(const char *packet);
		//����
		int DownloadFileCW(const char *packet);
		enum 
		{
			//����
			kSearchFileCW = 1,
			//����
			kDownloadFileCW,
		};
	};
}
#endif