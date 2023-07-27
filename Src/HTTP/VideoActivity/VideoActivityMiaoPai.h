#ifndef HTTP_VIDEOACTIVITY_MIAOPAI_H_H_H
#define HTTP_VIDEOACTIVITY_MIAOPAI_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class MiaoPaiVideo: public VideoActivity
	{
	public:
		MiaoPaiVideo(HttpSession *http_session, const string &pro_name);
		~MiaoPaiVideo();
		static int IsMiaoPaiVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�ֻ�����
		int AndroidSearch(const char *packet);
		//�ֻ����ۻظ�
		int AndroidComment(const char *packet);
		enum
		{
			kAndroidSearch,
			kAndroidComment,
		};
	};
}

#endif