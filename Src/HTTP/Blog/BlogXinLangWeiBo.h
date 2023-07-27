
#ifndef HTTP_BLOG_BLOGXINLANGWEIBO_H_H_H
#define HTTP_BLOG_BLOGXINLANGWEIBO_H_H_H

#include "Blog.h"

namespace blog
{
	class XinLangWeiBo : public Blog
	{
	public:
		XinLangWeiBo(HttpSession *http_session, const string &pro_name);
		~XinLangWeiBo();
		static int IsXinLangWeiBo(HttpSession *http_session);
		int Process(const char *packet,int action);
	protected:

	private:
		int AddWeiBo(const char *packet);
		int ForwardWeiBo(const char *packet);
		int AddComment(const char *packet);

		int AddWeiBoMA(const char *packet);
		string GetOutputText(int state = 0);
		string mid_;
		enum
		{
			kAddWeiBo = 1,
			kForwardWeiBo,
			kAddComment,
			kAddWeiBoMA
		};
		
	};
}


#endif