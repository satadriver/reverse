#include "stdafx.h"
#include "dcepub.h"
#include <sys\\stat.h>

Search	SearchMailReturnpath("return-path:");
Search	SearchMailReceived("\r\nreceived:");
Search	SearchMailFrom("\r\nfrom:");
Search	SearchMailTo("\r\nto:");

 BOOL CheckIfTrueMail(const char * buff, int len)
{
	char buf[4*1024];
	int txtlen = min(len, sizeof(buf)-4);
	memset(buf+txtlen, 0, 4);
	memcpy(buf, buff, txtlen);
	strlwr(buf);
	if (SearchMailFrom.find(buf) >=0 ||
		SearchMailTo.find(buf) >=0   ||
		SearchMailReturnpath.find(buf) >=0 ||
		SearchMailReceived.find(buf) >=0)
		return TRUE;
	else
		return FALSE;
}
 CString GetExtName(const char* chFileName)
{
	CString strExtName="";
	CString FileName=chFileName;
	
	if (FileName.ReverseFind('.')>0)
		strExtName=FileName.Mid(FileName.ReverseFind('.')+1);
	else
		strExtName="";
	strExtName.MakeLower();
	return strExtName;
}

//*************************************************
//	函数名称:		UnRarDbf
//	描述:			解压缩RAR方式的参数表
//	输入参数:		rarfile RAR文件名(包含路径)
//					undir	解压路径
//					wait	等待解压的时间(秒)
//	输出参数:		
//	返回值:			如果在规定的时间内解压成功就返回TRUE否则返回FALSE
//	其它:			
//*************************************************
BOOL UnRarDbf(LPCTSTR rarfile, LPCTSTR undir, int wait)
{
	if(_access(undir, 0) != 0)
	{
		CreateDir(undir);
	}

	BOOL unCompress = FALSE;
	CString cmd;
	cmd.Format("%s\\bin\\cmdrar.exe e -o+ -p- \"%s\" %s",	// rar
		v_RootPath, rarfile, undir);

	// 开进程的方式调用RAR
	PROCESS_INFORMATION pInfo;
	STARTUPINFO         sInfo;
	sInfo.cb              = sizeof(STARTUPINFO);
	sInfo.lpReserved      = NULL;
	sInfo.lpReserved2     = NULL;
	sInfo.cbReserved2     = 0;
	sInfo.lpDesktop       = NULL;
	sInfo.lpTitle         = NULL;
	sInfo.dwFlags         = STARTF_USESHOWWINDOW;
	sInfo.dwX             = 0;
	sInfo.dwY             = 0;
	sInfo.dwFillAttribute = 0;
	sInfo.wShowWindow     = SW_HIDE;
	if (!CreateProcess(NULL, (char*)(LPCTSTR)cmd, 
		NULL, NULL, FALSE, 0, NULL,	NULL, &sInfo, &pInfo))
	{
		WriteLog("dce", "解压文件%s时 创建进程失败!!", rarfile);
	}
	else
	{
		for (int i=0; i<wait; i++)
		{
			if (127 == (i & 128))
			{
				CreateFlgFile();		// 创建心跳文件
			}
			if (WAIT_OBJECT_0 == WaitForSingleObject(pInfo.hProcess, 1000))
			{
				CloseHandle(pInfo.hProcess);
				CloseHandle(pInfo.hThread);
				return TRUE;
			}
		}

		WriteLog("dce", "解压文件%s超时<%ds>!!", rarfile, wait);
		TerminateProcess(pInfo.hProcess, 0);
		CloseHandle(pInfo.hProcess);
		CloseHandle(pInfo.hThread);
	}
	return FALSE;
}

