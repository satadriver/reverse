#ifndef HTTP_EMPLOY_51JOB_H_H_H
#define HTTP_EMPLOY_51JOB_H_H_H

#include "Employ.h"

namespace employ
{
	class QianChengWuYou : public Employ
	{
	public:
		QianChengWuYou(HttpSession *http_session, const string &pro_name);
		~QianChengWuYou();
		static int IsQianChengWuYou(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		//ע�������Ϣ
		int PcWebInfo(const char *packet);
		//�޸ĸ�����Ϣ
		int PCWebModiy(const char *packet);
		//��¼
		int PcLogin(const char *packet);
		//�ϴ�ͷ��
		int PcWebPaPhoto(const char *packet);
		enum
		{
			//������Ϣ
			kPcWebInfo = 1,
			//�޸���Ϣ
			kPCWebModiy,
			//��¼
			kPcLogin,
			//�ϴ�ͷ��	
			kPcWebPaPhoto
		};
	};
}

#endif