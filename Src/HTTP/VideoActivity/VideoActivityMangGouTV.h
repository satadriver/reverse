#ifndef HTTP_VIDEOACTIVITY_MANGGUO_H_H_H
#define HTTP_VIDEOACTIVITY_MANGGUO_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class MangGuoVideo: public VideoActivity
	{
	public:
		MangGuoVideo(HttpSession *http_session, const string &pro_name);
		~MangGuoVideo();
		static int IsMangGuoVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//�û�����1
		int PcWebSearch(const char *packet);
		//�û�����2
		int PcWebSearcht(const char *packet);
		//�û����� �ظ�
		int PcWebComment(const char *packet);
		//pc������
		int PcClientSearch(const char *packet);
		//pc������ �ظ�
		int PcClientComment(const char *packet);
		//pc�˲���
		int PcClientPlay(const char *packet);
		//�ֻ�����
		int AndroidSearch(const char *packet);
		//�ֻ����ۻظ�
		int AndroidComment(const char *packet);

		enum
		{
			kPcWebSearch = 1,
			kPcWebSearcht,
			kPcWebComment,
			kPcClientSearch,
			kPcClientComment,
			kPcClientPlay,
			kAndroidSearch,
			kAndroidComment,
		};
	};
}

#endif