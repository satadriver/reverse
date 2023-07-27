#ifndef HTTP_VIDEOACTIVITY_BAOFENGYINGYIN_H_H_H
#define HTTP_VIDEOACTIVITY_BAOFENGYINGYIN_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class BaoFengVideo: public VideoActivity
	{
	public:
		BaoFengVideo(HttpSession *http_session, const string &pro_name);
		~BaoFengVideo();
		static int IsBaoFengVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�����
		int PcWebSearch(const char *packet);
		//�û����� �ظ�
		int PcWebComment(const char *packet);
		//pc�ͻ�������
		int PcClientSearch(const char *packet);
		//pc�ͻ�������
		int PcClientComment(const char *packet);
		//pc�ͻ��˵�Ļ
		int PcClientDanmu(const char *packet);
		//pc�ͻ��˲���
		int PcClientPlay(const char *packet);
		//�ֻ�����
		int AndroidSearch(const char *packet);
		//�ֻ�����
		int AndroidComment(const char *packet);
		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kPcClientSearch,
			kPcClientComment,
			kPcClientDanmu,
			kPcClientPlay,
			kAndroidSearch,
			kAndroidComment,
		};
	};
}

#endif