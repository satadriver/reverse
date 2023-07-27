
#include "stdafx.h"
#include "DcePub.h"
#include "ProMission.h"

#define		MAX_MARGE_NUM		100		// 文件的最大整合度
#define		SPACEMARK			"\r\n\r\nguid=AB7AA4CB_451A_4dc8_A78A_F1D2C788C02D_8CCE92F2_5E9A_439c_A29B_51A9210B1315_194B7FB8_5B07_4bd8_A139_527C062C5AA4\r\n\r\n"



DceProcotol::DceProcotol()
{
	m_ddefiletype	= NULL;				// 处理dde文件的类型名称
//	m_outdirlist	= NULL;				// 数据输出的路径列表
	m_HashSize		= 0;				// Socket哈希表的宽度
	m_PacketFrom	= 0;				// 当前处理包的类型
	m_pDseFileInfo	= NULL;
	m_DseInfoCount	 = 0;
//	m_DseFileMap.InitHashTable(5009);	// 整合用的哈希表
}

DceProcotol::~DceProcotol()
{
//	m_outdirlist = NULL;
	if (m_pDseFileInfo != NULL)
	{
		delete[] m_pDseFileInfo;
	}
}

BOOL DceProcotol::InitObject(DceMission* pMiss) {return TRUE;}

void DceProcotol::ProcessBuffer(DATABUFFER& packetbuf, time_t& packtm) {}

// 整合文件到流
BOOL DceProcotol::MergeFileToStream(LPIpDataStruct pDstFile,	// 目的文件信息结构
				DceFileStream* pDceStream,
				LPCTSTR srcfname,		// 原文件
				LPCTSTR	strnum			// 文件名的替换串
				)
{
	ASSERT(!pDstFile->m_indexfname.IsEmpty());
	ASSERT(!pDstFile->m_realfname.IsEmpty());
	ASSERT(pDstFile->m_pindexfile != NULL);
	ASSERT(pDstFile->m_prealfile != NULL);
	ASSERT(pDceStream != NULL);
	ASSERT(srcfname != NULL);
	ASSERT(srcfname[0] != 0);

	pDceStream->BeginPOS = ftell(pDstFile->m_prealfile);
	if (pDceStream->BeginPOS != pDstFile->m_realflen)
	{
		WriteLog("dce", "输出文件<%s>时 流实体文件长度错误%d_%d",
			srcfname, pDceStream->BeginPOS, pDstFile->m_realflen);
		return FALSE;
	}
	CFile file;
	if (file.Open(srcfname, CFile::modeRead))
	{
		// 整合文件
		pDceStream->FileLen = 0;
		DWORD flen = file.GetLength();
		char buffer[8192];
		while(flen > 0)
		{
			int rlen = min(flen, sizeof(buffer));
			if (rlen != file.Read(buffer, rlen))
			{
				break;
			}
			DWORD wlen = fwrite(buffer, 1, rlen, pDstFile->m_prealfile);
			flen -= rlen;
			pDceStream->FileLen += wlen;
		}
		file.Close();
		if (flen > 0)
		{
			// 文件读取失败
			return FALSE;
		}
		pDstFile->m_realflen += pDceStream->FileLen;

		// 写文件名
		CString tmpname = GetShortName(srcfname);
		if (strnum != NULL)
		{
			tmpname.Replace("_1_", strnum);
		}
		ASSERT(tmpname.GetLength() <= sizeof(pDceStream->Filename));
		memset(pDceStream->Filename, 0, sizeof(pDceStream->Filename));
		memcpy(pDceStream->Filename, tmpname, tmpname.GetLength());
		DWORD idxflen = ftell(pDstFile->m_pindexfile);
		if (idxflen != pDstFile->m_filenum*sizeof(DceFileStream))
		{
			WriteLog("dce", "输出文件<%s>时 流对照文件长度错误%d_%d",
				srcfname, idxflen, pDstFile->m_filenum*sizeof(DceFileStream));
			return FALSE;
		}
		pDstFile->m_filenum++;
		fwrite(pDceStream, sizeof(DceFileStream), 1, pDstFile->m_pindexfile);
		return TRUE;
	}
	else
	{
		// 写日志
	}
	return FALSE;
}

// 整合内存数据到流
BOOL DceProcotol::MergeMemDataToStream(LPIpDataStruct pDstFile,	// 目的文件信息结构
				DceFileStream* pDceStream,
				LPCTSTR	pfdata,						// 内存数据(srcfname=NULL时有效)
				int		fdatalen,					// 内存数据长度
				LPCTSTR dstfname					// 写到流里的文件名
				)
{
	ASSERT(!pDstFile->m_indexfname.IsEmpty());
	ASSERT(pDstFile->m_pindexfile != NULL);
	ASSERT(!pDstFile->m_realfname.IsEmpty());
	ASSERT(pDstFile->m_prealfile != NULL);
	ASSERT(pDceStream != NULL);
	pDceStream->BeginPOS = ftell(pDstFile->m_prealfile);
	if (pDceStream->BeginPOS != pDstFile->m_realflen)
	{
		WriteLog("dce", "输出文件<%s>时 流实体文件长度错误%d_%d",
			pDstFile->m_realfname, pDceStream->BeginPOS, pDstFile->m_realflen);
		return FALSE;
	}
	if (fdatalen > 0)
	{
		ASSERT(pfdata != NULL);
		DWORD wlen = fwrite(pfdata, 1, fdatalen, pDstFile->m_prealfile);
		pDstFile->m_realflen += wlen;
		pDceStream->FileLen = wlen;
		memset(pDceStream->Filename, 0, sizeof(pDceStream->Filename));
		if (dstfname != NULL)
		{
			// 写文件名
			strcpy(pDceStream->Filename, dstfname);
		}
		DWORD idxflen = ftell(pDstFile->m_pindexfile);
		if (idxflen != pDstFile->m_filenum*sizeof(DceFileStream))
		{
			WriteLog("dce", "输出文件<%s>时 流对照文件长度错误%d_%d",
				pDstFile->m_indexfname, idxflen, pDstFile->m_filenum*sizeof(DceFileStream));
			return FALSE;
		}
		pDstFile->m_filenum++;
		fwrite(pDceStream, sizeof(DceFileStream), 1, pDstFile->m_pindexfile);
	}
	return TRUE;
}

