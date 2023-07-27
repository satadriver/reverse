#include "StdAfx.h"
#include "FileTransfer.h"
#include "FileTransferZapya.h"

filetransfer::FileTransfer::FileTransfer(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	platformWeb		= "��ҳ��";
	platformPC		= "pc��";
	platformAndroid	= "Android��";
	platformIos		= "Ios��";
	platformAndPc   = "Android�˻�pc��";
}

filetransfer::FileTransfer::~FileTransfer()
{
	
}

int filetransfer::FileTransfer::ProcessFileTransfer(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//����
		action = ZapyaFileTransfer::IsZapyaFileTransfer(http_session);
		if (action)
		{
			ZapyaFileTransfer zapyafiletransfer(http_session,"ZAPYA_TRANFILE");
			return zapyafiletransfer.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessFileTransfer Err : %d",n);
	}
	return 0;
}

//namespace
//FileTransfer