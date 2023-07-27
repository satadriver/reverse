#ifndef HTTP_HARDWAREINFO_H_H_H
#define HTTP_HARDWAREINFO_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" //ʹ��json ��ȡ�ַ���

namespace hardwareinfo
{
	class HardwareInfo : public HttpPro
	{
	public:
		HardwareInfo(HttpSession *http_session, const string &pro_name);
		virtual ~HardwareInfo();
		
		static int ProcessHardwareInfo(HttpSession *http_session ,const char *packet);

	protected:
		string mid;          //Ӳ��������
		string app_;         //���
	};
}

#endif