LPIpDataStruct DceProcotol::AllocFileToDse(UINT DataType,	// 数据类型(0回溯, 1案件 2预警)
				UINT KeyHash					// 数据哈希
				)
{
	static long fnum = 1;
// 	if (DataType > 2)
// 	{
// 		DataType = 1;
// 	}
	int svrid = 0;
	LPCTSTR DseIp = "";
	LPIpDataStruct pVal = NULL;
	if(DataType == 3 || DataType == 4)
	{
		pVal = m_pDseFileInfo;
		if (pVal->m_indexfname.IsEmpty())
		{
			if (g_DseServer.m_count > 1)
			{
				UINT hash = GetTickCount(); //(rand()<<13) + rand();
				DseIp = g_DseServer.AllocateData(hash, &svrid);
			}
			else
			{
				DseIp = g_DseServer.AllocateData(0, &svrid);
			}
		}
	}
	else
	{
		ASSERT(0 == (m_DseInfoCount % 3));
	 	ASSERT(m_pDseFileInfo != NULL);
		ASSERT(DataType<3);		// 0/1/2
		if (m_DseInfoCount != 3)
		{
			ASSERT(m_DseInfoCount == g_DseServer.m_count * 3);
			// 分配数据
			CString str;
			DseIp = g_DseServer.AllocateData(KeyHash, &svrid);
			pVal = m_pDseFileInfo + (svrid*3) + DataType;
		}
		else
		{
			// 随机分配
			pVal = m_pDseFileInfo + DataType;
			if (pVal->m_indexfname.IsEmpty())
			{
				if (g_DseServer.m_count > 1)
				{
					UINT hash = GetTickCount(); //(rand()<<13) + rand();
					DseIp = g_DseServer.AllocateData(hash, &svrid);
	// 				if ((DataType == 0) && (strcmp(m_ddefiletype, "http") == 0))
	// 				{
	// 					WriteLog("dce", "File Hash = %d, DSeIP = %s", hash, DseIp);
	// 				}
				}
				else
				{
					DseIp = g_DseServer.AllocateData(0, &svrid);
				}
			}
		}
	}
	ASSERT(pVal != NULL);
	if (pVal->m_indexfname.IsEmpty())
	{
		char curtm[40];
		GetTimeString(time(0), "%Y%m%d%H%M%S", curtm, 36);
		long fid = InterlockedIncrement(&fnum);
		CString dir;
		switch(DataType)
		{
		case 0:		// 回溯数据
			dir.Format("%s\\IndexServer\\%s\\", v_datapath, DseIp);
			break;
		case 1:		// 案件数据
			dir.Format("%s\\CaseServer\\%s\\", v_datapath, DseIp);
			break;
		case 2:		// 预警数据
			dir.Format("%s\\keyobjServer\\%s\\", v_datapath, DseIp);
			break;
		case 3:		// 预警数据
			dir.Format("%s\\taobao\\", v_StatPath);
			break;
		case 4:		// 预警数据
			dir.Format("%s\\qqpass\\", v_StatPath);
			break;
		}
		CreateDir(dir);
		pVal->m_indexfname.Format("%stmp%s_%d_%s_Combo", dir,
			curtm, fid%10000, m_ddefiletype);
		pVal->m_realfname = pVal->m_indexfname + ".file";
		pVal->m_indexfname += ".idx";
		DeleteFile(pVal->m_indexfname);
		DeleteFile(pVal->m_realfname);
		pVal->m_filenum=0;
		pVal->m_realflen=0;
	}
	ASSERT(!pVal->m_realfname.IsEmpty());
	if (pVal->m_prealfile == NULL)
	{
		pVal->m_prealfile = fopen(pVal->m_realfname, "a+b");
	}
	if (pVal->m_pindexfile == NULL)
	{
		pVal->m_pindexfile = fopen(pVal->m_indexfname, "a+b");
	}
	fseek(pVal->m_pindexfile, 0, SEEK_END);
	fseek(pVal->m_prealfile, 0, SEEK_END);
	ASSERT(pVal->m_prealfile != NULL);
	ASSERT(pVal->m_pindexfile != NULL);
	return pVal;
}