/************************************************************************/
/* 处理RAR ZIP文件                                                      */
/************************************************************************/
 int DealRarFile(CString rarfilename, CString realfname, CStringArray& attarray, LPCTSTR ProType, CString& attflst)
{
	CString unCompdir;
	unCompdir.Format(v_datapath+"\\tmp\\%s\\rar", ProType);
	if(_access(unCompdir, 0) != 0)
	{
		CreateDir(unCompdir);
	}

	CString outpath;
	outpath.Format("%s\\tmp\\%s", v_datapath, ProType);

	CStringArray RarFileArray;
	RarFileArray.Add(rarfilename);
	BOOL unCompress = FALSE;

	for (int i=0; i<RarFileArray.GetSize(); i++)
	{
		CString& str = RarFileArray.ElementAt(i);
		CString savefname;		// 存储文件名
		CString dispfname;		// 显示文件名
		int pos = str.Find('|');
		if (pos >= 0)
		{
			savefname = str.Left(pos);
			dispfname = str.Mid(pos+1);
		}
		else
		{
			savefname = str;
			dispfname = GetShortName(str);
		}
		CString cmd;
		char shortname[40];
		memset(shortname, 0, sizeof(shortname));
		GetShortPathName(savefname, shortname, 40);
		if (shortname[0]!=0)
		{
			if (stricmp(str.Right(3), "rar")==0)
			{
				cmd.Format("%s\\bin\\cmdrar.exe e -o+ -p- \"%s\" %s",	// rar
					v_RootPath, shortname, unCompdir);
// 				isZip = FALSE;
			}
			else
			{
	//			cmd.Format("%s\\bin\\cuz.exe -qo \"%s\" %s",	// zip
//				cmd.Format("cmd /c \"%s\\bin\\unzip32.exe -oqj %s -d %s\"",	//zip
				cmd.Format("%s\\bin\\rar.exe e -o+ -y -p- \"%s\" %s",	//zip
					v_RootPath, shortname, unCompdir);
// 				isZip = TRUE;
			}

		}
		else
		{
			if (stricmp(str.Right(3), "rar")==0)
			{
				cmd.Format("%s\\bin\\cmdrar.exe e -o+ -p- \"%s\" %s",	// rar
					v_RootPath, str, unCompdir);
// 				isZip = FALSE;
			}
			else
			{
	//			cmd.Format("%s\\bin\\cuz.exe -qo \"%s\" %s",	// zip
//				cmd.Format("%s\\bin\\unzip32.exe -oqj '%s' -d %s",	//zip
				cmd.Format("%s\\bin\\rar.exe e -o+ -y -p- \"%s\" %s",	//zip
					v_RootPath, str, unCompdir);
// 				isZip = TRUE;
			}
		}

		pos = str.ReverseFind('\\');
		rarfilename = str.Mid(pos+1);
		// 开进程的方式调用RAR
		PROCESS_INFORMATION pInfo;
		STARTUPINFO         sInfo;
		sInfo.cb              = sizeof(STARTUPINFO);
		sInfo.lpReserved      = NULL;
		sInfo.lpReserved2     = NULL;
		sInfo.cbReserved2     = 0;
		sInfo.lpDesktop       = NULL;
		sInfo.lpTitle         = NULL;
		sInfo.dwFlags         = STARTF_USESHOWWINDOW;
		sInfo.dwX             = 0;
		sInfo.dwY             = 0;
		sInfo.dwFillAttribute = 0;
		sInfo.wShowWindow     = SW_HIDE;
		if (!CreateProcess(NULL, (char*)(LPCTSTR)cmd, 
			NULL, NULL, FALSE, 0, NULL,	NULL, &sInfo, &pInfo))
		{
			WriteLog("dce", "解压文件%s时 创建进程失败!!", str);
		}
		else if (WAIT_OBJECT_0 != WaitForSingleObject(pInfo.hProcess, 2000))
		{
			// 先检查一下有没有文件被释放出来
			// 如果没有就认为该压缩文件无法释放
			// 否则就再等5秒
			CFileFind finder;
			CString curdir;
			curdir.Format("%s\\*.*", unCompdir);
				
			BOOL bWorking = finder.FindFile(curdir);
			BOOL istmpSucceed = FALSE;
			while (bWorking)
			{
				bWorking = finder.FindNextFile();
				if (finder.IsDirectory())
					continue;
				istmpSucceed = TRUE;
				break;
			}
			finder.Close();
			if (istmpSucceed)
			{
				if (WAIT_OBJECT_0 != WaitForSingleObject(pInfo.hProcess, 5000))
				{
					TerminateProcess(pInfo.hProcess, 0);
				}
			}
			else
			{
				TerminateProcess(pInfo.hProcess, 0);
			}
				
		}
// #ifdef	_DEBUG
// //		system("chcp 936");
// #endif
		CloseHandle(pInfo.hProcess);
		CloseHandle(pInfo.hThread);
		
		CFileFind finder;
		CString curdir;
		CString curfname;
		curdir.Format("%s\\*.*", unCompdir);
		
		BOOL bWorking = finder.FindFile(curdir);
		BOOL isSucceed = FALSE;
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDirectory())
			{
				continue;
			}
			unCompress = TRUE;
			curdir = finder.GetFilePath();
			curfname = finder.GetFileName();
			attflst += curfname + ",";
			CString suffix;
			suffix = GetExtName(curdir);
			SetFileAttributes(curdir,FILE_ATTRIBUTE_NORMAL);
			if (!isSucceed)
			{
				if (i>0)
				{
					attarray.Add(str+"*3");	// 解压成功
				}
				isSucceed = TRUE;
			}
			CString sfname;
			CString dfname;
			CString addfname;
			sfname.Format("%s\\%s_解压_%s", outpath, dispfname, curfname);
#ifdef		_DEBUG
			if (sfname.GetLength()>200)
#else
			if (sfname.GetLength()>200)
#endif
			{
				dfname = GetShortName(sfname);
				sfname.Format("%s\\%s§%d.%s", outpath, realfname, attarray.GetSize() + RarFileArray.GetSize(), suffix);
				addfname.Format("%s|%s", sfname, dfname);
			}
			else
			{
				dfname = sfname;
				addfname = sfname;
			}
			// 附件最大5M
			if((finder.GetLength() > 15000000) || !(suffix=="rar" || suffix=="zip" || suffix=="txt" || suffix=="htm"|| suffix=="html" || suffix=="doc" || suffix=="pdf"  || suffix=="asp"  || suffix=="xls" || suffix=="eml"))
			{
				attarray.Add(addfname+"*2");	// 无实体
				remove(curdir);			
				continue;
			}
			if(MoveFileEx(curdir, sfname, 
				MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
			{
				if ((suffix=="rar") || (suffix=="zip"))
				{
					RarFileArray.Add(addfname);
				}
				else
				{
					if (suffix=="eml")
					{

						DealEmlFile(sfname, GetShortName(realfname), attarray, ProType, attflst);
						
					}

					attarray.Add(addfname+"*1");	// 一般文件
				}
			}
			else
			{
				attarray.Add(addfname+"*2");	// 无实体
				remove(curdir);
			}
		}
		if (!isSucceed)
		{
			if (i>0)
			{
				attarray.Add(str + "*4");	// 解压失败
			}
		}
		finder.Close();

		cmd.Format("del %s\\*.* /F/S/Q", unCompdir);
		system(cmd);
	}
	return unCompress;
}
#include <comdef.h>
#import "..\\..\\lib\\msado15.dll" rename ("EOF","FileEOF") no_namespace 
#import "..\\..\\lib\\cdoex.dll" rename("Folder","CDOFolder") no_namespace

