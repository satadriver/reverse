#ifndef HTTP_VIDEOACTIVITY_PPTV_H_H_H
#define HTTP_VIDEOACTIVITY_PPTV_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class PpTvVideo: public VideoActivity
	{
	public:
		PpTvVideo(HttpSession *http_session, const string &pro_name);
		~PpTvVideo();
		static int IsPpTvVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�����
		int PcWebSearch(const char *packet);
		//�û���Ļ
		int PcWebDanmu(const char *packet);
		//��ҳ���� 
		int PcWebComment(const char *packet);
		//��ҳ�ظ�
		int PcWebReply(const char *packet);
		//pc�ͻ�������
		int PcClientSearch(const char *packet);
		//�ֻ��û�����
		int AndroidSearch(const char *packet);
		//�ֻ��û�����
		int AndroidComment(const char *packet);
		//�ֻ��û��ظ�
		int AndroidReply(const char *packet);
		//�ֻ��û���Ļ
		int AndroidDanmu(const char *packet);
		enum
		{
			//��ҳ����
			kPcWebSearch = 1,
			//��ҳ����
			kPcWebComment,
			//��ҳ�ظ�
			kPcWebReply,
			//��ҳ��Ļ
			kPcWebDanmu,
			//pc�ͻ�������
			kPcClientSearch,
			//android����
			kAndroidSearch,
			//android����
			kAndroidComment,
			//android�ظ�
			kAndroidReply,
			//android��Ļ
			kAndroidDanmu,
		};
	};
}

#endif