
#ifndef HTTP_BLOG_BLOGWANGYILOFTER_H_H_H
#define HTTP_BLOG_BLOGWANGYILOFTER_H_H_H

#include "Blog.h"
namespace blog
{
	class WangYiLofter: public Blog
	{
	public:
		WangYiLofter(HttpSession *http_session, const string &pro_name);
		~WangYiLofter();
		static int IsWangYiLofter(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//登录
		int PCWebLogin(const char *packet);
		//文字
		int WebNewTopic(const char *packet);
		//图片
		int WebNewPicture(const char *packet);
		//音乐
		int WebNewMusic(const char *packet);
		//视频
		int WebNewVideo(const char *packet);
		//评论 回复
		int WebPostComment(const char *packet);
		//长文章
		int WebNewLong(const char *packet);
		//上传文件 GOOGLE
		int WebAttachFileGoogle(const char *packet);
		//android文字 图频 视频
		int AndroidNewTopicAll(const char *packet);
		//android评论
		int AndroidPostComment(const char *packet);

		enum
		{
			//登录
			kPCWebLogin = 1,
			//文字
			kWebNewTopic,
			//图片
			kWebNewPicture,
			//音乐
			kWebNewMusic,
			//视频
			kWebNewVideo,
			//评论 回复
			kWebPostComment,
			//长文章
			kWebNewLong,
			//上传文件 Google
			KWebAttachFileGoogle,
			//android 文字 图片 视频 
			kAndroidNewTopicAll,
			//android评论
			kAndroidPostComment,
		};
	};
}

#endif