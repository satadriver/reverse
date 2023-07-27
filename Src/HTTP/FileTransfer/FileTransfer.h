#ifndef HTTP_FILETRANSFER_H_H_H
#define HTTP_FILETRANSFER_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 

//�����ռ�
namespace filetransfer  
{
	//����
	class FileTransfer : public HttpPro
	{
	public:
		//���캯��
		FileTransfer(HttpSession *http_session, const string &pro_name);
		//������
		virtual ~FileTransfer();
		//�ж��Ƿ�Ϊ����
		static int ProcessFileTransfer(HttpSession *http_session ,const char *packet);
	//����������
	protected:
		string app_;						//��Դ
		string platformWeb;
		string platformPC;
		string platformAndroid;
		string platformIos;
		string platformAndPc;
    public:
		string longtitude_;	//����
		string latitude_;   //γ��
		string imei_;       //IMEI
		string ostype_;     //ϵͳ����
	};
}
#endif

//NEWCLASS
//newnamespace 
//NewClass