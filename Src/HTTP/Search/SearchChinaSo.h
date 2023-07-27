
#ifndef HTTP_SEARCH_CHINASO_H_H_H
#define HTTP_SEARCH_CHINASO_H_H_H

#include "Search.h"

namespace search
{
	class ChinaSo : public Search
	{
	public:
		ChinaSo(HttpSession *http_session, const string &pro_name);
		~ChinaSo();
		static int IsChinaSo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int WWW(const char *packet);
		enum
		{
			kNews = 1,
			kWeb,
			kPicture,
			kVideo,
			kBaike,
			kMap,
			kScholar,
			kBook,
		};	
	};
}
#endif