
#ifndef HTTP_BLOG_BLOGPAPA_H_H_H
#define HTTP_BLOG_BLOGPAPA_H_H_H

#include "Blog.h"

namespace blog
{
	class PaPa : public Blog
	{
	public:
		PaPa(HttpSession *http_session, const string &pro_name);
		~PaPa();
		static int IsPaPa(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		int NewTopic(const char *packet);
		string longitude_;
		string latitude_;
		enum
		{
			kNewTopic = 1,
		};

	};
}

#endif