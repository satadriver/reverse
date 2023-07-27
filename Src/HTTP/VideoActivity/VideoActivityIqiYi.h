#ifndef HTTP_VIDEOACTIVITY_IQIYI_H_H_H
#define HTTP_VIDEOACTIVITY_IQIYI_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class IQiYiVideo: public VideoActivity
	{
	public:
		IQiYiVideo(HttpSession *http_session, const string &pro_name);
		~IQiYiVideo();
		static int IsIQiYiVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�����
		int PcWebSearch(const char *packet);
		//�û�����(��������)
		int PcWebComment(const char *packet);
		//�û�����(��ͨ����)
		int PcWebPublish(const char *packet);
		//�û��ظ�(���ݻظ�)
		int PcWebReply(const char *packet);
		//�û��ظ�(��ͨ�ظ�)
		int PcWebReplyGeneral(const char *packet);
		//�û���Ļ
		int PcWebDanmu(const char *packet);
		//���Կͻ�������
		int PcSearch(const char *packet);
		//�ֻ��ͻ�������
		int AndroidSearch(const char *packet);
		//�ֻ��ͻ�������
		int AndroidComment(const char *packet);
		//�ֻ��ͻ��˻ظ�
		int AndroidReply(const char *packet);
		//�ֻ��ͻ��˵�Ļ
		int AndroidDanmu(const char *packet);
		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kPcWebPublish,
			kPcWebReply,
			kPcWebReplyGeneral,
			//�û���Ļ
			kPcWebDanmu,
			kPcSearch,
			kAndroidSearch,
			kAndroidComment,
			kAndroidReply,
			kAndroidDanmu,
		};
	};
}

#endif