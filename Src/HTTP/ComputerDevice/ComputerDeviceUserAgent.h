

#ifndef HTTP_COMPUTERDEVICE_COMPUTERDEVICEUSERAGENT_H_H_H
#define HTTP_COMPUTERDEVICE_COMPUTERDEVICEUSERAGENT_H_H_H

#include "ComputerDevice.h"

namespace computerdevice
{
	class UserAgent : public ComputerDevice
	{
	public:
		UserAgent(HttpSession *http_session, const string &pro_name);
		virtual ~UserAgent();
		
		static int IsUserAgent(HttpSession *http_session );
		int Process(const char *packet,int action);
	protected:
		
	private:

		int BaiDu(const char *packet);
		enum
		{
			kBaidu = 1
		};
	};
}

#endif
