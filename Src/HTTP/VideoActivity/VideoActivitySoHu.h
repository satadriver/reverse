#ifndef HTTP_VIDEOACTIVITY_SOHU_H_H_H
#define HTTP_VIDEOACTIVITY_SOHU_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class SoHuVideo: public VideoActivity
	{
	public:
		SoHuVideo(HttpSession *http_session, const string &pro_name);
		~SoHuVideo();
		static int IsSoHuVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�����
		int PcWebSearch(const char *packet);
		//�û����� �ظ�
		int PcWebComment(const char *packet);
		//�û���Ļ
		int PcWebDanmu(const char *packet);
		//pc�ͻ�������
		int PcClientSearch(const char *packet);
		//�ֻ��û�����
		int AndroidSearch(const char *packet);
		//android���� �ظ�
		int AndroidComment(const char *packet);
		//android��Ļ
		int AndroidDanmu(const char *packet);
		enum
		{
			//�û�����
			kPcWebSearch = 1,
			//�û����� �ظ�
			kPcWebComment,
			//�û���Ļ
			kPcWebDanmu,
			//pc�ͻ�������
			kPcClientSearch,
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