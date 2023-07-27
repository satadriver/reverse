#ifndef HTTP_BLOG_BLOG_H_H_H
#define HTTP_BLOG_BLOG_H_H_H

#include "../HttpPro.h"
#include "../BoundaryParser.h"
//ʹ��json ��ȡ�ַ���
#include "../json/json.h" 

namespace blog
{
	class Blog : public HttpPro
	{
	public:
		Blog(HttpSession *http_session, const string &pro_name);
		virtual ~Blog();
	    //�ж�Blog Э��	
		static int ProcessBlog(HttpSession *http_session ,const char *packet);
        //�ϴ��ļ� (�����ڲ����в���ͼƬ,���)
		void ConstructUploadAttachmentOuptputString();
	protected:
		virtual string GetOutputText(int state = 0);
		//�û���
		string user_;
		//�û�id
		string nick_;
		//����
		string title_;
		//����
		string content_;
		//������ҳ��ַ
		string blogdomain_;
	};
}
#endif