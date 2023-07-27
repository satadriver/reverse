#ifndef HTTP_VIDEOACTIVITY_WOLE_H_H_H
#define HTTP_VIDEOACTIVITY_WOLE_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class WoLeVideo: public VideoActivity
	{
	public:
		WoLeVideo(HttpSession *http_session, const string &pro_name);
		~WoLeVideo();
		static int IsWoLeVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//��ҳ����
		int PcWebSearch(const char *packet);
		//��ҳ���� �ظ�
		int PcWebComment(const char *packet);
		//��ҳ��Ļ
		int PcWebDanMu(const char *packet);
		//��ҳ����
		int PcWebPlay(const char *packet);
		//android����
		int AndroidSearch(const char *packet);
		enum
		{
			//��ҳ����
			kPcWebSearch = 1,
			//��ҳ���� �ظ�
			kPcWebComment,
			//��ҳ��Ļ
			kPcWebDanMu,
			//��ҳ����
			kPcWebPlay,
			//android����
			kAndroidSearch,
		};
	};
}
#endif