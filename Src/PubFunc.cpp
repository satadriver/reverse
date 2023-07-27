#include "stdafx.h"
#include "dcepub.h"
#include "WebHisTable.h"

#include <sys\\stat.h>
#pragma comment(lib, "ws2_32.lib")


//LPCTSTR AllotServer(SERVERPOWER* pSvrPower, int svrtype, int filecount);

// LPWRITEAPPLOGEX v_pWriteLogFun=NULL;

Search v_ErrorStr("error");

 void WriteLog(LPCTSTR module, LPCTSTR FmtTxt, ...)
{
	ASSERT(module != NULL);
	static long errorcount = 0;
	BOOL isErrorLog = FALSE;
	try
	{
// 		if (v_pWriteLogFun==NULL)
// 			return;
		CString str = FmtTxt;
		str.MakeLower();
		if (0<=v_ErrorStr.find(str, str.GetLength()))	// 如果日志串中包含"error" 就认为是严重的错误
		{
			isErrorLog = TRUE;
			InterlockedIncrement(&errorcount);			// 严重的错误累计
		}

		va_list args;
		va_start( args, FmtTxt );
		str.FormatV(FmtTxt, args);
		va_end( args );
		if(v_DceRunFlg)
		{
			// 写日志到文件
			SYSTEMTIME nowtm;
			GetLocalTime(&nowtm);
			str.Format("%04d-%02d-%02d %02d:%02d:%02d\t%s\r\n",
				nowtm.wYear, nowtm.wMonth, nowtm.wDay,
				nowtm.wHour, nowtm.wMinute, nowtm.wSecond, (CString)str);
// #ifdef		_DEBUG
			printf("%s:\t%s\n", module, str.Left(str.GetLength()-2));
// #endif
			char fname[MAX_PATH];
			sprintf(fname, "%s\\log\\reverse\\%04d%02d%02d\\",
				v_WorkPath, nowtm.wYear, nowtm.wMonth, nowtm.wDay);
			if (_access(fname, 0) != 0)
			{
				CreateDir(fname);
				if (_access(fname, 0) != 0)
					return;
			}
			if (isErrorLog)
			{
#ifdef		DUMPHTTPPACKET
				v_HttpDumpHttpPacket.SaveBuffer();		// 如果有错误发生就保存包文件
#endif

				strcat(fname, "error.log");
				str.Format("%s:\t%s", module, (CString)str);
			}
			else
			{
				strcat(fname, module);
				strcat(fname, ".log");
			}
 			CFile logfile;
 			for (int i = 0; i < 3; i++)
 			{
 				logfile.Open(fname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate);
 				if (logfile.m_hFile != CFile::hFileNull)
 					break;
 				else if (i >= 2)
 					return;
				else
 					::Sleep(50);
 			}
// 			// 判断文件大小
// 			// 写文件
 			logfile.SeekToEnd();
 			logfile.Write((LPCTSTR)str, str.GetLength());
 			DWORD flen = logfile.GetLength();
 			logfile.Close();
//			if (flen > 300*1024)		// 300K
 //			int maxsize = 4096*1024;
// 			if (stricmp(module, "dce") == 0)
// 				maxsize = 1024*1024;
// 			else
// 				maxsize = 300*1024;
			if(str.Find("************") == -1 && _access(fname, 0) != 0)
			{
				// 每次切换日志文件, 写版本信息
				str.Format("DCE.EXE Bulid: %s %s\n%s\n%s", __DATE__, __TIME__, __FILE__, CString(str));
			}

	//		WriteAppLogEx(fname, str, str.GetLength(), maxsize);
//			if (0==v_pWriteLogFun(fname, str, str.GetLength(), maxsize))
			if (errorcount >= 500)
			{
				// 严重的错误累计到500, 程序就重启
// 				char bakfname[MAX_PATH];
// 				for (int i = 1; i <= 6; i++)
// 				{
// 					sprintf(bakfname, "%s\\log\\dce\\%04d%02d%02d\\%s_%d.log",
// 						v_RootPath, nowtm.wYear, nowtm.wMonth, nowtm.wDay, module, i);
// 					if (_access(bakfname, 0) != 0)
// 					{
// 						rename(fname, bakfname);
// 						break;
// 					}
// 				}
// 				DeleteFile(fname);
				if (v_programtype == SYSTEM_002)
				{
					CFile logfile;
					if (logfile.Open(v_WorkPath+"\flg\\bakdde.flg", 
						CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
					{
						logfile.Close();
					}
				}
				InterlockedExchange(&errorcount, 0);
				ExitDce(0);
			}
		} 
	}
	catch(...)
	{
		InterlockedIncrement(&errorcount);
	}
}

 void ExitDce(int exitcode)
{
	WriteLog("error", "DCE Exit!!, exitcode %d", exitcode);
	//	WinExec("kill.exe dce.exe -f", SW_HIDE);
	InterlockedExchange((long*)&v_DceRunFlg, 0);
	::Sleep(3000);
	//	char fname[300];
	//	sprintf(fname, "%s\\bin\\kill.exe dce.exe -f", Root_Path);
	//	system(fname);
	//	ClearMem();
	exit(0);
}

 void CreateDir(CString dir)
{
	if (_access(dir, 0) == 0)
		return;
	if (dir.GetAt(dir.GetLength()-1) == '\\')
		dir.ReleaseBuffer(dir.GetLength()-1);
	int pos = dir.ReverseFind('\\');
	if (pos > 0)
	{
		CString dirname = dir.Left(pos);
		CreateDir(dirname);
		mkdir(dir);
	}
}

BOOL CreateFlgFile(LPCTSTR filename)
{
	try
	{
		char flgname[200];
		if ((filename == NULL) || (filename[0] == 0))
		{
			sprintf(flgname, "%s\\flg\\reverserun.flg", (LPCTSTR)v_WorkPath);
			filename = flgname;
		}
		if (_access(filename, 0) != 0)
		{
			CFile file;
			if (file.Open(filename, CFile::modeCreate))
			{
				file.Close();
				return TRUE;
			}
			else
			{
				WriteLog("dce", "Error!! CreateFlgFile<%s> Fail!!", filename);
			}
		}
		else
			return TRUE;
	}
	catch(...)
	{
		WriteLog("dce", "CreateRunFlgFile() Error!!");
	}
	return FALSE;
}


 int GetFileLength(const char * filename)
{
	struct stat fst;
	if (stat(filename, &fst) == 0)
		return fst.st_size;
	else
		return 0;
}

 void FormatMac(BYTE* pData, CString& smac, CString& dmac, DATEHEADER& packh)
{
	ASSERT(pData != NULL);
	CString m_sip,m_dip;

	CString flgfile = "e:\\netspy\\flg\\getmac.flg";

	if(_access(flgfile,0) ==0)
	{
		if (packh.m_sip[0]==10)
		{
			m_sip.Format("%03d.%03d.%03d.%03d",packh.m_sip[0],packh.m_sip[1],packh.m_sip[2],packh.m_sip[3]);
			smac = GetMacAddress(::inet_addr(m_sip),m_sip);
		}
		
		if (smac.IsEmpty() || smac.CompareNoCase("00:00:00:00:00:00") ==0)
		{
			smac.Format("%02x:%02x:%02x:%02x:%02x:%02x", pData[6], pData[7],
				pData[8], pData[9], pData[10], pData[11]);
		}
		
		if (packh.m_dip[0]==10)
		{
			m_dip.Format("%03d.%03d.%03d.%03d",packh.m_dip[0],packh.m_dip[1],packh.m_dip[2],packh.m_dip[3]);
			dmac = GetMacAddress(::inet_addr(m_dip),m_dip);
		}
		
		if (dmac.IsEmpty() || dmac.CompareNoCase("00:00:00:00:00:00") ==0)
		{
			dmac.Format("%02x:%02x:%02x:%02x:%02x:%02x", pData[0], pData[1],
				pData[2], pData[3], pData[4], pData[5]);
		}
	}
	else
	{
			smac.Format("%02x:%02x:%02x:%02x:%02x:%02x", pData[6], pData[7],
				pData[8], pData[9], pData[10], pData[11]);
			dmac.Format("%02x:%02x:%02x:%02x:%02x:%02x", pData[0], pData[1],
				pData[2], pData[3], pData[4], pData[5]);
	}
	return;
}

CString GetMacAddress(IPAddr ipDst,CString ip)
{
	CString strmac;
	BYTE DstMac[6];
	memset(DstMac, 0,6);

	DWORD dwLen = 6;

	int t1=time(0);
	DWORD dwRet = ::SendARP(ipDst, 0, (PULONG)&DstMac, &dwLen);
	strmac.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"),DstMac[0],DstMac[1],DstMac[2],
		DstMac[3],DstMac[4],DstMac[5]);
	int t2=time(0);
	int diff = t2 - t1;

	CString flgfile = "e:\\netspy\\flg\\getmaclog.flg";

	if(_access(flgfile,0) ==0)
	{
		WriteLog("dce","耗时%d s,ip:%s,mac:%s\n",diff,ip,strmac);
	}

	return strmac;
}

 CString GetTimeString(time_t ttm,const char *format)
{
	struct tm *today;
	char tmbuf[40];
	today = localtime(&ttm);
	if(today == NULL)
		return "";
	else
	{
		memset(tmbuf,0,sizeof(tmbuf));
		strftime(tmbuf, 40,format, today );
		return CString(tmbuf);
	}
}

 int GetTimeString(time_t ttm, LPCTSTR fmt, char* tmbuf, int buflen)
{
	ASSERT(buflen>=20);
	struct tm *today;
	today = localtime(&ttm);
	if(today == NULL)
	{
		memset(tmbuf, 0, 4);
		return 0;
	}
	else
	{
		return strftime(tmbuf, buflen, fmt, today);
	}
}

void DeleteFilesFromTmpDir(LPCTSTR filename, LPCTSTR indexfilename, CStringArray& attfiles)
{
	int arraycount=attfiles.GetUpperBound();
	// 附件
	for(int i=0 ;i<=arraycount;i++)
	{
		CString& attfile=attfiles.ElementAt(i);
		if(!attfile.IsEmpty() && _access(attfile,0)==0)
		{
			DeleteFile(attfile);
		}
	}
	// 实体文件
	if(filename[0]!=0 && _access(filename,0)==0)
	{
		DeleteFile(filename);
	}
	// 对照文件
	if(indexfilename[0]!=0 && _access(indexfilename,0)==0)
	{
		DeleteFile(indexfilename);
	}
}

void MoveFilesToDir(CString& desdir, LPCTSTR filename, LPCTSTR indexfilename,
					CStringArray& attfiles, BOOL isdelsrc,LPCTSTR pro,LPCTSTR dseIp)
{
// 	if(strcmp(pro, "httpup") == 0)
// 	{
// 		LPIpDataStruct m_LPIpDataStruct;
// 		if(!v_DseIpInfoMap.Lookup(dseIp,m_LPIpDataStruct))
// 		{
// 			m_LPIpDataStruct = new IpDataStruct;
// 			v_DseIpInfoMap.SetAt(dseIp,m_LPIpDataStruct);
// 		}
// 		if (m_LPIpDataStruct->num >= 100||
// 			m_LPIpDataStruct->fp_filename == NULL||
// 			m_LPIpDataStruct->fp_indexfile == NULL)
// 		{
// 			CString tmpindexfilename,tmpfilename;
// 			tmpindexfilename.Format("%s",desdir+GetShortName(indexfilename));
// 			tmpfilename.Format("%s",desdir+GetShortName(filename));
// 			m_LPIpDataStruct->num = 0;
// 			
// 			if (m_LPIpDataStruct->fp_filename != NULL)
// 			{
// 				fclose(m_LPIpDataStruct->fp_filename);
// 			}
// 			m_LPIpDataStruct->fp_filename = fopen(tmpfilename,"ab");
// 
// 			if (m_LPIpDataStruct->fp_indexfile != NULL)
// 			{
// 				fclose(m_LPIpDataStruct->fp_indexfile);
// 			}
// 
// 			m_LPIpDataStruct->fp_indexfile = fopen(tmpindexfilename,"ab");
// 		}
// 		m_LPIpDataStruct->num ++;
// 
// 		if(filename[0]!=0 && _access(filename,0)==0 &&
// 			indexfilename[0]!=0 && _access(indexfilename,0)==0)
// 		{
// 
// 			char* ptxt;
// 			CString str;
// 			char* buf = new char [FILEBUFFERSIZE];
// 			int tlen=GetFileLength(filename);
// 			
// 			FILE * fs=0;
// 			fs=fopen(filename,"rb");
// 			if(fs>0	)
// 			{
// 				if (tlen>=sizeof(buf)-10)
// 					ptxt = str.GetBufferSetLength(tlen+10);
// 				else
// 					ptxt = buf;
// 				memset(ptxt,0,tlen+10);
// 				fread(ptxt,1,tlen,fs);
// 				fclose(fs);
// 				if (tlen>=sizeof(buf)-10)
// 					str.ReleaseBuffer();
// 			}
// 			if (m_LPIpDataStruct->num != 1)
// 			{
// 				fwrite(SPACEMARK, 1, sizeof(SPACEMARK)-1, m_LPIpDataStruct->fp_filename);
// 			}
// 
// 			fwrite(ptxt,1,tlen,m_LPIpDataStruct->fp_filename);
// 			fflush(m_LPIpDataStruct->fp_filename);
// 
// 
// 			tlen=GetFileLength(indexfilename);
// 			fs=fopen(indexfilename,"rb");
// 			if(fs>0	)
// 			{
// 				if (tlen>=sizeof(buf)-10)
// 					ptxt = str.GetBufferSetLength(tlen+10);
// 				else
// 					ptxt = buf;
// 				memset(ptxt,0,tlen+10);
// 				fread(ptxt,1,tlen,fs);
// 				fclose(fs);
// 				if (tlen>=sizeof(buf)-10)
// 					str.ReleaseBuffer();
// 
// 			}
// 			CString filepos;
// 			filepos.Format("[%d]",m_LPIpDataStruct->num);
// 			fwrite(filepos,1,filepos.GetLength(),m_LPIpDataStruct->fp_indexfile);
// 			fwrite(ptxt+3,1,tlen-3,m_LPIpDataStruct->fp_indexfile);
// 			fflush(m_LPIpDataStruct->fp_indexfile);
// 			delete[] buf;
// 		}
// 		return ;
// 	}

	int arraycount=attfiles.GetUpperBound();
	CString attfile;
	ASSERT(desdir[desdir.GetLength()-1] == '\\');
	// 附件
	for(int i=0 ;i<=arraycount;i++)
	{
		attfile=attfiles.GetAt(i);
		if(!attfile.IsEmpty() && _access(attfile,0)==0)
		{
			if (isdelsrc)
				MoveFileEx(attfile, desdir+GetShortName(attfile),
				MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING |MOVEFILE_WRITE_THROUGH);
			else
				CopyFile(attfile, desdir+GetShortName(attfile), FALSE);
		}
	}
	// 实体文件
	if(filename[0]!=0 && _access(filename,0)==0)
	{
		if (isdelsrc)
			MoveFileEx(filename,desdir+GetShortName(filename),
			MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING |MOVEFILE_WRITE_THROUGH);
		else
			CopyFile(filename, desdir+GetShortName(filename), FALSE);
	}
	// 对照文件
	if(indexfilename[0]!=0 && _access(indexfilename,0)==0)
	{
		if (isdelsrc)
			MoveFileEx(indexfilename,desdir+GetShortName(indexfilename),
			MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING |MOVEFILE_WRITE_THROUGH);
		else
			CopyFile(indexfilename, desdir+GetShortName(indexfilename), FALSE);
	}
}

static char strcase[] = "case= true\r\n";
static Search casestr[5] = \
{
	Search("caseid= "),
	Search("objid= "),
	Search("limitaddr= "),
	Search("limittype= "),
	Search("[2]\r\n")
};

void FilterIndexfile(LPCTSTR indexfile)
{

//	static LPCTSTR pcasestr[4] = \
//	{
//		_T("limittype= "), _T("limitaddr= "), _T("objid= "), _T("\r\n") 
//	};
//	CFile file;
//	CString str;
//	CString tmpstr;
//	file.Open(indexfile, CFile::modeReadWrite);
//	if (file.m_hFile != CFile::hFileNull)
//	{
//		DWORD size = file.GetLength();
//		char* tp = str.GetBufferSetLength(size);
//		//		char* p = tmpstr.GetBufferSetLength(size);
//		file.Read(tp, size);
//		if (casestr[4].find(tp, size) > 0)
//		{
//			int pos = 0;
//			while((pos=casestr[0].find(tp, size))>=0)
//			{
//				BOOL isadd = FALSE;
//				int len = SearchCRLF.find(tp+pos, size-pos);
//				if (len >= sizeof(strcase)-1)
//				{
//					memcpy(tp+pos, strcase, sizeof(strcase)-1);
//					isadd = TRUE;
//					pos += sizeof(strcase)-1;
//					len -= sizeof(strcase)-1;
//				}
//				else if (len < 0)
//				{
//					tp[pos] = 0;
//					break;
//				}
//				memmove(tp+pos, tp+pos+len+2, size-pos-len-2);
//				size -= len+2;
//				for (int i = 1; i < 4; i++)
//				{
//					pos = casestr[i].find(tp, size);
//					if (pos >= 0)
//					{
//						len = SearchCRLF.find(tp+pos, size-pos);
//						if (!isadd && len >= sizeof(strcase)-1)
//						{
//							memcpy(tp+pos, strcase, sizeof(strcase)-1);
//							isadd = TRUE;
//							pos += sizeof(strcase)-1;
//							len -= sizeof(strcase)-1;
//						}
//						else if (len < 0)
//						{
//							tp[pos] = 0;
//							size = pos;
//							break;
//						}
//						memmove(tp+pos, tp+pos+len+2, size-pos-len-2);
//						size -= len+2;
//					}
//				}
//			}
//			str.ReleaseBuffer(size);
//			file.Seek(0, CFile::begin);
//			file.Write((LPCTSTR)str, str.GetLength());
//			file.SetLength(str.GetLength());
//		}
//		else
//		{
//			int pos = 0;
//			for (int i = 0; i < 4; i++)
//			{
//				int pos = casestr[i].find(tp, size);
//				if (pos > -1)
//				{
//					int len = SearchCRLF.find(tp+pos, size-pos);
//					if (len > -1)
//					{
//						memmove(tp+pos, tp+pos+len+2, size-pos-len-2);
//						size -= len+2;
//					}
//				}
//			}
//			str.ReleaseBuffer(size);
//			file.Seek(0, CFile::begin);
//			file.Write((LPCTSTR)str, str.GetLength());
//			file.Write(strcase, sizeof(strcase)-1);
//			file.SetLength(str.GetLength()+sizeof(strcase)-1);
//		}
//		file.Close();
//	}
}

// 分发文件至所有DcE
 void SendSpreadInfo(const char *InfoBuf, int bufLen)
{
	for (int iDCE = 0; iDCE < v_DceTeSpread.count ; iDCE++)
	{
		int rc = sendto(
			v_DceTeSpread.dceTe[iDCE].dceradius, 
			InfoBuf,
			bufLen, 
			0, 
			&(v_DceTeSpread.dceTe[iDCE].dce_addr), 
			sizeof(v_DceTeSpread.dceTe[iDCE].dce_addr) );
	}

	return ;
}

//  void CopyFileToTmpdir(LPCTSTR CaseId, LPCTSTR filename, LPCTSTR indexfilename,
// 								   CStringArray& attfiles, int curtm, LPCTSTR subdir, SERVERPOWER* pCsPwr,
// 								   SERVERPOWER* pIsPwr,
// 								   CStringArray	&ruleidarr,CStringArray	&fenleiarr,CStringArray	&levelarr,
// 								   CStringArray	&cardarr,CStringArray	&namearr,CStringArray	&peopletypearr,
// 								   CStringArray	&peoplearr,CStringArray	&keytypearr,CStringArray &IsOnlyAlarmArr,
// 								   CStringArray &dummytypearr)
// {
// 	ASSERT(pCsPwr!=NULL);
// 	CString desdir;
// 	// 实时跟踪数据
// 	BOOL isDelSrcFile = (strcmp(subdir, "httpurl") == 0 || pIsPwr==NULL);
// 	CString dstfilename;
// 
// ///*
// //	if(strcmp(subdir, "httpdown") == 0)
// //	{
// //		int len = strlen(filename);
// //		if(memcmp(filename+len-3, "gif", 3)==0)
// //			len = 0;
// //	}
// //*/
// //	ASSERT(stricmp(subdir, "webmail") != 0);
// 	int filecount = 1 + attfiles.GetSize();
// 	if (filename[0] != NULL)
// 		filecount++;
// 	//案件
// 	if (CaseId[0]!=0)
// 	{
// 		
// 		if (v_programtype==SYSTEM_SLEUTH)
// 			isDelSrcFile = TRUE;	// Sleuth2005系统 案件数据不进入预警
// 		LPCTSTR CsIp = AllotServer(pCsPwr, 1, filecount);
// 
// 		ASSERT(CsIp[0] != 0);
// 		BOOL isSucc = FALSE;
// 		for (int i=0; i<60; i++)
// 		{
// 			int zFreeSpace, zPercentFree;
// 			zPercentFree = GetZFreeDisk(zFreeSpace);
// 			if((zPercentFree < 15) || (stricmp(subdir, "media") == 0))
// 			{
// 				desdir.Format("%s\\caseserver\\%s\\%s\\%02d",
// 					v_RootPath, CsIp, subdir, (curtm+i)%MAXSUBDIR);
// 			}
// 			else
// 			{
// 				desdir.Format("v:\\netspy\\caseserver\\%s\\%s\\%02d",
// 					 CsIp, subdir, (curtm+i)%MAXSUBDIR);
// 			}
// 
// 			if(_access(desdir, 0)!=0)
// 			{
// 				CreateDir(desdir);
// 			}
// 			if (_access(desdir+"ok.flg", 0) != 0)
// 			{
// 				if (_access(desdir+"wt.flg", 0) != 0)
// 				{
// 					for (int l=0; l<3; l++)
// 					{
// 						CFile file;
// 						if (file.Open(desdir+"wt.flg", CFile::modeCreate))
// 						{
// 							file.Close();
// 							break;
// 						}
// 						else
// 							::Sleep(100);
// 					}
// 				}
// 				isSucc = TRUE;
// 				break;
// 			}
// 		}
// 
// 		if (isSucc)
// 		{
// 			desdir.Format("%s\\%s\\", (CString)desdir, CaseId);
// 			if(_access(desdir, 0)!=0)
// 			{
// 				CreateDir(desdir);
// 			}
// 			MoveFilesToDir(desdir,filename,indexfilename,attfiles,isDelSrcFile,"aa",CsIp);
// 		}
// 		if (fenleiarr.GetSize()>0)
// 		{
// 			fenleiarr.RemoveAll();
// 			ruleidarr.RemoveAll();
// 			levelarr.RemoveAll();
// 			cardarr.RemoveAll();
// 			namearr.RemoveAll();
// 			peopletypearr.RemoveAll();
// 			peoplearr.RemoveAll();
// 			keytypearr.RemoveAll();
// 			dummytypearr.RemoveAll();
// 		}
// 	}
// 	
// 	//处理预警数据，多条预警规则时要写多个文件
// 	if (fenleiarr.GetSize()>0 &&
// 		v_programtype!=SYSTEM_SLEUTH &&
// 		CaseId[0]==0)
// 	{
// 		CMapStringToString Tmp_CutRepeate;
// 
// 		for(int hh = 0;hh<fenleiarr.GetSize();hh++)
// 		{
// 			if (IsOnlyAlarmArr.GetAt(hh).CompareNoCase("是") == 0)
// 			{
// 				continue;
// 			}
// 			CString tmp_look,tmp_id;
// 			tmp_look.Format("%s%s%s",fenleiarr.GetAt(hh),ruleidarr.GetAt(hh),peoplearr.GetAt(hh));
// 
// 			if (Tmp_CutRepeate.Lookup(tmp_look,tmp_id))//一个文件只能对应分类，规则，人员一条。去除重复数据
// 			{
// 				continue;
// 			}
// 			else
// 			{
// 				Tmp_CutRepeate.SetAt(tmp_look,"");
// 			}
// 			LPCTSTR WsIp = AllotServer(pCsPwr+2, 4, filecount);
// 				
// 			::WritePrivateProfileString("1","fenlei",fenleiarr.GetAt(hh),indexfilename);
// 			::WritePrivateProfileString("1","ruleid",ruleidarr.GetAt(hh),indexfilename);
// 			::WritePrivateProfileString("1","level",levelarr.GetAt(hh),indexfilename);
// 			::WritePrivateProfileString("1","card",cardarr.GetAt(hh),indexfilename);
// 			::WritePrivateProfileString("1","name",namearr.GetAt(hh),indexfilename);
// 			::WritePrivateProfileString("1","peopletype",peopletypearr.GetAt(hh),indexfilename);
// 			::WritePrivateProfileString("1","people",peoplearr.GetAt(hh),indexfilename);
// 			::WritePrivateProfileString("1","keytype",keytypearr.GetAt(hh),indexfilename);
// 			::WritePrivateProfileString("1","dummytype",dummytypearr.GetAt(hh),indexfilename);
// 
// 			
// 			ASSERT(WsIp[0] != 0);
// 			BOOL isSucc = FALSE;
// 			for (int i=0; i<60; i++)
// 			{
// 				int zFreeSpace, zPercentFree;
// 				zPercentFree = GetZFreeDisk(zFreeSpace);
// 				if((zPercentFree < 15) || (stricmp(subdir, "media") == 0))
// 				{
// 					desdir.Format("%s\\warnserver\\%s\\%s\\%02d",
// 						v_RootPath, WsIp, subdir, (curtm+i)%MAXSUBDIR);
// 				}
// 				else
// 				{
// 					desdir.Format("v:\\netspy\\warnserver\\%s\\%s\\%02d",
// 						WsIp, subdir, (curtm+i)%MAXSUBDIR);
// 				}
// 
// 				if(_access(desdir, 0)!=0)
// 				{
// 					CreateDir(desdir);
// 				}
// 				if (_access(desdir+"ok.flg", 0) != 0)
// 				{
// 					if (_access(desdir+"wt.flg", 0) != 0)
// 					{
// 						for (int l=0; l<3; l++)
// 						{
// 							CFile file;
// 							if (file.Open(desdir+"wt.flg", CFile::modeCreate))
// 							{
// 								file.Close();
// 								break;
// 							}
// 							else
// 								::Sleep(100);
// 						}
// 					}
// 					isSucc = TRUE;
// 					break;
// 				}
// 			}
// 
// 			if (isSucc)
// 			{
// 				desdir.Format("%s\\%s\\", (CString)desdir, fenleiarr.GetAt(hh));
// 				if(_access(desdir, 0)!=0)
// 				{
// 					CreateDir(desdir);
// 				}
// 				MoveFilesToDir(desdir,filename,indexfilename,attfiles,isDelSrcFile,"aa",WsIp);
// 			}
// 		}
// 
// 		Tmp_CutRepeate.RemoveAll();
// 
// 		fenleiarr.RemoveAll();
// 		ruleidarr.RemoveAll();
// 		levelarr.RemoveAll();
// 		cardarr.RemoveAll();
// 		namearr.RemoveAll();
// 		peopletypearr.RemoveAll();
// 		peoplearr.RemoveAll();
// 		keytypearr.RemoveAll();
// 		dummytypearr.RemoveAll();
// 	}
// 	
// 	//给指定的全文Copy文件
// 	// 所有的文件都进全文一份	2005-01-18
// 	if (!isDelSrcFile)
// 	{
// 		ASSERT(pIsPwr!=NULL);
// 		LPCTSTR IsIp = AllotServer(pIsPwr, 2, filecount);
// 		if (v_programtype==SYSTEM_SLEUTH)
// 			isDelSrcFile = TRUE;	// Sleuth2005系统 案件数据不进入预警
// 
// 		ASSERT(IsIp[0] != 0);
// 		if (CaseId[0]!=0)
// 			::WritePrivateProfileString("1","IsCase","TRUE",indexfilename);
// 
// //			FilterIndexfile(indexfilename);	// 过虑对照文件中的案件信息
// // 		if(stricmp(subdir,"httpup") ==0)
// // 		{
// // 			desdir.Format("%s\\indexserver\\%s\\%s\\TMP%s\\%02d\\",v_RootPath, IsIp, subdir, Tm12,rand()%100);
// // 		}
// // 		else
// // 		{
// // 			desdir.Format("%s\\indexserver\\%s\\%s\\TMP%s\\",v_RootPath, IsIp, subdir, Tm12);
// // 		}
// 
// 
// 		BOOL isSucc = FALSE;
// 		for (int i=0; i<60; i++)
// 		{
// 			int zFreeSpace, zPercentFree;
// 			zPercentFree = GetZFreeDisk(zFreeSpace);
// 			if((zPercentFree < 15) || (stricmp(subdir, "media") == 0))
// 			{
// 				desdir.Format("%s\\indexserver\\%s\\%s\\%02d",
// 					v_RootPath, IsIp, subdir, (curtm+i)%MAXSUBDIR);
// 			}
// 			else
// 			{
// 				desdir.Format("v:\\netspy\\indexserver\\%s\\%s\\%02d",
// 					IsIp, subdir, (curtm+i)%MAXSUBDIR);
// 			}
// 
// 			if(_access(desdir, 0)!=0)
// 			{
// 				CreateDir(desdir);
// 			}
// 
// 			if (_access(desdir+"ok.flg", 0) != 0)
// 			{
// 				if (_access(desdir+"wt.flg", 0) != 0)
// 				{
// 					for (int l=0; l<3; l++)
// 					{
// 						CFile file;
// 						if (file.Open(desdir+"wt.flg", CFile::modeCreate))
// 						{
// 							file.Close();
// 							break;
// 						}
// 						else
// 							::Sleep(100);
// 					}
// 				}
// 				isSucc = TRUE;
// 				break;
// 			}
// 		}
// 
// 		if (isSucc)
// 		{
// 			desdir += "\\";
// 			if(strcmp(subdir, "httpup") == 0)
// 				MoveFilesToDir(desdir,filename,indexfilename,attfiles,TRUE,subdir,IsIp);
// 			else
// 				MoveFilesToDir(desdir,filename,indexfilename,attfiles,TRUE,"",IsIp);
// 
// 		}
// 	}
// 	DeleteFilesFromTmpDir(filename,indexfilename,attfiles);
// }

 long GetZFreeDisk(int &zFreeSpace)
{
	if(!V_WriteZ)
		return 0;
	DWORD SectorsPerCluster;
	DWORD BytesPerSector;
	DWORD NumberOfFreeClusters;
	DWORD TotalNumberOfClusters;
	CString tmpstr;
	tmpstr.Format("%s:\\",g_ddepath);
	if (_access(tmpstr,0) != 0)
	{
		return 0;
	}
	tmpstr.Format("%s:",g_ddepath);

    GetDiskFreeSpace( tmpstr, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters);
	zFreeSpace = (NumberOfFreeClusters*BytesPerSector*SectorsPerCluster)/1024;
    return (NumberOfFreeClusters*100)/TotalNumberOfClusters;
}

// 检查Z盘是否可用
 BOOL CheckZDrive()
{
	static BOOL s_bAvailable = TRUE;			//Z盘的状态-缺省是可用状态
	CString tmpstr;
	tmpstr.Format("%s:\\",g_ddepath);
	if ((_access(tmpstr,0) != 0)|| (!V_WriteZ))	//Z盘不存在||当前设置是不能使用Z盘
	{
		return FALSE;
	}
	// 获取Z盘的可用空间比例
	DWORD SectorsPerCluster;
	DWORD BytesPerSector;
	DWORD NumberOfFreeClusters;
	DWORD TotalNumberOfClusters;
    GetDiskFreeSpace( "z:", &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters);
    int bl = (NumberOfFreeClusters*100)/TotalNumberOfClusters;
	if (bl >= 30)
	{
		if (!s_bAvailable)
		{
			// 设置状态为有效
			InterlockedExchange((long*)&s_bAvailable, 1);
		}
		return TRUE;
	}
	else if (bl <= 20)
	{
		if (s_bAvailable)
		{
			// 设置状态为无效
			InterlockedExchange((long*)&s_bAvailable, 0);
		}
		return FALSE;
	}
	else
	{
		// 保持原状态
		return s_bAvailable;
	}
}
// LPCTSTR AllotServer(SERVERPOWER* pSvrPower, int svrtype, int filecount)
// {
// 	ASSERT(filecount>0);
// 	ASSERT(pSvrPower!=NULL);
// 	ASSERT(svrtype==1 || svrtype==2 || svrtype ==4);
// 	if (pSvrPower->servercount <= 0)
// 	{
// 		RefurServerPower(pSvrPower, svrtype);
// 	}
// 	ASSERT(pSvrPower->servercount>0);
// 	ASSERT(pSvrPower->server!=NULL);
// 	ONESVRPWR *tpsvr = pSvrPower->server+(rand()%pSvrPower->servercount);
// 	LPCTSTR ip = tpsvr->ip;
// 	if (tpsvr->power < filecount)
// 	{
// 		pSvrPower->servercount--;
// 		memcpy(tpsvr, pSvrPower->server+pSvrPower->servercount, sizeof(ONESVRPWR));
// 	}
// 	else
// 	{
// 		tpsvr->power -= filecount;
// 	}
// 	return ip;
// }

/*
		CString tempstr;
			tempstr.Format("%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%d\1%d\1%d\1%s\1%s\1",
				s->fenleiarr.GetAt(tmpi),s->ruleidarr.GetAt(tmpi),s->peopletypearr.GetAt(tmpi),
				s->peoplearr.GetAt(tmpi),s->namearr.GetAt(tmpi),s->cardarr.GetAt(tmpi),
				s->keytypearr.GetAt(tmpi),spytm,"","pop3",
				s->levelarr.GetAt(tmpi)=="红"?1:0,s->levelarr.GetAt(tmpi)=="橙"?1:0,s->levelarr.GetAt(tmpi)=="黄"?1:0,spytm,spytm);
			WriteArrToData(HTTP_PROTOCOL, WARNALARM_DATA, tempstr);
*/

//将预警数据写入缓冲区
// BOOL WriteWarnAlarmArrToData(int num,CStringArray& fenleiarr,CStringArray& ruleidarr,CStringArray& peopletypearr,
// 				CStringArray& peoplearr,CStringArray& namearr,CStringArray& cardarr,CStringArray& keytypearr,
// 				CString spytm,CString state,CString pro,CStringArray& levelarr,CString begtm,CString endtm,
// 				CString spyaddr,CStringArray& dummytypearr)
// {
// 	CMapStringToString  v_warnalarmmap;
// 	CString				strkey;
// 	CString				res,tempstr;
// 
// 	v_warnalarmmap.RemoveAll();
// 	v_warnalarmmap.InitHashTable(1001);	
// 
// 	for(int iloop=0;iloop<fenleiarr.GetSize();iloop++)
// 	{
// 		strkey.Format("%s%s%s",fenleiarr.GetAt(iloop),ruleidarr.GetAt(iloop),peoplearr.GetAt(iloop));
// 
// 		WriteLog("dce","ccj alarm strkey=%s",strkey);
// 
// 		if(v_warnalarmmap.Lookup(strkey,res))
// 		{
// 			WriteLog("dce","ccj alarm find");
// 			continue;
// 		}
// 		else
// 		{
// 			tempstr.Format("%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%d\1%d\1%d\1%s\1%s\1%s\1%s\1",
// 					fenleiarr.GetAt(iloop),ruleidarr.GetAt(iloop),peopletypearr.GetAt(iloop),
// 					peoplearr.GetAt(iloop),namearr.GetAt(iloop),cardarr.GetAt(iloop),
// 					keytypearr.GetAt(iloop),spytm,state,pro,
// 					levelarr.GetAt(iloop)=="红"?1:0,levelarr.GetAt(iloop)=="橙"?1:0,levelarr.GetAt(iloop)=="黄"?1:0,
// 					begtm,endtm,spyaddr,dummytypearr.GetAt(iloop));
// 
// 			WriteArrToData(num, WARNALARM_DATA, tempstr);
// 
// 			v_warnalarmmap.SetAt(strkey,"i am here");
// 		}
// 	}
// 
// 	return TRUE;
// }
// 



BOOL WriteArrToData(int num/*第几个数组*/,
					int type/*数据的类型*/,
					CString str/*1条记录*/,
					CStringArray* Arrstr/*多条记录*/,
					LPCTSTR pqq
					)
{
	if (type == TAOBAOPASS_DATA )//||
// 		type == ERROR_DATA)
	{
		;
	}
	else if (!v_stat)
	{
		return FALSE;
	}
	else if (_access("e:\\netspy\\flg\\dropwebfig.flg",0)==0)//增加标志文件，有时不吐虚拟身份
	{
		return FALSE;
	}
	if (str.IsEmpty())
	{
		if (Arrstr==NULL)
			return FALSE;
		else if (Arrstr->GetSize()==0)
			return FALSE;
	}
	ASSERT(num>=0);
	ASSERT(num<TOTALPRONUM);

	long bufid = v_statdata[num].m_writeflag;
	ASSERT(bufid==0 || bufid==1);
	CStringArray* pdataarr=NULL;
	switch(type)
	{
// 	case HISTORY_DATA:		// 活动历史
// 		pdataarr = v_statdata[num].m_hisdataarr+bufid;
// 		break;
// 	case SEARCHKEY_DATA:	// 引擎关键字
// 		pdataarr = v_statdata[num].m_keydataarr+bufid;
// 		break;
	case QQPASS_DATA:       // QQ密码对照表
		pdataarr = v_statdata[num].m_qqpassdataarr+bufid;
		break;
	case TAOBAOPASS_DATA:       // TAOBAO密码对照表
		pdataarr = v_statdata[num].m_taobaopassdataarr+bufid;
		break;

// 	case INDEXKEY_DATA:		// 预设关键字
// 		pdataarr = v_statdata[num].m_prekeydataarr+bufid;
// 		break;
// 	case WEBFIG_DATA:		// 虚拟身份
// 		pdataarr = v_statdata[num].m_webfigurearr+bufid;
// 		break;
	case WEBID_DATA:		// 虚拟身份
		pdataarr = v_statdata[num].m_webidarr+bufid;
		break;
	case QQCRACK_DATA:		// 虚拟身份
		//pdataarr = v_statdata[num].m_qqcrackarr+bufid;
		break;
	case HTTPREGRIST_DATA:		// HTTP注册
		pdataarr = v_statdata[num].m_httpregistarr+bufid;
		break;
	case UNIP_DATA:			// 非标
		pdataarr = v_statdata[num].m_uniparr+bufid;
		break;
	case ATTFILE_DATA:		// 附件
		pdataarr = v_statdata[num].m_attfilearr+bufid;
		break;
	case CCIC_ALARM:		// CCIC
		pdataarr = v_statdata[num].m_ccicarr+bufid;
		break;

//360 qqgroup先不输出，否则造成现场挤压
// 	case UNIMID_DATA:		// CCIC
// 		pdataarr = v_statdata[num].m_uniqmidarr+bufid;
// 		break;
// 	case QQGCHAT_DATA:		// CCIC
// 		pdataarr = v_statdata[num].m_qqgchatarr+bufid;
// 		break;

	case KEYOBJ_ALARM:		// 重点人员告警
		pdataarr = v_statdata[num].m_keyobjarr+bufid;
		break;
	case KEYOBJ_ONLINE:		// 重点人员
//		pdataarr = v_statdata[num].m_keyobjonlinearr+bufid;
		break;
	case REACTIONARY_ALARM:
		pdataarr = v_statdata[num].m_saReactionary + bufid;
		break;

// 	case AGENT_DTAT:		// 代理
// //		pdataarr = v_statdata[num].m_agentdataarr+bufid;
// 		return TRUE;
// 		break;
// 	case WARNALARM_DATA:       // 预警告警对照表
// 		pdataarr = v_statdata[num].m_warnalarmdataarr+bufid;
// 		break;
// 	case ERROR_DATA:       // 错误或正确的qq或taobao对照表
// 		pdataarr = v_statdata[num].m_errordataarr+bufid;
// 		break;
// 	case TAOBAOERROR_DATA:       // 错误的taobao对照表
// 		pdataarr = v_statdata[num].m_errortaobaoarr+bufid;
// 		break;
		
	default:
		ASSERT(FALSE);	// 无效的数据类型
		return FALSE;
	}

	if(type == QQCRACK_DATA)
	{
		if (!str.IsEmpty() && pqq != NULL)
		{
			v_statdata[num].m_qqcrackarr[bufid].SetAt(pqq, str);
		}
	}
	else if (type==KEYOBJ_ONLINE)
	{
		CString strnum="1";
		if (v_statdata[num].m_keyobjonlinearr[bufid].Lookup(str,strnum))
		{
			int num=atoi(strnum);
			num++;
			strnum.Format("%d",num);
		}
		v_statdata[num].m_keyobjonlinearr[bufid].SetAt(str,strnum);
	}
	else
	{
		if(type == WEBID_DATA &&g_dbgSwitch.m_outuplogin)
		{
			if(!str.IsEmpty())
			{
				WriteLog("dce", "Add Webfigure %s", str);
			}
			if(Arrstr != NULL)
			{
				for (int iiw = 0; iiw < Arrstr->GetSize() ; iiw++)
				{
					WriteLog("dce", "Add WebfigureArr %d/%d %s", iiw, Arrstr->GetSize(), Arrstr->GetAt(iiw));
				}
			}
		}

		// 
		if (pdataarr->GetSize()>=10000)
		{
			WriteLog("dce", "Error!! PRO%d的数据表缓冲区%d[%d]已满!!", num, type, bufid);
			return TRUE;
		}
		if (!str.IsEmpty())
		{
			pdataarr->Add(str);
		}
		else
		{
			pdataarr->Append(*Arrstr);
		}
	}

// 	if (pdataarr->GetSize()>0)
// 	{
// 		writetotmpfile(pdataarr, "ccic");
// 		pdataarr->RemoveAll();
// 	}
// 	v_statdata[num].m_ccicarr[bufid].Append(pdataarr);
	return TRUE;
}

 void Splitstr(CString str ,CStringArray &array,char e)//用于分割单个字符
{
	CString tmpstr;
	int pos;
	pos=str.Find(e);
	while (pos>=0)
	{
		tmpstr=str.Left(pos);
		tmpstr.TrimRight();
		tmpstr.TrimLeft();
		array.Add(tmpstr);
		str=str.Mid(pos+1);
		pos=str.Find(e);
	}
	if((pos == -1) && (!str.IsEmpty()))
		array.Add(str);	
}

 /*! \fn BOOL  CheckIfExcludeChinese(char *str,int len)   
	\brief	判断字符串是否不包含汉字
	\param	[IN]*str 要判断的字符串
	\param	[IN]len  字符串长度
	\return	如果不包含,函数返回TRUE,否则函数返回FALSE

*/
BOOL  CheckIfExcludeChinese(char *str,int len)   
{
	 for(int i=0;i<len;i++)
	 {
		 if (str[i] < 20 ||
			 str[i] > 128)
		 {
			return FALSE;
		 }
	 }

	 return TRUE;
}

struct
{
	int proidx;
	const char *pro;
	const char *ext;
}
mapProExt[] =
{
	{PRO_HTTPS, "https",		"http"},	
	{PRO_SQL,	"sqlserver",	"imsg"},	
	{PRO_OICQ,	"oicq",			"imsg"},	
	{PRO_MSN,	"msn",			"imsg"},	
	
	{ 0,		 0 }
};

BOOL BakProData(int proidx, DATEHEADER& packh,const char *pPacket )
{
	int bakcount = 0;
	time_t now = time(0);
	static time_t checktm = 0;
	if (abs(now - checktm)>=60)
	{
		// 检查是否备份Http
		// v_HttpDumpHttpPacket.InitDumpBuffer();
		checktm = now;
		bakcount = 0;
		// 读取各个协议的备份开关
		for (int i = 0; i < PRONUM_MAX; i++)
		{
			int bakval = ::GetPrivateProfileInt(mapProExt[i].pro,"bakfile",0,"d:\\netspy\\para\\ddebakfile.ini");
			if (bakval<0)
				bakval = 0;
			else if (bakval>100)
				bakval = 100;		// 最大100M
			if (bakval!=0)
				bakcount++;
			g_arrbakpro[mapProExt[i].proidx] = bakval*1024*1024;
		}
		g_bakpro = bakcount;
	}
	if(g_bakpro == 0)
		return TRUE;
	if(g_arrbakpro[proidx] == 0)
		return TRUE;

	if(_access(v_DdeBakPath, 0) != 0)
		CreateDir(v_DdeBakPath);

	CString bkfile;
	for (int ii = 0; ii < PRONUM_MAX ; ii++)
	{
		if(mapProExt[ii].proidx == proidx)
		{
			bkfile.Format("%s\\%s.%s", v_DdeBakPath, mapProExt[proidx].pro, mapProExt[proidx].ext);
			break;
		}
	}
	if(FALSE == bkfile.IsEmpty())
	{
		CFile fp;
		if(fp.Open(bkfile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			fp.SeekToEnd();
			if(fp.GetLength() < g_arrbakpro[proidx])
			{
				fp.Write(&(packh.m_type2), sizeof(OLDDATEHEADER));
				fp.Write(pPacket, packh.m_datalen);
			}
			fp.Close();
		}
	}
	
	return TRUE;
}

std::string GetStringFromHexString(const unsigned char * str, int length) 
{
	std::string res;
	for (int i = 0; i < length; i++)
	{
		res += formatstr_lit("%02x", str[i]);
	}
	return res;
}

std::string formatstr(const char* fm, ...) {
	char buff[1024 * 16];
	memset(buff, 0, sizeof(buff));
	va_list args;
	va_start(args, fm);
	_vsnprintf(buff, sizeof(buff) - 1, fm, args);
	va_end(args );
	return std::string(buff);
}

std::string formatstr_lit(const char* fm, ...) {
	char buff[1024 * 2];
	memset(buff, 0, sizeof(buff));
	va_list args;
	va_start( args, fm );
	_vsnprintf(buff, sizeof(buff) - 1, fm, args);
	va_end( args );
	return std::string(buff);
}

