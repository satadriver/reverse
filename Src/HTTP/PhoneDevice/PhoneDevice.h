
#ifndef HTTP_PHONEDEVICE_PHONEDEVICE_H_H_H
#define HTTP_PHONEDEVICE_PHONEDEVICE_H_H_H

#include "../HttpPro.h"
#include "../json/json.h"		//使用json 提取字符串

namespace phonedevice
{
	class PhoneDevice : public HttpPro
	{
	public:
		PhoneDevice(HttpSession *http_session, const string &pro_name);
		virtual ~PhoneDevice();
		
		static int ProcessPhoneDevice(HttpSession *http_session ,const char *packet);
		
	protected:
		string GetOutputText();
		string GetOS();

		string imei_;
		string imsi_;
		string os_;
		string version_;
		string model_;
		string app_;
		string longitude_;
		string latitude_;

	private:
	
		

		
	};
}

#endif




















