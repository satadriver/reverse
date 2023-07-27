
#ifndef HTTP_KUAIDI_SHENTONG_H_H_H
#define HTTP_KUAIDI_SHENTONG_H_H_H

#include "KuaiDi.h"

namespace kuaidi
{
	class Shentong : public KuaiDi
	{
	public:
		Shentong(HttpSession *http_session, const string &pro_name);
		~Shentong();
		static int IsShenTong(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//pc ���ڿ����ѯ
		int ShenTong(const char *packet);
		//pc ���ʿ����ѯ
		int ShenTongInt(const char *packet);
		//�ֻ���ͨapp
		int AndroidST(const char *packet);
		int PClogin(const char *packet);
		enum
		{ 
			//���ڿ����ѯ
			kShenTong = 1,
			//���ʿ����ѯ
			kShenTongInt,
			kAndroidST,
			kPClogin,
		};
	};
}

#endif