void DceProcotol::OutPutFileToStream(LPCTSTR pro,
					UINT type,				// 类型(0: 回溯，1: 案件，2:预警，其他值错误)
					LPCTSTR CIDWType,		// 案件ID(type==1)或预警分类(type==2)或空串(type==0)
					CString indexfname,
					CString realfname,
					CStringArray& attfiles,
					LPCTSTR filekey,		// 数据分配时的KEY(此时的KEY不包含pro和CIDWType)
//					CString AlarmText,		// 该数据的告警文本
					UINT	id				// 该数据输出的次数(一条数据匹配了多个线索)
					)
{
//******************************************************
// 文件的整合和分配策略
// 	Smtp、Pop3
// 		以邮件实体的CRC32的值分配
// 	此时filekey应该传邮件实体的CRC32的值
// 
// 	其他协议
// 		随机分配
//******************************************************
	// 首先根据协议选择不同的分配方式 （即确定pval的值）
	UINT hashval = 0;
	BOOL bOnLine;
	LPIpDataStruct pval = NULL;
	if (stricmp(pro, "smtp") == 0)
	{
		bOnLine = ((g_DseServer.m_count <= 30)		// DSE的数量<=30
			|| ((g_DseServer.m_count <= 100) && (type = 0)));	// 只有SMTP回溯的流文件保持打开状态
		pval = AllocFileToDse(type & 0xF, HashKey(filekey));
	}
	else if (stricmp(pro, "pop3") == 0)
	{
		// 以CRC分配
		bOnLine = (g_DseServer.m_count <= 20);			
		if (filekey[0] != 0)
		{
			pval = AllocFileToDse(type & 0xF, HashKey(filekey));
		}
		else
		{
			pval = AllocFileToDse(type & 0xF, rand());	// 无实体的随机分配
		}
	}
	else
	{
#ifdef _DEBUG
		if(strstr(pro, "media") != NULL)
		{
			// 调试用, 备份整合前的media, 可以直接播放器看
			if(_access("e:\\netspy\\flg\\testmedia.flg", 0) == 0 )
			{
				CString oname;
				if(_access(realfname, 0) == 0)
				{
					oname = realfname.Mid(realfname.ReverseFind('\\')+1);
					CopyFile(realfname, CString("d:\\")+oname, FALSE);
				}
			}
		}
#endif

		// 随机分配
		pval = AllocFileToDse(type & 0xF, 0);
		bOnLine = TRUE;
	}

	// 形成流文件
	DceFileStream	IdxFileStream;
	memset(&IdxFileStream, 0, sizeof(IdxFileStream));
	IdxFileStream.Type = type;
	strcpy(IdxFileStream.Pro, pro);
	strcpy(IdxFileStream.CaseIDorWarnID, CIDWType);

	CString strnum;
	LPCTSTR pstrnum = NULL;
	if (id > 1)
	{
		strnum.Format("_%d_", id);
		pstrnum = strnum;
		indexfname.Replace("_1_", strnum);
	}

	BOOL MergeOK = TRUE;
	// 对照
	MergeOK = MergeMemDataToStream(pval, &IdxFileStream,
		m_IndexFileData, m_IndexFileData.GetLength(), indexfname);
	
	// 实体
	if ((MergeOK) && (!realfname.IsEmpty()))
	{
		MergeOK = MergeFileToStream(pval, &IdxFileStream, realfname, pstrnum);
	}

	if (MergeOK)
	{
		// 附件
		for (int i=0; i<attfiles.GetSize(); i++)
		{
			CString attfname = attfiles[i];
			if (_access(attfname, 0) == 0)
			{
				if (!MergeFileToStream(pval, &IdxFileStream, attfname, /*pstrnum对照中的附件名要与这个一直，所以不再改名*/NULL))
				{
					MergeOK = FALSE;
					break;
				}
			}
		}
	}

	// 告警
// 	if ((MergeOK) && (!AlarmText.IsEmpty()))
// 	{
// 		IdxFileStream.Type = 4;		// 告警数据
// 		MergeOK = MergeMemDataToStream(pval, &IdxFileStream, AlarmText, AlarmText.GetLength(), NULL);
// 	}

	// 判断流文件大小, >2MByte 就提交
	DWORD reallen = ftell(pval->m_prealfile);
	if ((reallen >= 2*1024*1024) || (!bOnLine) || (!MergeOK))
	{
		// 关闭流文件
		int re = fclose(pval->m_pindexfile);
		if (re != 0)
		{
			WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_indexfname, re);
		}
		pval->m_pindexfile = NULL;
		re = fclose(pval->m_prealfile);
		if (re != 0)
		{
			WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_realfname, re);
		}
		pval->m_prealfile = NULL;
		if ((reallen >= 2*1024*1024) || (!MergeOK))
		{
			// 提交流文件
			CString idxfname = pval->m_indexfname;
			CString ralfname = pval->m_realfname;
			idxfname.Replace("tmp20", "20");
			ralfname.Replace("tmp20", "20");
			if (_access(v_WorkPath+"\\flg\\bakdcedata.flg",0) == 0)
			{
				if (_access("e:\\netspy\\bakdcedata",0)!=0)
				{
					_mkdir("e:\\netspy\\bakdcedata");
				}
				CString tmpfile;
				tmpfile.Format("e:\\netspy\\bakdcedata\\%s", pval->m_realfname.Mid(pval->m_realfname.ReverseFind('\\')));
				CopyFile(pval->m_realfname,tmpfile,TRUE);
				tmpfile.Format("e:\\netspy\\bakdcedata\\%s", pval->m_indexfname.Mid(pval->m_indexfname.ReverseFind('\\')));
				CopyFile(pval->m_indexfname,tmpfile,TRUE);

			}
//			rename(pval->m_realfname, ralfname);
			int re = rename(pval->m_realfname, ralfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_realfname, ralfname);
				if (re != 0)
				{
					m_TmpFileArr.Add(pval->m_realfname);
				}
			}
			