int DealEmlFile(CString rarfilename, CString realfname, CStringArray& attarray, LPCTSTR ProType, CString& attflst,char* buf)
{
//	return 0;
	int re = -2;
	int error = 0;
//	CString attfileslst;
	CString strNewFileName;
	try
	{
error=2;
		// 解码邮件实体
		IMessagePtr diMsg(__uuidof(Message));
		_StreamPtr  dpStm(__uuidof(Stream));	
		IBodyPartPtr diBp;	
		CString strTemp;

//		_bstr_t AttName;
		CString AttName;
		int j= -1;
		CString strFN;
		_bstr_t path;
		path = rarfilename;
		_variant_t varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);
error=3;
		try
		{
			dpStm->raw_Open( varOptional, 
				adModeUnknown,   
				adOpenStreamUnspecified,
				NULL,
				NULL);
			dpStm->LoadFromFile(path);
		}
		catch(_com_error e)
		{
//			WriteLog(POP3PRO, "SaveAffixTo() comErr1!!");
			return -2;
		}
error=4;		
		IDataSourcePtr iDsrc;
		diMsg->AutoGenerateTextBody=TRUE;
		iDsrc = diMsg;

		try
		{
			iDsrc->OpenObject(dpStm,_bstr_t("_Stream"));
		}
		catch(_com_error e)
		{
//			WriteLog(POP3PRO, "SaveAffixTo() comErr2!!");
			return -2;
		}
error=5;

		// 提取正文
		CString emlText = (char *)diMsg->TextBody;

		// 写附加文件
		FILE * fs = 0;
		CString emltxtfile;
		emltxtfile = rarfilename + "@AdditionalTxtFile.txt";
		fs = fopen(emltxtfile, "wb");
		if(fs>0)
		{
			#define 	 SS _T("\r\n\r\n-------------------------------------------------\r\n\r\n")

			CString str;
			// 写邮件属性
			if(buf == NULL)
			{
				CString tmpstr;
				int f_len = GetFileLength(rarfilename);
				buf = tmpstr.GetBufferSetLength(f_len+10);
				str = Getemlheader(rarfilename, buf, FILEBUFFERSIZE);

				fwrite((LPCTSTR)str, 1, str.GetLength(), fs);
				tmpstr.ReleaseBuffer();
				buf = NULL;
			}
			else
			{
				str = Getemlheader(rarfilename, buf, FILEBUFFERSIZE);

				fwrite((LPCTSTR)str, 1, str.GetLength(), fs);

			}
			fwrite(SS, 1, sizeof(SS)-1, fs);
			// 写正文
			fwrite((LPCTSTR)emlText, 1, emlText.GetLength(), fs);
			fwrite(SS, 1, sizeof(SS)-1, fs);
			// 写HTML格式的正文
			str = (char*)diMsg->HTMLBody;
			fwrite((LPCTSTR)str, 1, str.GetLength(), fs);
			fclose(fs);
			attarray.Add(emltxtfile + "*1");
		}
		else
		{
//			WriteLog(POP3PRO, "打开文件<%s>失败!");
			return -4;
		}


		// 保存附件
error=6;
		for(int i=1;i<diMsg->Attachments->Count+1;i++)
		{
error=61;
			diBp=diMsg->Attachments->Item[i];
			AttName=(char*)diBp->FileName;
			if (AttName.GetLength() <=0 )
			{
				continue;
			}

			// 转换url编码的文件名
			AttName = ConverHttp2Html(AttName);
			
			// 转换UTF-8编码的文件名
			ConvertUtf8toGbk(AttName, AttName.GetLength(), AttName);

//			attfileslst += AttName + ",";
			strNewFileName.Format("%s@%s", rarfilename, AttName);
			strTemp=GetExtName(AttName);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			if(strTemp=="" || strTemp=="exe" || strTemp=="com" || strTemp=="pif" || strTemp =="scr"/*strTemp=="gif" || strTemp=="jpg"*/)
			{
				attarray.Add(strNewFileName + "*2");
				continue;
			}
			if (strTemp == "eml")
			{
				DealEmlFile(strNewFileName, realfname,attarray, "dce7", attflst,buf);
			}
			CString trueFname = strNewFileName;		// 实际的文件名
#ifdef		_DEBUG
			if (strNewFileName.GetLength() >= 50)
#else
			if (strNewFileName.GetLength() >= 200)
#endif
			{
				CString outpath;
				outpath.Format("%s\\tmp\\%s", v_datapath, ProType);
				trueFname.Format("%s\\%s§%d.%s", outpath, realfname, attarray.GetSize() + attarray.GetSize(), strTemp);
			}
error=7;
			BOOL suc = FALSE;
			try
			{
				suc = (diBp->SaveToFile(_bstr_t(trueFname)) == 0);
			}
			catch (...)
			{
				suc = FALSE;
			}
//			if(iBp->SaveToFile(_bstr_t(trueFname))==0)
			if(suc)
			{
error=8;
				CString addfname;
				if (trueFname != strNewFileName)
				{
					addfname.Format("%s|%s", trueFname, GetShortName(strNewFileName));
				}
				else
				{
					addfname = strNewFileName;
				}
				int index = attarray.Add(addfname+"*1");
				// 如果是压缩文件(*.rar/*.zip)就要尝试解压
				if ((strTemp == "rar") || (strTemp == "zip"))
				{
					// 尝试解压
					if (!DealRarFile(addfname, realfname, attarray, ProType, attflst))
					{
						// 解压失败
						CString& tmpstr = attarray.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '4');
					}
					else
					{
						// 解压成功
						CString& tmpstr = attarray.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '3');
					}
				}
			}
			else
			{
				// 附件输出失败
				attarray.Add(strNewFileName + "*2");
			}
		}


	}
	catch(...)
	{
//		WriteLog(dce, "Mail SaveAffixTo() Error=%d!!", error);
		return -3;
	}
	return 0;
}

