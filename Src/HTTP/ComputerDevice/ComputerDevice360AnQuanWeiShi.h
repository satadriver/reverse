

#ifndef HTTP_COMPUTERDEVICE_COMPUTERDEVICE360ANQUANWEISHI_H_H_H
#define HTTP_COMPUTERDEVICE_COMPUTERDEVICE360ANQUANWEISHI_H_H_H

#include "ComputerDevice.h"

namespace computerdevice
{
	class AnQuanWeiShi360 : public ComputerDevice
	{
	public:
		AnQuanWeiShi360(HttpSession *http_session, const string &pro_name);
		virtual ~AnQuanWeiShi360();
		
		static int IsAnQuanWeiShi360(HttpSession *http_session );
		int Process(const char *packet,int action);
	protected:
		
	private:

		enum
		{
			kBaidu = 1
		};
	};
}

#endif
