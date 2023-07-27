// AAA.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"
#include "ReadDDeFile.h"
#include "SocketSession.h"
#include "DceMission.h"
#include "AAA.h"

#define		AAAPRO		"dce1"

DceMission*	g_pAuthMissObj = NULL;
time_t		g_AuthPacketTime = 0;
DWORD		g_packetcount=0;


AAAProtocol::AAAProtocol()
{
	m_ddefiletype = "aaa";
}

AAAProtocol::~AAAProtocol()
{
	this->RenameTmpDir();
	delete[] m_pDseFileInfo;
}

void AAAProtocol::ProcessBuffer(DATABUFFER& packetbuf, time_t& packtm)
{
	try
	{
		int datapos=0;
		m_SpyAddr = packetbuf.posaddr;
		for (int i = 0; i < packetbuf.packetnum; i++)
		{
			char packettype = packetbuf.pBuffer[datapos];
			if (packettype == AAAPacket && g_processAAA==1)
			{
				// 处理认证
				//wangzaixue 3g 状态下不写文件了
				if (g_bAAA)
				{
					ProcessAAAPacket((NEWAAAPACKET*)(packetbuf.pBuffer+datapos));
					datapos=datapos+sizeof(NEWAAAPACKET);
					g_pAuthMissObj->m_packetnum++;
				}
			}
			else
				break;
		}
	}
	catch(...)
	{
		WriteLog(AAAPRO, "ProcessBuffer() Error!!");
	}
	return;
}