CString NetBytestoAnsi(LPCTSTR strMessage, int CodePage/* = CP_ACP*/)
{
	WCHAR wcTemp[5000];
	WCHAR *pwc = wcTemp;
	WCHAR *p1=NULL;
//	WCHAR *wcTemp = NULL;
//	char *MultiBute = NULL;
	char  MultiBute[10000];
	char  *pMultiBute = MultiBute;
	char  *p2=NULL;
	CString strRest;
	
	int  i=  MultiByteToWideChar  (  CP_UTF8  ,  0  , strMessage , -1  , NULL , 0 );
	if( i == 0 )
		return "";
	if (i>sizeof(wcTemp)-8)
	{
		p1 = new WCHAR[i+2];
		pwc = p1;
	}
	memset(pwc, 0, 4);
	if(MultiByteToWideChar  (  CP_UTF8  ,  0  ,strMessage,  -1,  pwc  ,  i) == 0 )
	{
		delete[] p1;
		return "";
	}
	
	i=  WideCharToMultiByte(CodePage, 0, pwc, -1, NULL, 0, NULL, NULL); 
	if( i == 0 )
	{
		delete[] p1;  
		return "";
	}
	
	if (i > sizeof(MultiBute)-8)
	{
		p2 = new char[i + 8];
		pMultiBute = p2;
	}
	memset(pMultiBute, 0, 4);
	if(WideCharToMultiByte  (CodePage, 0, pwc, -1, pMultiBute, i, NULL, NULL ) != 0 ) 
		strRest = pMultiBute;
	
	delete[] p1;
	delete[] p2;
	
	return strRest;
}
void SWChar2Char1(WCHAR* string, int wlen,char* buffer,int& buflen)
{
	buflen=WideCharToMultiByte(CP_ACP,NULL,string,wlen,buffer,buflen,NULL,NULL);
}
CString ConverHttp2Html(CString strcontent)
{
int error=0;
int strlen;
int totlelen=strcontent.GetLength();
	try
	{
		int pos = strcontent.Find('%');
		if (pos == -1)
			return strcontent;
		CString str;
		
		BYTE *tp = (BYTE*)str.GetBufferSetLength(strcontent.GetLength()+20);
		//	::GetWindowText(m_hWnd, rString.GetBufferSetLength(nLen), nLen+1);
		//	rString.ReleaseBuffer();
		memcpy(tp, strcontent, pos);
		strlen = pos;
		tp += pos;
error=1;
		char *p = (char*)(LPCTSTR)strcontent + pos;
		int contentlen=pos;
		while (*p != 0)
		{
			if (contentlen>=totlelen)
			{
				break;
			}
			if (*p == '%')
			{
				if(p[1] == 0)
				{
					break;
				}
				else if(p[2] == 0) 
				{
					p++;
					contentlen++;
					continue;
				}
				else
				{

					if(p[1]=='u') // unenscape
					{

						DWORD value1,value2;
						sscanf(p+2, "%2x%2x", &value1,&value2);
						unsigned char inbuf[4];
						inbuf[0]=(BYTE)value2;
						inbuf[1]=(BYTE)value1;

					


						char tmp[4];
				
						int tmplen=4;
  						SWChar2Char1((WCHAR*)inbuf, 2 ,tmp,tmplen);
						
						*tp++=(BYTE)tmp[0];
						*tp++=(BYTE)tmp[1];
						strlen+=2;
						p+=6;
						contentlen+=6;
					}
					else 
					{
						BYTE b = 0;
						if ((p[1]>='0') && (p[1]<='9'))
						{
							b = p[1] - '0';
						}
						else if ((p[1]>='A') && (p[1]<='F'))
						{
							b = p[1] - 'A' + 10 ;
						}
						else if ((p[1]>='a') && (p[1]<='f'))
						{
							b= p[1] - 'a' + 10;
						}
						else
						{
							// 转换失败
							return strcontent;
						}
						if ((p[2]>='0') && (p[2]<='9'))
						{
							*tp++ = b*16 + p[2] - '0' ;
						}
						else if ((p[2]>='A') && (p[2]<='F'))
						{
							*tp++ = b*16 + p[2] - 'A' + 10 ;
						}
						else if ((p[2]>='a') && (p[2]<='f'))
						{
							*tp++ = b*16 + p[2] - 'a' + 10 ;
						}
						else
						{
							// 转换失败
							return strcontent;
						}
						strlen++;
						p+=3;
						contentlen+=3;
					}



				
				}
			}
			else
			{
				*tp++ = *p++;
				contentlen++;
				strlen++;
			}
		}
error=3;
		if (strlen > 0)
		{
			//strlen
			str.ReleaseBuffer(strlen);
		}
error=4;
		return str;
	}
	catch (...)
	{
		WriteLog("dce","ConverHttp2Html error =%d,strlen=%d",error,strlen);
		if (_access("e:\\netspy\\log\\dce\\httphtml.txt",0)!=0)
		{
			FILE* fp=fopen("e:\\netspy\\log\\dce\\httphtml.txt","wb");
			if (fp!=NULL)
			{
				fwrite((char*)(LPCSTR)strcontent,strcontent.GetLength(),1,fp);
				fclose(fp);
			}
		}
	}
	return "";
}

