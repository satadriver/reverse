
#ifndef HTTP_SEARCH_SEARCHSO360_H_H_H
#define HTTP_SEARCH_SEARCHSO360_H_H_H

#include "Search.h"

namespace search
{
	class So360 : public Search
	{
	public:
		So360(HttpSession *http_session, const string &pro_name);
		~So360();
		static int IsSo360(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		enum
		{
			kNews = 1,
			kWenDa,
			kWeb,
			kVideo,
			kImage,
			kMusic,
			kMap,
			kBaiKe,
			kLiangYi,
			kSoft,
			kETao
		};

	};
}

#endif