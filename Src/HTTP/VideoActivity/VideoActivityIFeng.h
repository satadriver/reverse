#ifndef HTTP_VIDEOACTIVITY_IFENG_H_H_H
#define HTTP_VIDEOACTIVITY_IFENG_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class IFengVideo: public VideoActivity
	{
	public:
		IFengVideo(HttpSession *http_session, const string &pro_name);
		~IFengVideo();
		static int IsIFengVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//��ҳ����
		int PcWebSearch(const char *packet);
		//��ҳ����
		int PcWebComment(const char *packet);
		//android����
		int AndroidSearch(const char *packet);
		//android���� �ظ�
		int AndroidComment(const char *packet);
		enum
		{
			//��ҳ����
			kPcWebSearch = 1,
			//��ҳ����
			kPcWebComment,
			//android����
			kAndroidSearch,
			//android���� �ظ�
			kAndroidComment,
		};
	};
}
#endif