//			rename(pval->m_indexfname, idxfname);
			re = rename(pval->m_indexfname, idxfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_indexfname, idxfname);
				if (re != 0)
				{
					m_TmpFileArr.Add(pval->m_indexfname);
				}
			}
// 			DeleteFile(pval->m_realfname);
// 			DeleteFile(pval->m_indexfname);
			pval->m_realfname.Empty();
			pval->m_indexfname.Empty();
			pval->m_filenum=0;
			pval->m_realflen=0;
		}
	}
	else
	{
		if (pval->m_prealfile != NULL)
		{
			fflush(pval->m_prealfile);
		}
		if (pval->m_pindexfile != NULL)
		{
			fflush(pval->m_pindexfile);
		}
	}
}

void DceProcotol::OutPutFile(LPCTSTR pro, UINT type, LPCTSTR CIDWType,
				CString indexfname, CString realfname, CStringArray& attfiles,
				LPCTSTR filekey, CString AlarmText)
{
//******************************************************
// 文件的整合和分配策略
// 	Smtp、Pop3(侦控和密取一致)
// 		案件: 不整合，以案件ID+邮件实体的CRC32的值分配
// 		预警: 不整合，以预警分类+邮件实体的CRC32的值分配
// 		回溯: 不整合，以邮件实体的CRC32的值分配
// 	此时filekey应该传邮件实体的CRC32的值
// 
// 	WebMail、Media、Unip(侦控和密取一致)
// 		案件: 不整合，随机分配
// 		预警: 不整合，随机分配
// 		回溯: 不整合，随机分配
// 	此时filekey无意义
// 
// 	侦控的其他协议(只整合txt文件)
// 		案件: 以filekey+协议+案件ID整合，随机分配
// 		预警: 以filekey+协议+预警分类整合，随机分配
// 		回溯: 以filekey+协议整合，随机分配
// 	此时filekey的取值范围:
// 		filekey == "" 表示该数据只有对照文件
// 		filekey == "txt" 表示该数据需要整合
// 		否则表示该数据不需要整合
// 		
// 	密取的其他协议
// 		案件: 以filekey+客户端IP+协议+案件ID整合，随机分配
// 		回溯: 以filekey+客户端IP+协议整合，随机分配
// 	此时filekey的取值范围:
// 		filekey == "" 表示该数据只有对照文件
// 		filekey == "txt" 表示该数据需要整合
// 		否则表示该数据不需要整合
//******************************************************
		
//******************************************************
//	告警数据的输出
//  1. 告警数据存放在Alarm.txt文件中
//	2. 一个告警文件(Alarm.txt)可以包含多条告警数据
//	3. 每条告警数据所在的文件(Alarm.txt)要和其对照文件在同一个目录下
//	3. 多条告警文本之间采用GUID串分割(即SPACEMARK宏)
//******************************************************
	ASSERT(FALSE);
	// 首先判断协议
// 	CString hashkey;
// 	UINT hashval = 0;
// 	BOOL Output = FALSE;
// 	if (  (stricmp(pro, "smtp") == 0)
// 		||(stricmp(pro, "pop3") == 0)
// 		||(strnicmp(pro, "httpmail", 8) == 0))
// 	{
// 		hashkey.Format("%s_%s", CIDWType, filekey);
// 		hashval = HashKey(hashkey);	// 以CRC分配
// 		Output = TRUE;
// 	}
// 	else if ( (stricmp(pro, "webmail") == 0)
// 			||(stricmp(pro, "media") == 0)
// 			||(stricmp(pro, "unip") == 0))
// 	{
// 		hashval = rand();	// 随机分配
// 		Output = TRUE;
// 	}
// 	else
// 	{
// 		hashval = rand();	// 随机分配
// 		if ((strcmp(filekey, "") == 0) || (stricmp(filekey, "txt") == 0))
// 		{
// 			hashkey.Format("%s_%s_%s", pro, CIDWType, filekey);
// 		}
// 		else
// 		{
// 			Output = TRUE;
// 		}
// 	}
// 
// 	if (Output)
// 	{
// 		// 输出文件
// 		CString dstdir = GetOutDir(pro, type, CIDWType, hashval);
// 		if (!dstdir.IsEmpty())
// 		{
// 			CopyFileToServer(indexfname, realfname, attfiles,
// 				dstdir, (type == 0)? TRUE : FALSE);		// 是回溯就删除原文件
// 			if (!AlarmText.IsEmpty())
// 			{
// 				OutPutAlarm(dstdir, AlarmText);
// 			}
// 		}		
// 		else
// 		{
// 			// 丢弃该数据 写日志
// 		}
// 	}
// 	else
// 	{
// 		// 整合文件（整合的数据中不能有附件）
// 		LPIpDataStruct pval = NULL;
// 		if (!m_DseFileMap.LookupEx(hashkey, pval))
// 		{
// 			CString dstdir = GetOutDir(pro, type, CIDWType, hashval);
// 			if (!dstdir.IsEmpty())
// 			{
// 				IpDataStruct dsefinfo;
// 				dsefinfo.m_num = 1;
// 				dsefinfo.m_pindexfile = NULL;
// 				dsefinfo.m_prealfile = NULL;
// 				dsefinfo.m_indexfname.Format("%s%s", dstdir,
// 					GetShortName(indexfname));
// 				if (strcmp(realfname, "") != 0)
// 				{
// 					dsefinfo.m_realfname.Format("%s%s", dstdir,
// 						GetShortName(realfname));
// 				}
// 				if (!MergeFile(&dsefinfo, indexfname, realfname))
// 				{
// 					CopyFileToServer(indexfname, realfname, attfiles,
// 						dstdir, (type == 0)? TRUE : FALSE);		// 是回溯就删除原文件
// 					if (!AlarmText.IsEmpty())
// 					{
// 						OutPutAlarm(dstdir, AlarmText);
// 					}
// 				}
// 				else
// 				{
// 					if (!AlarmText.IsEmpty())
// 					{
// 						OutPutAlarm(dstdir, AlarmText);
// 					}
//  					m_DseFileMap.SetAt(hashkey, dsefinfo);
// 				}
// 			}
// 			else
// 			{
// 				// 丢弃该数据 写日志
// 			} // end if (!dstdir.IsEmpty())
// 		}
// 		else
// 		{
// 			ASSERT(pval != NULL);
// 			if (!MergeFile(pval, indexfname, realfname))
// 			{
// 				CString dstdir = GetOutDir(pro, type, CIDWType, hashval);
// 				if (!dstdir.IsEmpty())
// 				{
// 					CopyFileToServer(indexfname, realfname, attfiles,
// 						dstdir, (type == 0)? TRUE : FALSE);		// 是回溯就删除原文件
// 					if (!AlarmText.IsEmpty())
// 					{
// 						OutPutAlarm(dstdir, AlarmText);
// 					}
// 				}
// 				else
// 				{
// 					// 丢弃该数据 写日志
// 				} // end if (!dstdir.IsEmpty())
// 			}
// 			else
// 			{
// 				if (!AlarmText.IsEmpty())
// 				{
// 					OutPutAlarm(GetPath(pval->m_indexfname), AlarmText);
// 				}
// 
// 				if (pval->m_num > MAX_MARGE_NUM)
// 				{
// 					// 整合达到最大数
// 					m_DseFileMap.RemoveKey(hashkey);
// 				}
// 			}
// 		}
// 	}// end if (Output)
}


