
#ifndef HTTP_BLOG_BLOGWANGYIBLOG_H_H_H
#define HTTP_BLOG_BLOGWANGYIBLOG_H_H_H

#include "Blog.h"

namespace blog
{
	class WangYiBlog: public Blog
	{
	public:
		WangYiBlog(HttpSession *http_session, const string &pro_name);
		~WangYiBlog();
		static int IsWangYiBlog(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//发表日志
		int WebNewTopic(const char *packet);
		//发表晚安日志
		int WebNewTopicNight(const char *packet);
		//发表心情
		int WebXinQing(const char *packet);
		//评论
		int WebPostComment(const char *packet);
		//转载
		int WebZhuanZai(const char *packet);
		//回复
		int WebReply(const char *packet);
		//上传文件 
		int WebAttachFile(const char *packet);

		//android发表日志
		int AndroidNewTopic(const char *packet);
		//android写心情
		int AndroidXinQing(const char *packet);
		//android评论
		int AndroidPostComment(const char *packet);
		//android回复
		int AndroidReply(const char *packet);
		enum
		{
			//发表日志
			kWebNewTopic =1,
			//发表晚安日志
			kWebNewTopicNight,
			//写心情
			KWebXinQing,
			//评论
			kWebPostComment,
			//转载
			KWebZhuanZai,
			//回复
			kWebReply,
			//上传文件
			KWebAttachFile,	
			//android发表日志
			kAndroidNewTopic,
			//android写心情
			KAndroidXinQing,
			//android评论
			kAndroidPostComment,
			//android回复
			kAndroidReply,
		};
	};
}

#endif