CString Getemlheader(LPCTSTR emlfilename, char* buf, int bufsize)
{
	ASSERT(bufsize > 1024*32);
	CString strheader;
	int flen = GetFileLength(emlfilename);
	memset(buf, 0x20, 8);
	CFile file;
	if (file.Open(emlfilename, CFile::modeRead))
	{
		while (flen > 0)
		{
			int len = min(flen, 16*1024);		// 一次读16K
			int rlen = file.Read(buf + 8, len);
			int pos = Search2CRLF.find(buf, rlen + 8);
			if(pos < 0)
			{
				pos = Search2CRLFUnix.find(buf, rlen + 8);
			}
			if (pos > 0)
			{
				if (pos > 8)
				{
					strheader += CString(buf + 8, pos - 8);
				}
				break;
			}
			if (rlen != len)
			{
				break;
			}
			else
			{
				strheader += CString(buf + 8, rlen);
				memcpy(buf, buf + rlen, 8);
				flen -= rlen;
			}
		}
		file.Close();
	}
	return strheader;
}

// static PROCESSIMSGPROXYDATA pCheckFun[10]= \
// 	{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
// 
// BOOL CheckImsgProxy(IMSGPROXY& ImsgProxy)
// {
// 	for (int i=0; i<10; i++)
// 	{
// 		if (pCheckFun[i]==NULL)
// 			return FALSE;
// 		else if (pCheckFun[i](&ImsgProxy))
// 			return TRUE;
// 	}
// 	return FALSE;
// }