BOOL DceProcotol::MergeFile(LPIpDataStruct pDstFile,	// 目的文件信息结构
				LPCTSTR indexfname,		// 对照文件
				LPCTSTR realfname		// 实体文件
				)
{
// 	ASSERT(pDstFile != NULL);
// 	BOOL bMergeReal;
// 	if (strcmp(pDstFile->m_realfname, "") == 0)
// 	{
// 		bMergeReal = FALSE;
// 	}
// 	else
// 	{
// 		bMergeReal = TRUE;
// 	}
// 
// 	if (((strcmp(realfname, "") == 0) && bMergeReal)
// 		|| ((strcmp(realfname, "") != 0) && !bMergeReal))
// 	{
// 		// 带实体的数据和不带实体的数据不能整合到一起
// 		return FALSE;
// 	}
// 
// 	// 打开目的对照文件
// 	if (pDstFile->m_pindexfile == NULL)
// 	{
// 		ASSERT(!pDstFile->m_indexfname.IsEmpty());
// 		pDstFile->m_pindexfile = fopen(pDstFile->m_indexfname, "ab");
// 		if (pDstFile->m_pindexfile == NULL)
// 		{
// 			return FALSE;			//  打开目的对照文件失败
// 		}
// 	}
// 
// 	// 打开目的实体文件
// 	if (bMergeReal)
// 	{
// 		if (pDstFile->m_prealfile == NULL)
// 		{
// 			pDstFile->m_prealfile = fopen(pDstFile->m_realfname, "ab");
// 			if (pDstFile->m_prealfile == NULL)
// 			{
// 				return FALSE;			//  打开目的实体文件失败
// 			}
// 		}
// 	}
// 
// 	// 读取源对照文件
// 	CFile file;
// 	CString txt1, txt2;
// 	char buf1[4096], buf2[4096];
// 	char* p1 = buf1;
// 	char* p2 = buf2;
// 	int len1 = 0;
// 	int len2 = 0;
// 	if (file.Open(indexfname, CFile::modeRead))
// 	{
// 		len1 = file.GetLength();
// 		if (len1 > (sizeof(buf1) - 32))
// 		{
// 			p1 = txt1.GetBufferSetLength(len1+32);
// 		}
// 		memset(p1, 0, len1+32);
// 		file.Read(p1+24, len1);		// p1+24是要让出新片段号的位置
// 		file.Close();
// 		// 写片段号
// 		char buf[12];
// 		int tlen = sprintf(buf, "[%d]", pDstFile->m_num);
// 		p1 = p1 + 24 + 3 - tlen;
// 		memcpy(p1, buf, tlen);
// 		len1 = len1 - 3 + tlen;
// 	}
// 	else
// 	{
// 		return FALSE;		// 打开源对照文件失败
// 	}
// 
// 	// 读取源实体文件
// 	if (bMergeReal)
// 	{
// 		if (file.Open(realfname, CFile::modeRead))
// 		{
// 			len2 = file.GetLength();
// 			if (len2 > (sizeof(buf2) - sizeof(SPACEMARK) - 8))
// 			{
// 				p2 = txt1.GetBufferSetLength(len2 + sizeof(SPACEMARK) + 8);
// 			}
// 			memset(p2, 0, len2 + sizeof(SPACEMARK) + 8);
// 			file.Read(p2 + sizeof(SPACEMARK) - 1, len2);	// 让出写GUID的位置
// 			file.Close();
// 		}
// 		else
// 		{
// 			return FALSE;		// 打开源实体文件失败
// 		}
// 		// 写GUID
// 		if (pDstFile->m_num > 1)
// 		{
// 			memcpy(p2, SPACEMARK, sizeof(SPACEMARK)-1);
// 			len2 += sizeof(SPACEMARK)-1;
// 		}
// 		else
// 		{
// 			p2 += sizeof(SPACEMARK)-1;
// 		}
// 	}
// 
// 	// 写目的文件
// 	if (bMergeReal)
// 	{
// 		fwrite(p2, 1, len2, pDstFile->m_prealfile);
// 		fflush(pDstFile->m_prealfile);
// 	}
// 	fwrite(p1, 1, len1, pDstFile->m_pindexfile);
// 	fflush(pDstFile->m_pindexfile);
// 	pDstFile->m_num++;
	return TRUE;
}

