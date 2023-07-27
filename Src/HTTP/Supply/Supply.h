#ifndef HTTP_SUPPLY_H_H_H
#define HTTP_SUPPLY_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 
#include "../BoundaryParser.h"
//�����ռ�
namespace supply  
{
	//����
	class Supply:public HttpPro
	{
		public:
			//���캯��
			Supply(HttpSession *http_session, const string &pro_name);
			//������
			virtual ~Supply();
			//�ж��Ƿ�Ϊ����
			static int ProcessSupply(HttpSession *http_session ,const char *packet);
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
