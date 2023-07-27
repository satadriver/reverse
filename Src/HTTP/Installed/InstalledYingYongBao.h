#ifndef HTTP_INSTALLED_INSTALLEDYINGYONGBAO_H_H_H
#define HTTP_INSTALLED_INSTALLEDYINGYONGBAO_H_H_H
#include "Installed.h"

//��ʱδ���� ����������
namespace installed
{
	class YingYongBao : public Installed
	{
	public:
		YingYongBao(HttpSession *http_session, const string &pro_name);
		~YingYongBao();
		
		static int IsYingYongBao(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:	
		//����ѹѹ����
		int SoftwareUpdate1(const char *packet); 
		//��ѹѹ����
		int SoftwareUpdate2(const char *packet);
		enum 
		{
			kSoftwareUpdate1 = 1,
			kSoftwareUpdate2 = 2,			
		};
	};
}

#endif