#ifndef HTTP_VIDEOACTIVITY_TUDOU_H_H_H
#define HTTP_VIDEOACTIVITY_TUDOU_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class TuDouVideo: public VideoActivity
	{
	public:
		TuDouVideo(HttpSession *http_session, const string &pro_name);
		~TuDouVideo();
		static int IsTuDouVideo(HttpSession *http_session);
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
		//pc�ͻ�������
		int PcClientSearch(const char *packet);
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
			kPcWebSearch = 1,
			//��ҳ���� �ظ�
			kPcWebComment,
			//��ҳ��Ļ
			kPcWebDanMu,
			//��ҳ����
			kPcWebPlay,
			//pc�ͻ�������
			kPcClientSearch,
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