void AAAProtocol::WriteAuthFile(LPIpDataStruct pAAABuf, NEWAAAPACKET* pAAA, LPCTSTR pDDEPOS)
{
	static CTime tt(2000, 1, 1, 0, 0, 0);

	ASSERT(!pAAABuf->m_indexfname.IsEmpty());
	ASSERT(!pAAABuf->m_realfname.IsEmpty());
	ASSERT(pAAABuf->m_pindexfile != NULL);
	ASSERT(pAAABuf->m_prealfile != NULL);


	char clientip[20];
	memset(clientip,0,20);
	sprintf(clientip, "%03d.%03d.%03d.%03d", pAAA->m_ClientIp[0],
		pAAA->m_ClientIp[1], pAAA->m_ClientIp[2], pAAA->m_ClientIp[3]);

	char buffer[2048];
	int len = 0;
	// 写文件
	len += sprintf(buffer+len, "pro=aaa\r\nspyaddr=%s\r\n", v_ddelist.spyaddr);
	char strtm[40];
	GetTimeString(pAAA->m_tm, "%Y%m%d%H%M%S", strtm, sizeof(strtm)-4);
	if (pAAA->m_tm <= tt.GetTime())
	{
		WriteLog("dce1", "AAA Time(%d_%s) Error1!!", pAAA->m_tm, strtm);
	}
	len += sprintf(buffer+len, "tm=%s\r\n", (pAAA->m_tm<=0)? "N/A" : strtm);
	len += sprintf(buffer+len, "status=%d\r\n", pAAA->m_RecType);
	len += sprintf(buffer+len, "username=%s\r\n", pAAA->m_strUserName[0]==0 ? "N/A" : (char*)pAAA->m_strUserName);
	len += sprintf(buffer+len, "usercelllac=%s\r\n", pAAA->m_strUserCellLac[0]==0 ? "N/A" : (char*)pAAA->m_strUserCellLac);
	len += sprintf(buffer+len, "usercellci=%s\r\n", pAAA->m_strUserCellCi[0]==0 ? "N/A" : (char*)pAAA->m_strUserCellCi);
	len += sprintf(buffer+len, "userimei=%s\r\n", pAAA->m_strUserImei[0]==0 ? "N/A" : (char*)pAAA->m_strUserImei);
	len += sprintf(buffer+len, "telno=%s\r\n", pAAA->m_strPhone[0]==0 ? "N/A" : (char*)pAAA->m_strPhone);
	len += sprintf(buffer+len, "Nasip=%03d.%03d.%03d.%03d\r\n", pAAA->m_Nasip[0],
		pAAA->m_Nasip[1], pAAA->m_Nasip[2], pAAA->m_Nasip[3]);
	len += sprintf(buffer+len, "frameip=%03d.%03d.%03d.%03d\r\n", pAAA->m_ClientIp[0],
		pAAA->m_ClientIp[1], pAAA->m_ClientIp[2], pAAA->m_ClientIp[3]);
	len += sprintf(buffer+len, "delay=%d\r\n", pAAA->m_ldelay);
	len += sprintf(buffer+len, "Sessionid=%s\r\n", pAAA->m_SessionId[0]==0 ? "N/A" : (char*)pAAA->m_SessionId);
	if (v_programtype!=SYSTEM_SLEUTH)
	{
		len += sprintf(buffer+len, "datasrc= %s\r\n",	GetDataSrc(3));
	}
// 	if (aaa.CaseID.IsEmpty())
// 	{
		// 该协议暂时不处理案件
		len += sprintf(buffer+len, "AddrType=255\r\nCaseid=N/A\r\nWlevel=0\r\nObjectid=N/A\r\n");
// 	}
// 	else
// 	{
// 		fprintf(pAAABuf->authfile, "AddrType=%d\r\n", aaa.LimitType);
// 		fprintf(pAAABuf->authfile, "Caseid=%s\r\n", aaa.CaseID);
// 		fprintf(pAAABuf->authfile, "Wlevel=0\r\nObjectid=%s\r\n", aaa.ObjectID);
// 	}

	WarnlimitMap& warnlimitMap = v_warnlimitPara.m_warnlimitMap[v_CaseLimitPara.m_readflg]; //重点人员帐号
	Lpwarnkeyinfo lpwarnkeyinfo = NULL;
	//找出所有符合账号，ip，mac的所有重点人员来，放到m_WarnlimitMap中，匹配上关键字之后需要与这个map匹配
	//判断是否中上重点人
	//先处理帐号
	CString writeauthwarndata;
	int classno;
	CMapStringToString warnridmap;
	CString tmpmid;
	if (warnlimitMap.Lookup(clientip, lpwarnkeyinfo))
	{
		while (lpwarnkeyinfo != NULL)
		{
			if (warnridmap.Lookup(lpwarnkeyinfo->m_id,tmpmid))
			{
				;
			}
			else
			{
				writeauthwarndata.Format("%skeyobjid= %s\r\n",(CString)writeauthwarndata,lpwarnkeyinfo->m_id);
				CString strrule=getruletype("aaa",lpwarnkeyinfo,classno);
				CString classvalue;
				if(v_warnlimitPara.m_warnruleMap[v_CaseLimitPara.m_readflg].Lookup(strrule,classvalue))
				{
					CString bcprule;
					bcprule.Format("%s\1%s\1%d\1%s",lpwarnkeyinfo->m_id,lpwarnkeyinfo->m_secclass,classno,classvalue);
					WriteArrToData(AAA_PROTOCOL, KEYOBJ_ALARM, bcprule);
				}
				warnridmap.SetAt(lpwarnkeyinfo->m_id,lpwarnkeyinfo->m_id);
			}
			lpwarnkeyinfo = lpwarnkeyinfo->m_next;
		}
	}
	if (warnlimitMap.Lookup(pAAA->m_strUserName, lpwarnkeyinfo))
	{
		while (lpwarnkeyinfo != NULL)
		{

			lpwarnkeyinfo = lpwarnkeyinfo->m_next;
		}
	}

	len += sprintf(buffer+len, "%s",writeauthwarndata);
	len += sprintf(buffer+len, "LineTok=End\r\n\r\n");

	//wangzaixue 2014-04-22
	//将MAC地址信息提交到MAC地址库
	CString strTempPhone(pAAA->m_strPhone);
	strTempPhone.Replace("-","");
	
	CString strTempUserName(pAAA->m_strUserName);
	
	int nTempPos =strTempUserName.Find("\r\n");
	if (-1 != nTempPos)
	{
		strTempUserName = strTempUserName.Left(nTempPos);
	}
	
	WriteThreadMacIndexFile("",strTempPhone,"","",strTempUserName);


//	return ;
	DceFileStream StreamInfo;
	memset(&StreamInfo, 0, sizeof(StreamInfo));
	memcpy(StreamInfo.CaseIDorWarnID, "aaa", 4);
	memcpy(StreamInfo.Pro, "aaa", 4);
	StreamInfo.Type = 3;		// 认证数据
	g_pAuthMissObj->m_pSubPro->MergeMemDataToStream(pAAABuf, &StreamInfo,
		buffer, len, NULL);
	
	// 关闭流文件
	DWORD reallen = ftell(pAAABuf->m_prealfile);
	int re = fclose(pAAABuf->m_prealfile);
	if (re != 0)
	{
		WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pAAABuf->m_realfname, re);
	}
	pAAABuf->m_prealfile = NULL;
	re = fclose(pAAABuf->m_pindexfile);
	if (re != 0)
	{
		WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pAAABuf->m_indexfname, re);
	}
	pAAABuf->m_pindexfile = NULL;
	if (reallen >= 2*1024*1024)
	{
		// 提交流文件
		CString idxfname = pAAABuf->m_indexfname;
		CString ralfname = pAAABuf->m_realfname;
		idxfname.Replace("tmp20", "20");
		ralfname.Replace("tmp20", "20");
//		rename(pAAABuf->m_realfname, ralfname);
		int re = rename(pAAABuf->m_realfname, ralfname);
		if (re != 0)
		{
			::Sleep(200);
			re = rename(pAAABuf->m_realfname, ralfname);
			if (re != 0)
			{
				m_TmpFileArr.Add(pAAABuf->m_realfname);
			}
		}
		re = rename(pAAABuf->m_indexfname, idxfname);
//		re = rename(pAAABuf->m_realfname, idxfname);
		if (re != 0)
		{
			::Sleep(200);
			re = rename(pAAABuf->m_realfname, idxfname);
			if (re != 0)
			{
				m_TmpFileArr.Add(pAAABuf->m_realfname);
			}
		}
// 		DeleteFile(pAAABuf->m_realfname);
// 		DeleteFile(pAAABuf->m_indexfname);
		pAAABuf->m_realfname.Empty();
		pAAABuf->m_indexfname.Empty();
		pAAABuf->m_filenum=0;
		pAAABuf->m_realflen=0;
	}


}

