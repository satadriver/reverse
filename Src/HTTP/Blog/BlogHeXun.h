
#ifndef HTTP_BLOG_BLOGHEXUN_H_H_H
#define HTTP_BLOG_BLOGHEXUN_H_H_H

#include "Blog.h"

namespace blog
{
	class HeXun : public Blog
	{
	public:
		HeXun(HttpSession *http_session, const string &pro_name);
		~HeXun();
		static int IsHeXun(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//����
		int WebNewTopic(const char *packet);
		//����
		int WebPostComment(const char *packet);
		//�ϴ��ļ�
		int WebAttachFile(const char *packet);

		enum
		{
			//����
			kWebNewTopic = 1,
			//����
			kWebPostComment,
			//�ϴ��ļ�
			KWebAttachFile,		
		};
		//��λ��Ϣ
		string longitude_;
		string latitude_;
	};
}

#endif