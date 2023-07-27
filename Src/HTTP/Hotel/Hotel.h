#ifndef HTTP_HOTEL_H_H_H
#define HTTP_HOTEL_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 

//�����ռ�
namespace hotel  
{
	//����
	class Hotel:public HttpPro
	{
		public:
			//���캯��
			Hotel(HttpSession *http_session, const string &pro_name);
			//������
			virtual ~Hotel();
			//�ж��Ƿ�Ϊ����
			static int ProcessHotel(HttpSession *http_session ,const char *packet);
		//����������
		protected:
		   string app_;
		   string platformWeb;
		   string platformPC;
		   string platformAndroid;
		   string platformIos;
		   string platformAndPc;
	};
}
#endif
