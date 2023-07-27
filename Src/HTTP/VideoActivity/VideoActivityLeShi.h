#ifndef HTTP_VIDEOACTIVITY_LESHI_H_H_H
#define HTTP_VIDEOACTIVITY_LESHI_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class LeShiVideo: public VideoActivity
	{
	public:
		LeShiVideo(HttpSession *http_session, const string &pro_name);
		~LeShiVideo();
		static int IsLeShiVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�����
		int PcWebSearch(const char *packet);
		//�û�����
		int PcWebComment(const char *packet);
		//�û��ظ�
		int PcWebReply(const char *packet);
		//�û���Ļ
		int PcWebDanmu(const char *packet);
		//���Կͻ�������
		int PcClientSearch(const char *packet);
		//���Կͻ��˵�Ļ
		int PcClientDanMu(const char *packet);
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
			kPcWebSearch = 1,
			kPcWebComment,
			kPcWebReply,
			kPcWebDanmu,
			//���Կͻ�������
			kPcClientSearch,
			//���Կͻ��˵�Ļ
			kPcClientDanMu,
			kAndroidSearch,
			kAndroidComment,
			kAndroidReply,
			kAndroidDanmu,
		};
	};
}

#endif