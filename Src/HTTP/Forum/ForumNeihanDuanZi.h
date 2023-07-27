#ifndef HTTP_FORUM_NEIHANDUANZI_H_H_H
#define HTTP_FORUM_NEIHANDUANZI_H_H_H
/************************************************************************/
/* 内涵段子                                                             */
/************************************************************************/
#include "Forum.h"
namespace forum
{
	class NeiHanDuanZi : public Forum
	{
	public:
		NeiHanDuanZi(HttpSession *http_session, const string &pro_name);
		~NeiHanDuanZi();
		
		static int IsNeiHanDuanZi(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		//pc 端投稿
		int PCWebNewPost(const char *packet);
		//pc 端评论
		int PCWebNewComment(const char *packet);
		//pc 端投稿 囧图
		int PCWebUploadPicture(const char *packet);
		//pc 端投稿 视频
		int PCWebUploadVideo(const char *packet);
		//pc 端投稿 视频 路径
		int PCWebUploadVideoPath(const char *packet);

		//android 端投稿
		int AndroidNewPost(const char *packet);
		//android 端投稿 囧图
		int AndroidUploadPicture(const char *packet);
		//android 端投稿 视频
		int AndroidUploadVideo(const char *packet);
		//android 端评论
		int AndroidNewComment(const char *packet);
		
		enum
		{
			//pc 端投稿
			kPCWebNewPost = 1,
			//pc 端投稿 囧图
			KPCWebUploadPicture,
			//pc 端投稿 视频
			KPCWebUploadVideo,
			//pc 端投稿 视频 路径
			KPCWebUploadVideoPath,
			//pc 端评论
			kPCWebNewComment,
			//android 端投稿
			kAndroidNewPost,
			//android 端投稿 囧图
			KAndroidUploadPicture,
			//android 端投稿 视频
			KAndroidUploadVideo,
			//android 端评论
			KAndroidNewComment,
		};
		string host_;
	};
}
#endif