// typedef BOOL* PROCESSIMSGPROXYDATA (IMSGPROXY&, CString)
//  void SetImsgProxyFunction(PROCESSIMSGPROXYDATA pfunction)
// {
// 	ASSERT(pfunction!=NULL);
// 	for (int i=0; i<10; i++)
// 	{
// 		if (pCheckFun[i]== pfunction)
// 			break;
// 		if (pCheckFun[i]==NULL)
// 		{
// 			pCheckFun[i] = pfunction;
// 			break;
// 		}
// 	}
// }

 BOOL GetHeapInfo(HANDLE heapH, DWORD& ValidMemSize, DWORD& TotalMemSize, int& BlockCount)
{
	PROCESS_HEAP_ENTRY heapinfo;
	memset(&heapinfo, 0, sizeof(heapinfo));
	heapinfo.cbData = sizeof(heapinfo);
	heapinfo.wFlags = PROCESS_HEAP_REGION;
	BlockCount=0;
	ValidMemSize=0;
	TotalMemSize=0;
	int i = 0;
	while(1)
	{
		i++;
		if (!HeapWalk(heapH, &heapinfo))
			break;
		if ((heapinfo.wFlags == 1) && (BlockCount<=heapinfo.iRegionIndex))
		{
			ValidMemSize += heapinfo.Region.dwCommittedSize;
			TotalMemSize += heapinfo.Region.dwUnCommittedSize;
			BlockCount++;
			heapinfo.lpData = heapinfo.Region.lpLastBlock;
			heapinfo.cbData = heapinfo.Region.dwUnCommittedSize;
			heapinfo.wFlags=2;
		}
		if (i >= 100)
			return FALSE;
//		else
//		{
//			printf("error!!\n");
//		}
	}
	TotalMemSize += ValidMemSize;
	return TRUE;
}

// 函数名称:	ConvertUtf8toGbk
// 函数描述:    判断一个字符串, 如果是Utf-8, 转换为Gbk
// 函数参数:	[in]cchString	要判断的文本串
// 				[in]lenString	文本串的大小
//				[out]strGbk		转换后的文本, 长度为strGbk.GetLength()
// 返回值:		判断串为为Utf-8且转换成功返回TRUE, 否则返回 FALSE
BOOL ConvertUtf8toGbk(LPCTSTR cchString, int lenString, CString &strGbk)
{
	int Err = 0;
	BOOL bUtf8 = FALSE;
	try{

	Err = 10;
		DWORD rNeed = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, cchString, lenString, NULL, 0);
		if(rNeed > 0)
		{
	Err = 20;
			DWORD dwTrans ;
			CString str;
			WCHAR *pUniBuff = (WCHAR*)str.GetBufferSetLength(rNeed*2);
			dwTrans = MultiByteToWideChar(CP_UTF8, 0, cchString, lenString, pUniBuff, rNeed);
			if(dwTrans > 0)
			{
				rNeed = WideCharToMultiByte(CP_ACP, 0, pUniBuff, dwTrans, NULL, 0, NULL, NULL);
				if(rNeed > 0)
				{
	Err = 21;
					char *pGbk = strGbk.GetBufferSetLength(rNeed);
					dwTrans = WideCharToMultiByte(CP_ACP, 0, pUniBuff, dwTrans, pGbk, rNeed, NULL, NULL);
					if(dwTrans > 0)
					{
						bUtf8 = TRUE;
					}
	Err = 23;
					strGbk.ReleaseBuffer(-1);
				}
			}
	Err = 29;
		}
	}
	catch (...)
	{
		WriteLog("error", "ConvertUtf8toGbk Catch Error, No:%d!", Err);
	}

	
	return bUtf8;
}