// CString DceProcotol::GetOutDir(LPCTSTR pro, UINT type, LPCTSTR CIDWType, UINT hashval, 
// 							   int* pSvrIdx)
// {
// 	CString outdir;
// 	CString str;
// 	// 分配数据
// 	LPCTSTR DseIp = g_DseServer.AllocateData(hashval, pSvrIdx);
// 	switch(type)
// 	{
// 	case 0:		// 回溯数据
// 		str.Format("IndexServer\\%s\\%s", DseIp, pro);
// 		break;
// 	case 1:		// 案件数据
// 		str.Format("CaseServer\\%s\\%s", DseIp, pro);
// 		break;
// 	case 2:		// 预警数据
// 		str.Format("WarnServer\\%s\\%s", DseIp, pro);
// 		break;
// 	default:	// 非法
// 		return outdir;		// return "";
// 		break;
// 	}
// 
// 	char dir[512];
// 	if (strcmp(pro, "media") != 0)
// 	{
// 		// 检查Z盘
// 		int zFreeSpace = 0;
// 		if (CheckZDrive())
// 		{
// 			// Z盘可用
// 			int curdirtm = m_nowstm;
// 			for (int i=0; i<60; i++)
// 			{
// 				int len = sprintf(dir, "v:\\netspy\\%s\\%02dwt.flg", str, curdirtm);
// 				len -= 6;
// 				if (_access(dir, 0) == 0)			// 检查wt.flg
// 				{
// 					if (type != 0)
// 					{
// 						sprintf(dir + len, "\\%s\\", CIDWType);
// 					}
// 					else
// 					{
// 						strcpy(dir + len, "\\");
// 					}
// 					CreateDir(dir);
// 					return dir;
// 				}
// 				else
// 				{
// 					dir[len] = 0;
// 					CreateDir(dir);
// 					strcpy(dir + len, "ok.flg");
// 					if (_access(dir, 0) != 0)		// 检查ok.flg
// 					{
// 						strcpy(dir + len, "wt.flg");
// 						if (CreateFlgFile(dir))			// 创建wt.flg
// 						{
// 							// 创建wt.flg成功
// 							if (type != 0)
// 							{
// 								sprintf(dir + len, "\\%s\\", CIDWType);
// 							}
// 							else
// 							{
// 								strcpy(dir + len, "\\");
// 							}
// 							CreateDir(dir);
// 							return dir;
// 						}
// 					}
// 				} // end if (_access(dir, 0) == 0)
// 				curdirtm = (++curdirtm) % 60;
// 			} // end for
// 		} // end if (GetZFreeDisk(zFreeSpace) > 20)
// 	}
// 
// 	// 检查D盘
// 	int curdirtm = m_nowstm;
// 	for (int j=0; j<60; j++)
// 	{
// 		int len = sprintf(dir, "x:\\xxx\\%s\\%02dwt.flg", str, curdirtm);
// 		len -= 6;
// 		if (_access(dir, 0) == 0)			// 检查wt.flg
// 		{
// 			if (type != 0)
// 			{
// 				sprintf(dir + len, "\\%s\\");
// 			}
// 			else
// 			{
// 				strcpy(dir + len, "\\");
// 			}
// 			CreateDir(dir);
// 			return dir;
// 		}
// 		else
// 		{
// 			dir[len] = 0;
// 			CreateDir(dir);
// 			strcpy(dir + len, "ok.flg");
// 			if (_access(dir, 0) != 0)		// 检查ok.flg
// 			{
// 				strcpy(dir + len, "wt.flg");
// 				if (CreateFlgFile(dir))			// 创建wt.flg
// 				{
// 					// 创建wt.flg成功
// 					if (type != 0)
// 					{
// 						sprintf(dir + len, "\\%s\\", CIDWType);
// 					}
// 					else
// 					{
// 						strcpy(dir + len, "\\");
// 					}
// 					CreateDir(dir);
// 					return dir;
// 				}
// 			}
// 		} // end if (_access(dir, 0) == 0)
// 		curdirtm = (++curdirtm) % 60;
// 	} // end for
// 	return outdir;	// return "";
// }

