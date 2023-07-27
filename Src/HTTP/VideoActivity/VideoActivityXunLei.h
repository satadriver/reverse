#ifndef HTTP_VIDEOACTIVITY_XUNLEI_H_H_H
#define HTTP_VIDEOACTIVITY_XUNLEI_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class XunLeiVideo: public VideoActivity
	{
	public:
		XunLeiVideo(HttpSession *http_session, const string &pro_name);
		~XunLeiVideo();
		static int IsXunLeiVideo(HttpSession *http_session);
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
		//pc������
		int PcClientSearch(const char *packet);
		//�ֻ��û�����
		int AndroidSearch(const char *packet);
		//�ֻ��û�����
		int AndroidComment(const char *packet);
		//�ֻ��û���Ļ
		int AndroidDanmu(const char *packet);
		enum
		{
			kPcWebSearch = 1,
			kPcWebcomment,
			kPcWebReply,
			kPcWebDanmu,
			kPcClientSearch,
			kAndroidSearch,
			kAndroidComment,
			kAndroidDanmu,
		};
	};
}

#endif