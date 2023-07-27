#include "StdAfx.h"
#include "FileTransfer.h"
#include "FileTransferZapya.h"

filetransfer::FileTransfer::FileTransfer(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	platformWeb		= "ÍøÒ³¶Ë";
	platformPC		= "pc¶Ë";
	platformAndroid	= "Android¶Ë";
	platformIos		= "Ios¶Ë";
	platformAndPc   = "Android¶Ë»òpc¶Ë";
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
		//¿ìÑÀ
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