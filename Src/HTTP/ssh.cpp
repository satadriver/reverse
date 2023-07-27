// ssh.cpp: implementation of the ssh class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ssh.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL ssh_DealPacket(const char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl)
{
	//写对照文件
	CString txt;
	txt.Format("state=登陆\r\n");
	
	HttpSession tmpsession(packh, g_httpFrom);
	tmpsession.m_dceprocotol=httpprotocl;
	
	tmpsession.WriteSshFile(txt);

	return TRUE;
}

//定时清理SSHMAP
DWORD WINAPI CleanSshMap(LPVOID p)
{

	while (true)
	{
		POSITION pos = v_sshMap.GetStartPosition();
		CString rKey;
		CString tm;

		while(pos != NULL)
		{
			v_sshMap.GetNextAssoc(pos, rKey, tm);

			if(tm.GetLength()>0)
			{
				COleDateTime currtime = COleDateTime::GetCurrentTime();
				COleDateTime oldtime;
				oldtime.ParseDateTime(tm);
				COleDateTimeSpan resule = currtime - oldtime;
				if(resule.GetMinutes()>=10)
				{
					v_sshMap.RemoveKey(rKey);
				}
			}

		}
		Sleep(1000*60*10);
	}
	return 1;
}
