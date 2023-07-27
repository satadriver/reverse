#ifndef HTTP_VIDEOACTIVITY_DIYITV_H_H_H
#define HTTP_VIDEOACTIVITY_DIYITV_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class DiYiVideo: public VideoActivity
	{
	public:
		DiYiVideo(HttpSession *http_session, const string &pro_name);
		~DiYiVideo();
		static int IsDiYiVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�����
		int PcWebSearch(const char *packet);
		//�û�����
		int PcWebComment(const char *packet);
		//�ֻ�����
		int AndroidSearch(const char *packet);
		//�ֻ�����
		int AndroidComment(const char *packet);

		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kAndroidSearch,
			kAndroidComment,
		};
	};
}

#endif