// Big5 => GBK：
// い地チ㎝瓣 --> 中華人民共和國

void CChineseCodeLib::BIG52GBK(char *szBuf)
{
	if(!strcmp(szBuf, ""))
	return;
	int nStrLen = strlen(szBuf);
	wchar_t *pws = new wchar_t[nStrLen + 1];
	try
	{
		int nReturn = MultiByteToWideChar(950, 0, szBuf, nStrLen, pws, nStrLen + 1);
		BOOL bValue = false;
		nReturn = WideCharToMultiByte(936, 0, pws, nReturn, szBuf, nStrLen + 1, "?", &bValue);
		szBuf[nReturn] = 0;
		delete[] pws;
	}
	catch(...)
	{
		delete[] pws;
	}
}

//---------------------------------------------------------------------------
// GBK => Big5
// 中華人民共和國 --> い地チ㎝瓣
void CChineseCodeLib::GBK2BIG5(char *szBuf)
{
	if(!strcmp(szBuf, ""))
		return ;
	int nStrLen = strlen(szBuf);
	wchar_t *pws = new wchar_t[nStrLen + 1];
	__try
	{
		MultiByteToWideChar(936, 0, szBuf, nStrLen, pws, nStrLen + 1);
		BOOL bValue = false;
		WideCharToMultiByte(950, 0, pws, nStrLen, szBuf, nStrLen + 1, "?", &bValue);
		szBuf[nStrLen] = 0;
	}
	__finally
	{
		delete[] pws;
	}
}
//----------------------------------------------------------------------------
// GB2312 => GBK
// 中华人民共和国 --> 中華人民共和國
void CChineseCodeLib::GB2GBK(char *szBuf)
{
	if(!strcmp(szBuf, ""))
		return;
	int nStrLen = strlen(szBuf);
	WORD wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	int nReturn = LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nStrLen, NULL, 0);
	if(!nReturn)
		return;
	char *pcBuf = new char[nReturn + 1];
	__try
	{
		wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
		LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
		strncpy(szBuf, pcBuf, nReturn);
	}
	__finally
	{
		delete[] pcBuf;
	}
}
//---------------------------------------------------------------------------
// GBK =〉GB2312
// 中華人民共和國 --> 中华人民共和国
void CChineseCodeLib::GBK2GB(char *szBuf)
{
	if(!strcmp(szBuf, ""))
		return;
	int nStrLen = strlen(szBuf);
	WORD wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
	int nReturn = LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nStrLen, NULL, 0);
	if(!nReturn)
		return;
	char *pcBuf = new char[nReturn + 1];
	__try
	{
		wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
		LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
//		strncpy()
		strncpy(szBuf, pcBuf, nReturn);
	}
	__finally
	{
		delete []pcBuf;
	}
}

void CChineseCodeLib::GBBIG5(char *szBuf)
{
	GB2GBK(szBuf);
	GBK2BIG5(szBuf);
}

void CChineseCodeLib::BIG5GB(char *szBuf)
{
	BIG52GBK(szBuf);
	GBK2GB(szBuf);

}

// 
// 调用示例
// 
// ......
// 
// 　 char sourceEncode[255];
// 　 char szBuf[1024];
// 
// 　 // 从 GB2312 转到 GBK
// 　 strcpy(szBuf, sourceEncode);
// 　 GB2GBK(szBuf);
// 
// 　 // 从GB2312 转到 BIG5，通过 GBK 中转
// 　 strcpy(szBuf, sourceEncode);
// 　 GB2GBK(szBuf);
// 　 GBK2BIG5(szBuf);
// 
// ......
// 
//}


  // 把UTF-8转换成Unicode
void CChineseCodeLib::UTF_8ToUnicode(WCHAR* pOut,char *pText)
{
	char* uchar = (char *)pOut;

	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

	return;
}

// Unicode 转换成UTF-8 
void CChineseCodeLib::UnicodeToUTF_8(char* pOut,WCHAR* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
	char* pchar = (char *)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}


// 把Unicode 转换成 GB2312 
void CChineseCodeLib::UnicodeToGB2312(char* pOut,unsigned short uData)
{
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
	return;
}     


// GB2312 转换成　Unicode
void CChineseCodeLib::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)
{
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	return;
}