void AAAProtocol::ProcessAAAPacket(NEWAAAPACKET* pAAAData)
{
	g_AuthPacketTime = pAAAData->m_tm;
	// 判断是否写到文件中
	if ((pAAAData->m_bWriteFile == 1) &&
		(pAAAData->m_RecType != 3))
	{
		// 优先使用SessionID以分配其次是客户端IP分配
		g_packetcount++;
		LPIpDataStruct pval = NULL;
		if (pAAAData->m_strUserName[0]!=0)
		{
			pval = AllocFileToDse(1, HashKey((LPCTSTR)pAAAData->m_strUserName));		// 按帐号分配
		}
		else
		{
			pval = AllocFileToDse(1, pAAAData->m_ClientIp[0]+pAAAData->m_ClientIp[1]
				+pAAAData->m_ClientIp[2]+pAAAData->m_ClientIp[3]);						// 按客户端IP
		}
		WriteAuthFile(pval, pAAAData, m_SpyAddr);
	}
	// 该认证方式不入哈希表了
}


BOOL AAAProtocol::InitObject(DceMission* pMiss)
{
	WriteLog(AAAPRO, "**************** DCE Module-AAA(ver:%s %s) Start Run!! ****************",
		__DATE__, __TIME__);

	g_pAuthMissObj = pMiss;
	if (pMiss->m_logfilename[0] == 0)
		pMiss->m_logfilename = AAAPRO;

	m_DseInfoCount = g_DseServer.m_count*3;
	m_pDseFileInfo = new IpDataStruct[m_DseInfoCount];
	for (int i=0; i<m_DseInfoCount; i++)
	{
		m_pDseFileInfo[i].m_filenum = 0;
		m_pDseFileInfo[i].m_realflen = 0;
		m_pDseFileInfo[i].m_pindexfile = NULL;
		m_pDseFileInfo[i].m_prealfile = NULL;
	}
	return TRUE;
}

BOOL AAAProtocol::WriteObjectState(time_t now, CString& strtxt)
{
// 	DWORD heapsize=0;
// 	DWORD emptysize=0;
// 	int   blockcount=0;
// 	if (g_AuthHeap!=NULL)
// 	{
// 		if(!GetHeapInfo(g_AuthHeap, heapsize, emptysize, blockcount))
// 			heapsize = emptysize = blockcount = 0;
// 	}
	char ptm[40];
	GetTimeString(g_AuthPacketTime, "%Y-%m-%d %H:%M:%S", ptm, 40);

	WriteLog(AAAPRO, "当前状态:packtm:%s  loop:%u  packnum:%u(%u)",
		ptm, g_pAuthMissObj->m_loopnum, g_pAuthMissObj->m_packetnum, g_packetcount);
	g_packetcount=0;
	return TRUE;
}

//写MAC数据信息
BOOL AAAProtocol::WriteThreadMacIndexFile(CString pcname,CString pcmac,CString pcip,CString pcostype,CString pcaccount)
{
	CString currenttm;
	SYSTEMTIME now;
	GetLocalTime(&now);

	currenttm.Format("%04d%02d%02d%02d%02d%02d",
		now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond);

	CString writecontent;
	//格式：mac\t接入点名称\t时间\t计算机名称\t计算机mac\t计算机ip\t计算机操作系统\t上网帐号\n
	//       0------1---------2--------3---------4-----------5----------6----------
	writecontent.Format("ADSL拨号\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
						v_ddelist.spyaddr,currenttm,pcname,pcmac,pcip,pcostype,pcaccount);


	CString threadmacpath;
	threadmacpath.Format("e:\\netspy\\basedata\\tmpmac");
	if(access(threadmacpath,0)!=0)
	{
		mkdir(threadmacpath);
	}

	//构建文件名
	CString savefilepath;
	savefilepath.Format("%s\\%s_%s_mac.tmp",threadmacpath,currenttm,v_ddelist.spyaddr);

	CFile cf;
	cf.Open(savefilepath,CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	cf.Write(writecontent,writecontent.GetLength());
	cf.Close();

	return TRUE;
}