void DceProcotol::OutPutAlarm(CString DstDir,		// 目的路径
					CString AlarmText				// 告警文本
					)
{
	ASSERT(!DstDir.IsEmpty());
	ASSERT(!AlarmText.IsEmpty());
	ASSERT(DstDir[DstDir.GetLength()-1] == '\\');
	CString alarmfile;
	alarmfile = DstDir + "Alarm.txt";
	FILE* fp = fopen(alarmfile, "ab");
	if (fp != NULL)
	{
		if (_filelength(fp->_file) > 0)
		{
			// 如果文件不为空就先写GUID
			fwrite(SPACEMARK, sizeof(SPACEMARK)-1, 1, fp);
		}
		fwrite((LPCTSTR)AlarmText, AlarmText.GetLength(), 1, fp);
		fclose(fp);
		fp = NULL;
	}
}

void DceProcotol::CopyFileToServer(LPCTSTR indexfname, LPCTSTR realfname,
			CStringArray& attfiles, CString DstDir, BOOL isDelSrcFile)
{
	static long filetotal = 1;

	int arraycount=attfiles.GetUpperBound();
	CString attfile;
	ASSERT(DstDir[DstDir.GetLength()-1] == '\\');
	CreateDir(DstDir);
	BOOL bReplace = FALSE;
	long filenum;
	char buf[20];
	memset(buf, 0, sizeof(buf));
	// 首先判断对照文件是否存在
	if((indexfname[0] != 0) && (_access(indexfname,0) == 0))
	{
		CString tmpfile = GetShortName(indexfname);
		CString DstIndexFile = DstDir + tmpfile;
		if (_access(DstIndexFile, 0) == 0)
		{
			// 文件名冲突
			bReplace = TRUE;
			filenum = InterlockedIncrement(&filetotal);
			if (filenum > 999900)
			{
				InterlockedExchange(&filetotal, 1);
			}
			sprintf(buf, "_%d_", filenum);
			tmpfile.Replace("_1_", buf);
			DstIndexFile = DstDir + tmpfile;
		}

		// 提交实体文件
		if((realfname[0] != 0) && (_access(realfname,0) == 0))
		{
			CString DstFile;
			if (bReplace)
			{
				DstFile = GetShortName(realfname);
				DstFile.Replace("_1_", buf);
				::WritePrivateProfileString("1", "filename", DstFile, indexfname);
				DstFile = DstDir + DstFile;
			}
			else
			{
				DstFile = DstDir + GetShortName(realfname);
			}
			if (isDelSrcFile)
			{
				MoveFileEx(realfname, DstFile,
					MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING |MOVEFILE_WRITE_THROUGH);
			}
			else
			{
				CopyFile(realfname, DstFile, FALSE);
			}
		}

		// 提交附件
		for(int i=0; i<=arraycount; i++)
		{
			attfile=attfiles.GetAt(i);
			if((!attfile.IsEmpty()) && (_access(attfile, 0)==0))
			{
				CString DstFile;
				if (bReplace)
				{
					char attfid[20];
					sprintf(attfid, "attfile%d", i+1);
					DstFile = GetShortName(attfile);
					DstFile.Replace("_1_", buf);
					::WritePrivateProfileString("1", attfid, DstFile, indexfname);
					DstFile = DstDir + DstFile;
				}
				else
				{
					DstFile = DstDir + GetShortName(attfile);
				}
				if (isDelSrcFile)
				{
					MoveFileEx(attfile, DstFile,
						MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING |MOVEFILE_WRITE_THROUGH);
				}
				else
				{
					CopyFile(attfile, DstFile, FALSE);
				}
			}
		}

		// 提交对照文件
		if (isDelSrcFile)
		{
			MoveFileEx(indexfname, DstIndexFile,
				MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING |MOVEFILE_WRITE_THROUGH);
		}
		else
		{
			CopyFile(indexfname, DstIndexFile, FALSE);
		}
	}
}

