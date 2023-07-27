
#ifndef HTTP_PLANE_QINGDAOAIRLINES_H_H_H
#define HTTP_PLANE_QINGDAOAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class QingDaoAirlines : public Airplane
	{
	public:
		QingDaoAirlines(HttpSession *http_session, const string &pro_name);
		~QingDaoAirlines();
		static int IsQingDaoAirlines(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		//��¼
		int pclogin(const char *packet);
		int pcticket(const char *packet);
		int pcorder(const char *packet);
		enum
		{
			//��¼
			kpclogin = 1,
		   //��ѯ
			kpcticket,
			//�ύ����
			kpcorder,
		};
	};
}

#endif