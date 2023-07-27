
#ifndef HTTP_BLOG_BLOGTENGXUNWEIBO_H_H_H
#define HTTP_BLOG_BLOGTENGXUNWEIBO_H_H_H

#include "Blog.h"

namespace blog
{
	class TengXunWeiBo : public Blog
	{
	public:
		TengXunWeiBo(HttpSession *http_session, const string &pro_name);
		~TengXunWeiBo();
		static int IsTengXunWeiBo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:

		string GetOutputText(int state = 0);
		string weibo_id_;
		enum
		{
			kAddWeiBo = 1,
			kForwardWeiBo,
			kAddComment
		};
		
	};
}

#endif