// void DceProcotol::RenameTmpSubDir(LPCTSTR Ip,		// Dse服务器IP
// 					LPCTSTR strpro,					// 协议名
// 					LPCTSTR SvrType)					// 服务器类型	
// {
// 	char now[20];
// 	sprintf(now, "%02dwt.flg", m_nowstm);	// 预计要输出的目录
// 
// 	// 检查Z盘
// 	if (V_WriteZ)
// 	{
// 		CString tmppath;
// 		tmppath.Format("v:\\netspy\\%s\\%s\\%s\\",SvrType, Ip, strpro);
// 		if(_access(tmppath,0)!=0)
// 		{
// 			CreateDir(tmppath);
// 		}
// 
// 		HANDLE hfindfile;
// 		WIN32_FIND_DATA finddata;
// 		hfindfile = FindFirstFile(tmppath+"*wt.flg", &finddata);
// 		if (hfindfile != INVALID_HANDLE_VALUE)
// 		{
// 			try
// 			{
// 				do
// 				{
// 					if (strcmp(finddata.cFileName, now) != 0)
// 					{
// 						CString wtfname;
// 						wtfname.Format("%s%s", tmppath, finddata.cFileName);
// 						CString okfname = wtfname;
// 						okfname.Replace("wt.flg", "ok.flg");
// 						if (0 != rename(wtfname, okfname))
// 						{
// 							Sleep(200);
// 							rename(wtfname, okfname);
// 						}
// 					}
// 
// 				}while(FindNextFile(hfindfile, &finddata));
// 			}
// 			catch (...)
// 			{
// 				WriteLog("dce", "RenameTmpSubDir(%s) Error!!", strpro);
// 			}
// 			FindClose(hfindfile);
// 		}
// 	}
// 
// 	// 检查D盘
// 	{
// 		CString tmppath;
// 		tmppath.Format("x:\\xxx\\%s\\%s\\%s\\",SvrType, Ip, strpro);
// 		if(_access(tmppath,0)!=0)
// 		{
// 			CreateDir(tmppath);
// 		}
// 
// 		HANDLE hfindfile;
// 		WIN32_FIND_DATA finddata;
// 		hfindfile = FindFirstFile(tmppath+"*wt.flg", &finddata);
// 		if (hfindfile != INVALID_HANDLE_VALUE)
// 		{
// 			try
// 			{
// 				do
// 				{
// 					if (strcmp(finddata.cFileName, now) != 0)
// 					{
// 						CString wtfname;
// 						wtfname.Format("%s%s", tmppath, finddata.cFileName);
// 						CString okfname = wtfname;
// 						okfname.Replace("wt.flg", "ok.flg");
// 						if (0!=rename(wtfname, okfname))
// 						{
// 							Sleep(200);
// 							rename(wtfname, okfname);
// 						}
// 					}
// 
// 				}while(FindNextFile(hfindfile, &finddata));
// 			}
// 			catch (...)
// 			{
// 				WriteLog("dce", "RenameTmpSubDir(%s) Error!!", strpro);
// 			}
// 			FindClose(hfindfile);
// 		}
// 		return;
// 	}
// }

void DceProcotol::RenameTmpDir()
{
	// 首先提交以前提交失败的文件
	for(int n=0; n<m_TmpFileArr.GetSize(); )
	{
		CString& tmpfile = m_TmpFileArr.ElementAt(n);
		CString outfile = tmpfile;
		outfile.Replace("tmp20", "20");
		int re = rename(tmpfile, outfile);
		if (re != 0)
		{
			if (_access(tmpfile, 0) == 0)
			{
				WriteLog("dce", "提交文件<%s>失败! ErrCode = %d", tmpfile, re);
				n++;
			}
			else
			{
				m_TmpFileArr.RemoveAt(n);
			}
		}
		else
		{
			m_TmpFileArr.RemoveAt(n);
		}
	}

	// 其次提交新文件
	LPIpDataStruct pval = m_pDseFileInfo;
	for (int i=m_DseInfoCount; i>0; i--, pval++)
	{
		if (!pval->m_indexfname.IsEmpty())
		{
			// 关闭流文件
			if (pval->m_pindexfile != NULL)
			{
				int re = fclose(pval->m_pindexfile);
				if (re != 0)
				{
					WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_indexfname, re);
				}
				pval->m_pindexfile = NULL;
			}
			if (pval->m_prealfile != NULL)
			{
				int re = fclose(pval->m_prealfile);
				if (re != 0)
				{
					WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_realfname, re);
				}
				pval->m_prealfile = NULL;
			}

			// 提交流文件
			CString idxfname = pval->m_indexfname;
			CString realfname = pval->m_realfname;
			idxfname.Replace("tmp20", "20");
			realfname.Replace("tmp20", "20");
			if (_access(v_WorkPath+"\\flg\\bakdcedata.flg",0) == 0)
			{
				if (_access(v_WorkPath+"\\bakdcedata",0)!=0)
				{
					_mkdir(v_WorkPath+"\\bakdcedata");
				}
				CString tmpfile;
				tmpfile.Format("%s\\bakdcedata\\%s",v_WorkPath, pval->m_realfname.Mid(pval->m_realfname.ReverseFind('\\')));
				CopyFile(pval->m_realfname,tmpfile,TRUE);
				tmpfile.Format("%s\\bakdcedata\\%s",v_WorkPath, pval->m_indexfname.Mid(pval->m_indexfname.ReverseFind('\\')));
				CopyFile(pval->m_indexfname,tmpfile,TRUE);

			}

			int re = rename(pval->m_realfname, realfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_realfname, realfname);
				if (re != 0)
				{
					m_TmpFileArr.Add(pval->m_realfname);
				}
			}

			re = rename(pval->m_indexfname, idxfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_indexfname, idxfname);
				if (re != 0)
				{
					m_TmpFileArr.Add(pval->m_indexfname);
				}
			}
		}
		pval->m_realfname.Empty();
		pval->m_indexfname.Empty();
		pval->m_filenum=0;
		pval->m_realflen=0;
	}
}
