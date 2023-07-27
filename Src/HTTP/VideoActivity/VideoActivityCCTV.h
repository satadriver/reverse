#ifndef HTTP_VIDEOACTIVITY_CCTV_H_H_H
#define HTTP_VIDEOACTIVITY_CCTV_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class CCTVVideo: public VideoActivity
	{
	public:
		CCTVVideo(HttpSession *http_session, const string &pro_name);
		~CCTVVideo();
		static int IsCCTVVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�����1
		int PcWebSearch(const char *packet);
		//�û����� �ظ�
		int PcWebComment(const char *packet);
		//pc���Զ�����
		int PcClientSearch(const char *packet);
		//�ֻ�����
		int AndroidSearch(const char *packet);
		//�ֻ����ۻظ�(���Զ�)
		int AndroidComment(const char *packet);

		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kPcClientSearch,
			kAndroidSearch,
			kAndroidComment,
		};
	};
}

#endif