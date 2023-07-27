#ifndef HTTP_VIDEOACTIVITY_YOUKU_H_H_H
#define HTTP_VIDEOACTIVITY_YOUKU_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class YouKuVideo: public VideoActivity
	{
	public:
		YouKuVideo(HttpSession *http_session, const string &pro_name);
		~YouKuVideo();
		static int IsYouKuVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//��ҳ����
		//pc�ͻ�������
		int PcWebSearch(const char *packet);
		//��ҳ���� �ظ�
		int PcWebComment(const char *packet);
		//��ҳ��Ļ
		int PcWebDanMu(const char *packet);
		//pc�ͻ��˵�Ļ
		int PcClientDanMu(const char *packet);
		//android����
		int AndroidSearch(const char *packet);
		//android���� �ظ�
		int AndroidComment(const char *packet);
		//android��Ļ
		int AndroidDanMu(const char *packet);

		enum
		{
			//��ҳ����
			//pc�ͻ�������
			kPcWebSearch = 1,
			//��ҳ���� �ظ�
			kPcWebComment,
			//��ҳ��Ļ
			kPcWebDanMu,
			//pc�ͻ��˵�Ļ
			kPcClientDanMu,
			//android����
			kAndroidSearch,
			//android���� �ظ�
			kAndroidComment,
			//android��Ļ
			kAndroidDanmu,
		};
	};
}

#endif