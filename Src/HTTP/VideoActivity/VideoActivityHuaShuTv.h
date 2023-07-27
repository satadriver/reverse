#ifndef HTTP_VIDEOACTIVITY_HUASHUTV_H_H_H
#define HTTP_VIDEOACTIVITY_HUASHUTV_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class HuaShuTvVideo: public VideoActivity
	{
	public:
		HuaShuTvVideo(HttpSession *http_session, const string &pro_name);
		~HuaShuTvVideo();
		static int IsHuaShuTvVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�����
		int PcWebSearch(const char *packet);
		//�û�����
		int PcWebComment(const char *packet);
		//pc�ͻ�������
		int PcClientSearch(const char *packet);
		//�ֻ��û�����
		int AndroidSearch(const char *packet);
		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kPcClientSearch,
			kAndroidSearch,
		};
	};
}

#endif