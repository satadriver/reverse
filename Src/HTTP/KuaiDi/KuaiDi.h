#ifndef HTTP_KUAIDI_KUAIDI_H_H_H
#define HTTP_KUAIDI_KUAIDI_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" //ʹ��json ��ȡ�ַ���

namespace kuaidi
{
	class KuaiDi : public HttpPro
	{
	public:
		KuaiDi(HttpSession *http_session, const string &pro_name);
		virtual ~KuaiDi();
		
		static int ProcessKuaidi(HttpSession *http_session ,const char *packet);

	protected:
		string hd;
	};
}

#endif