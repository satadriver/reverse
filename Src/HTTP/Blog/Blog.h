#ifndef HTTP_BLOG_BLOG_H_H_H
#define HTTP_BLOG_BLOG_H_H_H

#include "../HttpPro.h"
#include "../BoundaryParser.h"
//使用json 提取字符串
#include "../json/json.h" 

namespace blog
{
	class Blog : public HttpPro
	{
	public:
		Blog(HttpSession *http_session, const string &pro_name);
		virtual ~Blog();
	    //判断Blog 协议	
		static int ProcessBlog(HttpSession *http_session ,const char *packet);
        //上传文件 (适用于博客中插入图片,相册)
		void ConstructUploadAttachmentOuptputString();
	protected:
		virtual string GetOutputText(int state = 0);
		//用户名
		string user_;
		//用户id
		string nick_;
		//标题
		string title_;
		//内容
		string content_;
		//个人主页地址
		string blogdomain_;
	};
}
#endif