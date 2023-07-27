#ifndef HTTP_FILETRANSFER_H_H_H
#define HTTP_FILETRANSFER_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 

//命名空间
namespace filetransfer  
{
	//类名
	class FileTransfer : public HttpPro
	{
	public:
		//构造函数
		FileTransfer(HttpSession *http_session, const string &pro_name);
		//虚析构
		virtual ~FileTransfer();
		//判断是否为此类
		static int ProcessFileTransfer(HttpSession *http_session ,const char *packet);
	//定义额外变量
	protected:
		string app_;						//来源
		string platformWeb;
		string platformPC;
		string platformAndroid;
		string platformIos;
		string platformAndPc;
    public:
		string longtitude_;	//经度
		string latitude_;   //纬度
		string imei_;       //IMEI
		string ostype_;     //系统类型
	};
}
#endif

//NEWCLASS
//newnamespace 
//NewClass