//GB2312 转为 UTF-8
void CChineseCodeLib::GB2312ToUTF_8(CString& pOut,char *pText, int pLen)
{
	char buf[4];
	char* rst = new char[pLen + (pLen >> 2) + 2+10];

	memset(buf,0,4);
	memset(rst,0,pLen + (pLen >> 2) + 2+10);

	int i = 0;
	int j = 0;      
	while(i < pLen)
	{
		   //如果是英文直接复制就可以
		   if( *(pText + i) >= 0)
		   {
				   rst[j++] = pText[i++];
		   }
		   else
		   {
				   WCHAR pbuffer;
				   Gb2312ToUnicode(&pbuffer,pText+i);
               
				   UnicodeToUTF_8(buf,&pbuffer);
               
				   unsigned short int tmp = 0;
				   tmp = rst[j] = buf[0];
				   tmp = rst[j+1] = buf[1];
				   tmp = rst[j+2] = buf[2];
               
               
				   j += 3;
				   i += 2;
		   }
	}
	rst[j] = '\0';

	//返回结果
//	pOut = rst;  
	pOut.Format("%s",rst);
	delete[] rst;   

	return;
}


//UTF-8 转为 GB2312
void CChineseCodeLib::UTF_8ToGB2312(CString &pOut, char *pText, int pLen)
{
	char * newBuf = new char[pLen];
	char Ctemp[4];
	memset(Ctemp,0,4);

	int i =0;
	int j = 0;

	while(i < pLen)
	{
		if(pText[i] > 0)
		{
				newBuf[j++] = pText[i++];                       
		}
		else                 
		{
				WCHAR Wtemp;
				UTF_8ToUnicode(&Wtemp,pText + i);
    
				UnicodeToGB2312(Ctemp,Wtemp);
        
				newBuf[j] = Ctemp[0];
				newBuf[j + 1] = Ctemp[1];

				i += 3;    
				j += 2;   
		}
	}
	newBuf[j] = '\0';
	pOut = newBuf;
	delete []newBuf;

	return; 
}
using namespace std;


std::string UrlEncode(const std::string& src)
{
    static    char hex[] = "0123456789ABCDEF";
    std::string dst;
    
    for (size_t i = 0; i < src.size(); i++)
    {
        unsigned char ch = src[i];
        if (isalnum(ch))
        {
            dst += ch;
        }
        else
            if (src[i] == ' ')
            {
                dst += '+';
            }
            else
            {
                unsigned char c = static_cast<unsigned char>(src[i]);
                dst += '%';
                dst += hex[c / 16];
                dst += hex[c % 16];
            }
    }
    return dst;
}
//url解码 http://tieba.baidu.com/f?kw=%C0%B1%BD%B7 转换成 http://tieba.baidu.com/f?kw=辣椒
std::string UrlDecode(const std::string& src)
{
    std::string dst, dsturl;
    int srclen = src.size();
    for (size_t i = 0; i < srclen; i++)
    {
        if (src[i] == '%')
        {
            if(isxdigit(src[i + 1]) && isxdigit(src[i + 2]))
            {
                char c1 = src[++i];
                char c2 = src[++i];
                c1 = c1 - 48 - ((c1 >= 'A') ? 7 : 0) - ((c1 >= 'a') ? 32 : 0);
                c2 = c2 - 48 - ((c2 >= 'A') ? 7 : 0) - ((c2 >= 'a') ? 32 : 0);
                dst += (unsigned char)(c1 * 16 + c2);
            }
        }
        else
            if (src[i] == '+')
            {
                dst += ' ';
            }
            else
            {
                dst += src[i];
            }
    }

 //   int len = dst.size();
    
//     for(unsigned int pos = 0; pos < len;)
//     {
//         unsigned int nvalue = utf8_decode((char *)dst.c_str(), &pos);
//         dsturl += (unsigned char)nvalue;
//     }
    return dst;
}
//检查文件的类型（rar，pgp等），如果是rar判断是否是加密的
//返回值6是rar加密改名 5 8是pgp加密文件 7是rar改名 9是pgp加密改名
char checkfiletype(CString filename,CString houzhui)
{
	int flen = GetFileLength(filename);
	if (flen<45)
	{
		return '0';
	}
	char begbuf[20],endbuf[44];
	FILE *fp;
	fp=fopen(filename,"rb");
	if(fp!=NULL)
	{
		fread(begbuf,1,20,fp);
		fseek(fp,flen-44,SEEK_SET);
		fread(endbuf,1,44,fp);
		fclose(fp);

		if (memcmp(begbuf,"Rar",3)==0)
		{
			if (memcmp(begbuf+10,"\x80",1)==0)
			{
				if (houzhui!="rar")
				{
					return '6';
				}
				else
				{
					return '5';
				}
			}
			else if (houzhui!="rar")
			{
				return '7';
			}
		}
		else if (memcmp(endbuf,"PGPSDA",6)==0)
		{
			if (houzhui=="exe")
			{
				return '8';
			}
			else
			{
				return '9';
			}
		}
	}
	return '0';
}