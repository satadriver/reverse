#ifndef HTTP_READING_READINGNEIHANDUANZI_H_H_H
#define HTTP_READING_READINGNEIHANDUANZI_H_H_H
/************************************************************************/
/* �ں�����                                                             */
/************************************************************************/
#include "Reading.h"

namespace reading
{
	class NeiHanDuanZi : public Reading
	{
	public:
		NeiHanDuanZi(HttpSession *http_session, const string &pro_name);
		~NeiHanDuanZi();
		
		static int IsNeiHanDuanZi(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		//pc ��Ͷ��
		int PCWebNewPost(const char *packet);
		//pc ������
		int PCWebNewComment(const char *packet);
		//pc ��Ͷ�� ��ͼ
		int PCWebUploadPicture(const char *packet);
		//pc ��Ͷ�� ��Ƶ
		int PCWebUploadVideo(const char *packet);
		//pc ��Ͷ�� ��Ƶ ·��
		int PCWebUploadVideoPath(const char *packet);

		//android ��Ͷ��
		int AndroidNewPost(const char *packet);
		//android ��Ͷ�� ��ͼ
		int AndroidUploadPicture(const char *packet);
		//android ��Ͷ�� ��Ƶ
		int AndroidUploadVideo(const char *packet);
		//android ������
		int AndroidNewComment(const char *packet);
		
		enum
		{
			//pc ��Ͷ��
			kPCWebNewPost = 1,
			//pc ��Ͷ�� ��ͼ
			KPCWebUploadPicture,
			//pc ��Ͷ�� ��Ƶ
			KPCWebUploadVideo,
			//pc ��Ͷ�� ��Ƶ ·��
			KPCWebUploadVideoPath,
			//pc ������
			kPCWebNewComment,
			//android ��Ͷ��
			kAndroidNewPost,
			//android ��Ͷ�� ��ͼ
			KAndroidUploadPicture,
			//android ��Ͷ�� ��Ƶ
			KAndroidUploadVideo,
			//android ������
			KAndroidNewComment,
		};
	};
}
#endif