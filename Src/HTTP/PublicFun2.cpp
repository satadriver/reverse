#include "stdafx.h"
#include "http.h"
#include "httpsession.h"
#include "PublicFun.h"
#include "httpproxy.h"
#include <afxdisp.h>
#include "thread.h"

int CheckHotmail(HttpSession* s, HttpCommInfo* phttpinfo,
		CString strDesPath, IMessagePtr& iMsg);
//BOOL ConvertUtf8toGbk(LPCTSTR cchString, int lenString, CString &strGbk);

typedef struct SUFFIX
{
	int		suffixlen;
	LPCTSTR	urlsuffix;
	LPCTSTR filesuffix;
	UINT	filetype;
}SUFFIX;

//检查contentype
UINT CheckContenttype(CString &contenttype,CString &suffix)
{
	UINT re=0;//1 图片动画 2 文本 3 特殊 4 压缩 5 未定义 6 QQ 7 eml 8 webmail
	//fname.MakeLower();
	static SUFFIX sub_suffix[18] = 
	{
		{9,		"image/gif",		"gif",	1},
		{10,	"image/jpeg",		"jpg",	1},
		{11,	"image/pjpeg",		"jpg",	1},
		{9,		"text/html",		"htm",	2},
		{10,	"text/plain",		"txt",	2},
		{29,	"application/x-shockwave-flash",	"swf",	1},
		{8,		"text/xml",			"xml",	2},
		{8,		"text/css",			"css",	1},
		{10,	"image/tiff",		"tif",	1},
		{9,		"image/png",		"png",	1},
		{24,	"application/x-javascript",			"js",	1},
		{15,	"application/zip",	"zip",	4},
		{15,	"application/pdf",	"pdf",	3},
		{18,	"application/msword",				"doc",	3},
		{12,	"image/bmpbmp",		"bmp",	1},
		{27,	"application/x-msn-messenger",		"txt",	2},
		{16,	"application/oicq",	"txt",	2},
		{17,	"application/x-tar",				"tar",	4}
	};

	int len = contenttype.GetLength();
	for (int i=0; i<18; i++)
	{
		if ((len == sub_suffix[i].suffixlen) &&
			(memcmp((LPCTSTR)contenttype, sub_suffix[i].urlsuffix, len) == 0))
		{
			suffix = sub_suffix[i].filesuffix;
			return sub_suffix[i].filetype;
		}
	}
	if (strncmp((LPCTSTR)contenttype, "image/", 6) == 0)
	{
		suffix = "jpg";
		return 1;
	}
	if (strncmp((LPCTSTR)contenttype, "text/", 5) == 0)
	{
		suffix = "txt";
		return 2;
	}
	suffix = "txt";
	return 5;
// 	if (strcmp((LPCTSTR)contenttype,"image/jpeg")==0 || strcmp((LPCTSTR)contenttype,"image/jppeg")==0)
// 	{
// 		suffix="jpg";
// 		re=1;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"image/gif")==0)
// 	{
// 		suffix="gif";
// 		re=1;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"image/bmp")==0)
// 	{
// 		suffix="bmp";
// 		re=1;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"image/tiff")==0)
// 	{
// 		suffix="tif";
// 		re=1;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"image/png")==0)
// 	{
// 		suffix="png";
// 		re=1;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"image/")==0)
// 	{
// 		suffix="jpg";
// 		re=1;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"application/x-shockwave-flash")==0)
// 	{
// 		suffix="swf";
// 		re=1;
// 	}	
// 	else if(strcmp((LPCTSTR)contenttype,"application/x-javascript")==0)
// 	{
// 		suffix="js";
// 		re=1;
// 	}	
// 	else if(strcmp((LPCTSTR)contenttype,"text/plain")==0)
// 	{
// 		suffix="txt";
// 		re=2;
// 	}	
// 	else if(strcmp((LPCTSTR)contenttype,"text/html")==0)
// 	{
// 		suffix="htm";
// 		re=2;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"text/xml")==0)
// 	{
// 		suffix="xml";
// 		re=2;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"application/msword")==0)
// 	{
// 		suffix="doc";
// 		re=3;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"application/pdf")==0)
// 	{
// 		suffix="pdf";
// 		re=3;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"application/zip")==0)
// 	{
// 		suffix="zip";
// 		re=4;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"application/x-gzip")==0)
// 	{
// 		suffix="gz";
// 		re=4;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"application/x-tar")==0)
// 	{
// 		suffix="tar";
// 		re=4;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"application/oicq")==0)
// 	{
// 		suffix="txt";
// 		re=6;
// 	}
// 	else if(strcmp((LPCTSTR)contenttype,"application/x-msn-messenger")==0)
// 	{
// 		suffix="msn.txt";
// 		re=6;
// 	}
// 	else
// 	{
//		suffix="txt";
// 		re=5;
// 	}
//	return re;
}
//检查后缀
UINT CheckSuffix(LPCTSTR fname, CString &suffix)
{
	static SUFFIX sub_suffix[28] = 
	{
		{3,		"gif",		"gif",	1},
		{3,		"jpg",		"jpg",	1},
		{3,		"swf",		"swf",	1},
		{3,		"htm",		"htm",	2},
		{4,		"html",		"htm",	2},
		{3,		"asp",		"htm",	2},
		{3,		"php",		"htm",	2},
		{3,		"css",		"txt",	1},
		{2,		"js",		"js",	1},
		{4,		"jpeg",		"jpg",	1},
		{3,		"zip",		"zip",	4},
		{3,		"rar",		"rar",	4},
		{5,		"shtml",	"htm",	2},
		{3,		"exe",		"exe",	10},
		{3,		"ocx",		"ocx",	10},
		{4,		"tiff",		"tif",	1},
		{3,		"tif",		"tif",	1},
		{3,		"png",		"png",	1},
		{3,		"dll",		"dll",	10},
		{2,		"gz",		"gz",	4},
		{3,		"pdf",		"pdf",	3},
		{3,		"doc",		"doc",	3},
		{3,		"txt",		"txt",	2},
		{3,		"bmp",		"bmp",	1},
		{3,		"xml",		"xml",	2},
		{3,		"eml",		"txt",	2},
		{3,		"log",		"txt",	2},
		{3,		"tar",		"tar",	4}
	};
	char* tp = strrchr(fname, '.');
	if (tp!=NULL)
	{
		int l = strlen(++tp);
		if (l>1 && l<=5)
		{
			char buf[8];
			memset(buf, 0, sizeof(buf));
			memcpy(buf, tp, l);
			strlwr(buf);
			for (int i=0; i<28; i++)
			{
				if ((l == sub_suffix[i].suffixlen) &&
					(memcmp(sub_suffix[i].urlsuffix, buf, l)==0))
				{
					suffix = sub_suffix[i].filesuffix;
					return sub_suffix[i].filetype;
				}
			}
		}
	}
	suffix="txt";		
	return 5;
	
//	fname.MakeLower();
//	if(fname.GetLength()>3 && fname.Find('.')>0 && fname.Find('.')<fname.GetLength()-1)
//	{
//		suffix=fname.Mid(fname.ReverseFind('.')+1);
//		if (suffix=="jpeg" || suffix=="jpg")
//		{
//			suffix="jpg";
//			re=1;
//		}
//		else if(suffix=="gif")
//		{
//			re=1;
//		}
//		else if(suffix=="tif" ||suffix=="tiff" )
//		{
//			suffix="tif";
//			re=1;
//		}
//		else if(suffix=="png")
//		{
//			re=1;
//		}
//		else if(suffix=="swf")
//		{
//			re=1;
//		}	
//		else if(suffix=="js")
//		{
//			re=1;
//		}	
//		else if(suffix=="css")
//		{
//			re=1;
//		}	
//		else if(suffix=="bmp")
//		{
//			re=1;
//		}
//		else if(suffix=="txt" || suffix=="htm" || suffix=="xml" || suffix=="shtml" || 
//			suffix=="log" || suffix=="eml" || suffix=="html" || suffix=="php")
//		{
//			suffix="txt";
//			re=2;
//		}
//		//////////////////////////////////////////////////////////////////////////
//		else if(suffix=="doc")
//		{
//			re=3;
//		}
//		else if(suffix=="pdf")
//		{
//			re=3;
//		}
//		//////////////////////////////////////////////////////////////////////////
//		else if(suffix=="zip")
//		{
//			re=4;
//		}
//		else if(suffix=="gz")
//		{
//			re=4;
//		}
//		else if(suffix=="tar")
//		{
//			re=4;
//		}
//		else if(suffix=="rar")
//		{
//			re=4;
//		}
//		else if(suffix=="exe")
//		{
//			re = 10;
//		}
//		else if (suffix=="ocx")
//		{
//			re = 10;
//		}
//		else if (suffix=="dll")
//		{
//			re = 10;
//		}
//		else
//		{
//			fname="txt";
//			suffix="txt";
//			re=5;
//		}
//	}
//	else
//	{
//		fname="txt";
//		suffix="txt";		
//		re=5;
//	}
//	return re;
}

CString GetUnixShortName(CString fname)
{
	int i,k,pos;
	i=fname.ReverseFind('/');
	k=fname.GetLength();
	if (i > 0)
	{
		if(i==k-1)
			fname="";
		else
			fname=	fname.Mid(i+1);
	}
	else
		return fname;
	if(fname=="")
		return fname;
	k=fname.GetLength();
	char *p=fname.GetBuffer(k);
	pos=0;
	while(pos<=k-1 && (*p))
	{
		switch(*(p+pos))
		{
		case '?':
			break;
		case ' ':
			break;
		case ':':
			break;
		case '\"':
			break;
		case '|':
			break;
		case '<':
			break;
		case '>':
			break;
		default:
			pos++;
			continue;
		}
		break;
	}
	fname=fname.Left(pos);
	return fname;
}

BOOL CreateFilename(HttpSession*s)
{
	CString Suffix;
//	LPCTSTR pro = "";
	CString fname;

	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmpdir = s->m_dceprocotol->m_httpMissObj->GetTmpPath();

	if(s->m_Requestline.m_Action==HTTP_POST)	// "POST"
	{
		if (s->m_Requestline.m_Browser==TE_DYNAPASS)
		{
			Suffix = "dynapassup.txt";
		}
		else if (s->m_Requestline.m_Browser==TE_FREEGATE)
		{
			Suffix = "freegateup.txt";
		}
		if(s->m_Requestline.m_Content_Type=="multipart/form-data")
		{
			s->m_Requestline.m_filetype=8;
			Suffix="httpup.txt";
		}
		else if(s->m_Requestline.m_Content_Type=="message/rfc822" || s->m_Requestline.m_Content_Type=="message/rfc821")
		{
			s->m_Requestline.m_filetype=7;
			Suffix="smtp.eml";
		}
		else
		{
			Suffix="httpup.txt";
		}
		s->m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s",
			tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
			s->m_sip, s->m_dip, ++s->m_dceprocotol->m_httpFileNum, Suffix);

	}
	else if(s->m_Requestline.m_Action==HTTP_GET)	// "GET")
	{
		LPCTSTR pro = "";
		if (s->m_Requestline.m_Browser==TE_DYNAPASS)
		{
			pro = "dynapassdown.";
		}
		else if (s->m_Requestline.m_Browser==TE_FREEGATE)
		{
			pro = "freegatedown.";
		}
		else
		{
			pro = "server.";
		}
		if(s->m_Responseline.m_Content_Type=="message/rfc822" || s->m_Responseline.m_Content_Type=="message/rfc821")
		{
			s->m_Responseline.m_filetype=7;
			Suffix="pop3.eml";
		}
		else
		{
			if( s->m_Responseline.m_Content_Type=="application/octet-stream" ||
				s->m_Responseline.m_Content_Type=="" )
			{
				fname=GetUnixShortName(s->m_Requestline.m_URL);
				if(fname != "")
				{
					//检查后缀
					s->m_Responseline.m_filetype=CheckSuffix(fname, Suffix);
				}
			}
			if(Suffix.IsEmpty())
			{
				s->m_Responseline.m_filetype=CheckContenttype(s->m_Responseline.m_Content_Type,Suffix);
				//下载图片 脚本 关闭
				if (s->m_Responseline.m_filetype == 1)
				{
// 					if (s->CaseID.IsEmpty())
// 					{ // 不是案件
// 						if (!v_openflagindeximage)
// 							return FALSE;	// 预警图片关闭
// 					}
// 					else
// 					{ // 是案件
// 						if (!v_opencasehttpimage)
// 							return FALSE;  // 案件图片关闭
// 					}
				}
			}
		}
		s->m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s%s",
				tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
				s->m_sip, s->m_dip, ++s->m_dceprocotol->m_httpFileNum, pro, Suffix);

	}
	return TRUE;
}

CString CheckFileName(CString strName)
{
	int i;
	CString tmpname;	
	i=strName.ReverseFind('\\');
	tmpname=strName.Right(strName.GetLength() - i-1);
	strName = strName.Left(i+1);
	
	tmpname.Remove('\r');
	tmpname.Remove('\n');
	tmpname.Remove('/');
	tmpname.Remove('\\');
	tmpname.Remove('?');
	tmpname.Remove('*');
	tmpname.Remove('|');
	tmpname.Remove(':');
	tmpname.Remove('<');
	tmpname.Remove('>');
	strName+=tmpname;
	
	return strName;
}

//拆分webmail
int DealWebMail(HttpSession *s, HttpCommInfo* phttpInfo, CString& attflst)
{
/*
	if(is163Packet(s))
	{
		mail163 cmail163;
		if(cmail163.DealMailSend(s))
		{
			s->m_Requestline.m_pro="webmail";
			return 1;
		}
	}
	
	else if(isBoundaryPacket(s))
	{
		Boundarymail cBoundarymail;
		if(cBoundarymail.DealMailSend(s,attflst))
		{
			s->m_Requestline.m_pro="webmail";
			return 1;
		}
	}
	else if(isyahooPacket(s))
	{
		mailyahoo cmailyahoo;
		if(cmailyahoo.DealMailSend(s))
		{
			s->m_Requestline.m_pro="webmail";
			return 1;
		}
	}
	else if(isWeiboPacket(s))
	{
		weibo wb;
		if(wb.DealWeiboIndex(s))
		{
			s->m_Requestline.m_pro="wb_sina";
			return 2;
		}
	}
	else if(isQQWeiboPacket(s))
	{
		qqweibo qwb;
		if(qwb.DealWeiboIndex(s))
		{
			s->m_Requestline.m_pro="wb_qq";
			return 2;
		}
	}
	else if(is_weibo_sohu_packet(s))
	{
		if(WeiBoSohu::weibo_sohu_process(s))
		{
			s->m_Requestline.m_pro="wb_sohu";
			return 2;
		}
	}
	else if(WeiboRenmin_is_weibo_packet(s))
	{
		WeiboRenmin WBRM;
		if(WBRM.process_weibo(s))
		{
			s->m_Requestline.m_pro="wb_people";
			return 2;
		}
	}
	else if(WeiboWangyi_is_weibo_packet(s))
	{
		WeiboWangyi wbwy;
		if(wbwy.process_weibo(s))
		{
			return 2;
		}
	}

	else if(isMobileQQWeiboPacket(s))
	{
		mobileqqweibo mqqw;
		if(mqqw.DealWeiboIndex(s))
		{
			s->m_Requestline.m_pro="wb_qq";
			return 2;
		}
	}
	else if(isMobileYYPacket(s))
	{
		mobileyy myy;
		if(myy.DealYYPost(s))
		{
			s->m_Requestline.m_pro="mobile_yy";
			return 2;
		}
	}
*/
/*	else if(int nValue = IsVoipALITONGMessage(s))
	{
		if (VoipALITONGMessage(s,s->m_databuffer,nValue))
		{
			return 2;
		}
	}
	else if (IsVoip97CALLMessage(s))
	{
		if (Voip97CALLMessage(s,s->m_databuffer))
		{
			return 2;
		}
	}
	else if(IsVoipZHCALLMessage(s))
	{
		if (VoipZHCALLMessage(s,s->m_databuffer))
		{
			return 2;
		}
	}
	else if (IsVoipDYTMessage(s))
	{
		if (VoipDYTMessage(s,s->m_databuffer))
		{
			return 2;
		}
	}

	else if (int iValue = IsQQhttpSession(s))
	{
		ProcessQQhttpSession(s,s->m_databuffer,iValue) ;
	}
	else if ( DealVoipCall(s,s->m_databuffer) )
	{
		return 2;
	}

	else if (DealPhoneMap(s,s->m_databuffer))
	{
		return 2;
	}
	else if (DealPhoneMedia(s,s->m_databuffer))
	{
		return 2;
	}
	else if (DealPhoneReadApp(s,s->m_databuffer))
	{
		return 2;
	}
	else if (DealPhoneNumCheat(s,s->m_databuffer))
	{
		return 2;
	}

	else if (DealPhoneAd(s,s->m_databuffer))
	{
		return 2;
	}


	*//*
#define		MaxBoundaryLen		255
	if (s->m_Requestline.m_strBoundary=="" ||
		s->m_Requestline.m_Action ==HTTP_200OK 
		||
				s->Requestline.strBoundary=="xYzZY"
		)	//"HTTP"
		return 1;
	CString strFileName= s->m_realfname;
	if (strFileName.IsEmpty())
	{
		return 0;
	}
	CString strBoundary;
	CString	strNewFile;

	CString strFinalName;
	FILE *fp;
	FILE *fptxt;
	FILE *fpAttFile;

	DWORD	FileLen;
	char *p1,*RawP;
	char *tmpfbuf=0;
	char *pfilebuf = NULL;
	char *FieldHeader=NULL;
	char *tmpHeader=NULL;

	int pos=0;
	int pos1=0;

	int BoundaryLen=0;
	int i=0;
	CString	strHeader;
	CString	strAttFileName;
//	ASSERT(FALSE);
	static Search sh_FileName("filename=");
int error=0;
	try
	{
		FileLen=GetFileLength(strFileName);
		if (FileLen > FILEBUFFERSIZE-10)
		{
			pfilebuf = new char[FileLen+8];
			if (pfilebuf==NULL)
				return -1;
			tmpfbuf = pfilebuf;
		}
		else
			tmpfbuf = s->m_dceprocotol->m_HttpFileBuffer;
error=1;		
		s->m_Requestline.m_strBoundary="--"+s->m_Requestline.m_strBoundary;
		BoundaryLen= s->m_Requestline.m_strBoundary.GetLength();
		Search sh_Boundary;
		if (BoundaryLen<=MaxBoundaryLen)
		{
			LPCTSTR pbound=s->m_Requestline.m_strBoundary;
			sh_Boundary.InitSearch(pbound);
		}
		else
		{
			ASSERT(FALSE);
			WriteLog(HTTPPRO, "strBoudaryLen=%d, str=%s", BoundaryLen, s->m_Requestline.m_strBoundary);
			int tpos = s->m_Requestline.m_strBoundary.Find(';');
			if ((tpos>0) && (tpos<255))
			{
				CString pbound=s->m_Requestline.m_strBoundary.Left(tpos);
				sh_Boundary.InitSearch((LPCTSTR)pbound);
				BoundaryLen = pbound.GetLength();
			}
			else
			{
				CString pbound=s->m_Requestline.m_strBoundary.Left(MaxBoundaryLen);
				sh_Boundary.InitSearch((LPCTSTR)pbound);
			}
		}
error=2;
		fp=fopen(strFileName,"rb");
		if(fp==NULL)
		{
			if (pfilebuf)	delete[] pfilebuf;
			return -1;
		}
		FileLen = fread(tmpfbuf, 1, FileLen, fp);
		fclose(fp);
		memset(tmpfbuf+FileLen, 0, 8);
		p1=tmpfbuf;
		RawP=tmpfbuf;
		strNewFile = strFileName+".webmail.txt";
		fptxt = fopen(strNewFile,"ab");
		if(fptxt==NULL)
		{
			if (pfilebuf)	delete[] pfilebuf;
			return -1;
		}
error=3;	
		pos1=0;
		pos =sh_Boundary.find(tmpfbuf);
		if ((BoundaryLen>MaxBoundaryLen) && (pos>=0))
		{
			if ((pos+BoundaryLen > FileLen) ||
				(memcmp(tmpfbuf+pos, (LPCTSTR)s->m_Requestline.m_strBoundary, BoundaryLen)!=0))
			{
				pos = -1;	// 没有找到
			}
		}
error=4;
		pos1+=pos;
		while ((pos >=0) &&  (tmpfbuf <(p1+FileLen)))
		{
			tmpfbuf+=BoundaryLen;
			pos=Search2CRLF.find(tmpfbuf);
			if (pos < 0)
				break;
			pos1+=pos+4;

			if (pos1>=FileLen)
				break;

			if (pos >= FILEBUFFERSIZE-10)
			{
				FieldHeader=new char[pos+10];
				tmpHeader = FieldHeader;
			}
			else
				tmpHeader = s->m_dceprocotol->m_CHUNKBUFFER;

			if ((memcmp(tmpfbuf, "\r\n", 2) == 0) && (pos >= 2))
			{
				memcpy(tmpHeader, tmpfbuf+2, pos-2);
				memset(tmpHeader+pos-2, 0, 8);
			}
			else
			{
				memcpy(tmpHeader, tmpfbuf, pos);
				memset(tmpHeader+pos, 0, 8);
			}
			tmpfbuf+=(pos+4);

error=5;
			strAttFileName.ReleaseBuffer(0);
			i=sh_FileName.find(tmpHeader);
			if (i > 0)
			{
				strHeader.Format("%s",tmpHeader+i+10);
//				strHeader=strHeader.Mid(i+strlen("filename=")+1);
				i = strHeader.Find('"');
				strHeader= strHeader.Left(i);
// 				CString Gbkstr;
// 				if(ConvertUtf8toGbk(strHeader, strHeader.GetLength(), Gbkstr))
// 				{
// 					strHeader.Format("%s", Gbkstr);
// 				}
				strAttFileName=GetShortName(strHeader);
				strAttFileName=GetUnixShortName(strAttFileName);
				strAttFileName=CheckFileName(strAttFileName);
// 				if (strAttFileName.GetLength() > 60)
// 					strAttFileName= strAttFileName.Right(50);
			}
error=6;
			if (strAttFileName.IsEmpty())
			{
				pos =sh_Boundary.find(tmpfbuf);
				if ((BoundaryLen>MaxBoundaryLen) && (pos>=0))
				{
					if ((pos+BoundaryLen+pos1 > FileLen) ||
						(memcmp(tmpfbuf+pos, (LPCTSTR)s->m_Requestline.m_strBoundary, BoundaryLen)!=0))
					{
						pos = -1;	// 没有找到
					}
				}
				if(pos > 0)
				{
					fwrite(tmpfbuf,pos,1,fptxt);
					tmpfbuf+=pos;
					pos1+=pos;
				}
			}
			else
			{
				CString strAttLine;
				strAttLine.Format("* %s\r\n", strAttFileName);
				fwrite(strAttLine,strAttLine.GetLength(),1,fptxt);
				strAttLine.Empty();
				
				// 转换url编码的文件名
				strAttFileName = ConverHttp2Html(strAttFileName);
				
				// 转换UTF-8编码的文件名
				ConvertUtf8toGbk(strAttFileName, strAttFileName.GetLength(), strAttFileName);

				attflst.Format("%s%s\r\n", (CString)attflst, strAttFileName);
				strAttLine = GetExtName(strAttFileName);
				strAttFileName=strNewFile+".eml@_"+strAttFileName;
				CString savefname;
#ifdef		_DEBUG
				if (strAttFileName.GetLength() > 200)
#else
				if (strAttFileName.GetLength() > 200)
#endif
				{
					savefname.Format("%s§%d.%s", strNewFile, s->m_attfiles.GetSize()+1, strAttLine);
					fpAttFile=fopen(savefname, "wb");
				}
				else
				{
//					savefname = strAttFileName;
					fpAttFile=fopen(strAttFileName, "wb");
				}
error=7;
				if (fpAttFile==NULL)
				{
					savefname.Format("%s§%d.%s", strNewFile, s->m_attfiles.GetSize()+1, strAttLine);
					fpAttFile=fopen(savefname, "wb");
				}
				if (fpAttFile!=NULL)
				{
					if (!savefname.IsEmpty())
					{
						strAttFileName.Format("%s|%s", savefname, (CString)GetShortName(strAttFileName));
					}
					//p=p1;
					pos=sh_Boundary.find(tmpfbuf, FileLen-pos1);
					if ((BoundaryLen>MaxBoundaryLen) && (pos>=0))
					{
						if ((pos+BoundaryLen+pos1 > FileLen) ||
							(memcmp(tmpfbuf+pos, (LPCTSTR)s->m_Requestline.m_strBoundary, BoundaryLen)!=0))
						{
							pos = -1;	// 没有找到
						}
					}
// 					while ((pos <0) &&  (tmpfbuf<(p1+FileLen)) )
// 					{
// 						while((*tmpfbuf!=0) &&  (tmpfbuf<(p1+FileLen)))
// 						{
// 							fwrite(tmpfbuf,1,1,fpAttFile);
// 							tmpfbuf++;
// 							pos1++;
// 						}
// 						while((*tmpfbuf==0) &&  (tmpfbuf<(p1+FileLen)))
// 						{
// 							fwrite(tmpfbuf,1,1,fpAttFile);
// 							tmpfbuf++;
// 							pos1++;
// 						}
// 						pos=sh_Boundary.find(tmpfbuf, FileLen-pos1);
// 					}

					if (pos > 0)
					{
						if ((pos>=2) && (memcmp(tmpfbuf+pos-2, "\r\n", 2)==0))
							fwrite(tmpfbuf, 1, pos-2, fpAttFile);
						else
							fwrite(tmpfbuf,pos,1,fpAttFile);
						pos1+=pos;
						tmpfbuf+=pos;
					}
					else
					{
						pos = FileLen-pos1;
						if ((pos>=2) && (memcmp(tmpfbuf+pos-2, "\r\n", 2)==0))
							fwrite(tmpfbuf, 1, pos-2, fpAttFile);
						else
							fwrite(tmpfbuf,pos,1,fpAttFile);
						pos1+=pos;
						tmpfbuf+=pos;
					}
					fclose(fpAttFile);
error=8;
//					if(strAttFileName.Right(4).CompareNoCase(".zip")==0)
//						DealZipFile(strAttFileName,phttpInfo->attfiles,httptmpzip);
					if ((strAttLine == "rar") || (strAttLine == "zip"))
					{
						int index = s->m_attfiles.Add(strAttFileName+"*3");	// 压缩文件
						if (!DealRarFile(strAttFileName, GetShortName(s->m_realfname), s->m_attfiles, "dce2", attflst))
						{
							CString& tmpstr = s->m_attfiles.ElementAt(index);
							ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
							tmpstr.SetAt(tmpstr.GetLength()-1, '4');
						}
					}
					else
					{
						s->m_attfiles.Add(strAttFileName+"*1");	// 一般文件
					}
				}
				else
				{
					if (FieldHeader)
						delete[] FieldHeader;
					break;
				}
			}
			if (FieldHeader)
				delete[] FieldHeader;
		}
error=9;
		fclose(fptxt);
		if (pfilebuf)
			delete[] pfilebuf;
		s->m_realfname = strNewFile;
		remove(strFileName);
		return 0;
	}
	catch(...)
	{
		WriteLog(HTTPPRO, "DealWebMail() Error!!=%d",error);
	}*/
	return 0;
}
	
//存附件 和 文本附加文件 垃圾过滤
int SaveAffixTo(HttpSession *s, HttpCommInfo* phttpinfo, LPCTSTR strDesPath)
{
	int re=-2;
	CString attfileslst;
	ASSERT(strDesPath[0] != 0);
	if(strDesPath[0] == 0)
		return -3;

	try
	{
		IMessagePtr iMsg(__uuidof(Message));
		_StreamPtr  pStm(__uuidof(Stream));	
		IBodyPartPtr iBp;	
		CString strTemp;
		CString strNewFileName;
		CString AttName;
		int j=-1;
		CString strFN;
		//获得Imsg////////////////////////////////////////////////////////////
		_bstr_t path;
		path = strDesPath;
		_variant_t varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);
		try
		{
			pStm->raw_Open( varOptional, 
				adModeUnknown,   
				adOpenStreamUnspecified,
				NULL,
				NULL);
			pStm->LoadFromFile(path);
		}
		catch(_com_error e)
		{
			WriteLog(HTTPPRO, "SaveAffixTo1() ComError!!");
			return -2;
		}
		IDataSourcePtr iDsrc;
		iMsg->AutoGenerateTextBody=TRUE;
		iDsrc = iMsg;
		try
		{
			iDsrc->OpenObject(pStm,_bstr_t("_Stream"));
		}
		catch(_com_error e)
		{
			WriteLog(HTTPPRO, "SaveAffixTo2() ComError!!");
			return -2;
		}
		//////////////////////////////////////////////////////////////////////////
		//生成邮件头信息
		//垃圾过滤
		//生成emlfield
		//文本附加文件 
		re=CheckHotmail(s, phttpinfo,strDesPath,iMsg);
		if(re!=8)
			return re;
		//////////////////////////////////////////////////////////////////////////
		for(int i=1;i<iMsg->Attachments->Count+1;i++)
		{
			iBp=iMsg->Attachments->Item[i];
			AttName=(char*)iBp->FileName;
			if (AttName.GetLength() <=0 )
			{
				continue;
			}

			// 转换url编码的文件名
			AttName = ConverHttp2Html(AttName);
			
			// 转换UTF-8编码的文件名
			ConvertUtf8toGbk(AttName, AttName.GetLength(), AttName);

			attfileslst+= AttName+",";
			strNewFileName.Format("%s@%s", s->m_realfname, AttName);
			strTemp=GetExtName(AttName);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			if(strTemp=="" || strTemp=="exe" || strTemp=="com" || strTemp=="pif" || strTemp =="scr"/*strTemp=="gif" || strTemp=="jpg"*/)
			{
				s->m_attfiles.Add(strNewFileName+"*2");
				continue;
			}

			CString trueFname = strNewFileName;		// 实际的文件名
#ifdef		_DEBUG
			if (strNewFileName.GetLength() >= 200)
#else
			if (strNewFileName.GetLength() >= 200)
#endif
			{
				trueFname.Format("%s§%d.%s", s->m_realfname, s->m_attfiles.GetSize(), strTemp);
			}

			BOOL suc = FALSE;
			try
			{
				suc = (iBp->SaveToFile(_bstr_t(trueFname)) == 0);
			}
			catch (...)
			{
				suc = FALSE;
			}
// 			if(iBp->SaveToFile(_bstr_t(trueFname))==0)
			if(suc)
			{
				CString addfname;
				if (trueFname != strNewFileName)
				{
					addfname.Format("%s|%s", trueFname, GetShortName(strNewFileName));
				}
				else
				{
					addfname = strNewFileName;
				}
				int index = s->m_attfiles.Add(addfname+"*1");
				// 如果是压缩文件(*.rar/*.zip)就要尝试解压
				if ((strTemp == "rar") || (strTemp == "zip"))
				{
					// 尝试解压
					if (!DealRarFile(addfname, GetShortName(s->m_realfname), s->m_attfiles, "dce2", attfileslst))
					{
						// 解压失败
						CString& tmpstr = s->m_attfiles.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '4');
					}
					else
					{
						// 解压成功
						CString& tmpstr = s->m_attfiles.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '3');
					}
				}
			}
			else
			{
				// 附件输出失败
				s->m_attfiles.Add(strNewFileName + "*2");
			}
		}

		if(SearchBurrishByMail(attfileslst+phttpinfo->m_emlfield))
		{
			s->m_rubbishflag = TRUE;
		}
	}
	catch(...)
	{
		WriteLog(HTTPPRO, "Http SaveAffixTo() Error!!");
		return -3;
	}
	return 8;
}

int DealHotMailFile(HttpSession *s ,HttpCommInfo* phttpinfo)
{
	phttpinfo->m_emlfield.Empty();
	FILE *fp;
	DWORD	FileLen;
	char	*tp=0;
	int i=0;
	int re=8;
	CString strSourceFile;
	if (phttpinfo->m_Action==HTTP_POST && 
		(phttpinfo->m_Content_Type=="message/rfc822" ||
		phttpinfo->m_Content_Type=="message/rfc821"))
	{
		strSourceFile = s->m_realfname;
		FileLen = GetFileLength(strSourceFile);
		if(FileLen<=0)
			return -2;
		try
		{
			char *tmp;
			if (FileLen < FILEBUFFERSIZE-10)
				tmp = s->m_dceprocotol->m_HttpFileBuffer;
			else
			{
				tp = new char[FileLen+10];
				tmp = tp;
			}
			fp = fopen(strSourceFile,"rb");
			if(fp!=NULL)
			{
				FileLen = fread(tmp, 1, FileLen, fp);
				fclose(fp);
			}
			memset(tmp+FileLen, 0, 8);
			i=Search2CRLF.find(tmp, FileLen);
			if (i > 0)
			{
				*(tmp+i)=' ';
				*(tmp+i+1)=' ';
			}
			fp=fopen(strSourceFile,"wb");
			if(fp!=NULL)
			{
				fwrite(tmp,FileLen,1,fp);
				fclose(fp);
			}
			delete[] tp;
		}
		catch (...)
		{
			WriteLog(HTTPPRO, "DealHotMailFile() Error!!");
			delete[] tp;		
		}
		re=SaveAffixTo(s,phttpinfo, s->m_realfname);
	}
	if((phttpinfo->m_Action==HTTP_200OK && 
		(phttpinfo->m_Content_Type=="message/rfc822" || 
		phttpinfo->m_Content_Type=="message/rfc821")))
	{
		re=SaveAffixTo(s,phttpinfo, s->m_realfname);
	}
	return re;
}

int CheckHotmail(HttpSession* s, HttpCommInfo* phttpinfo, CString strDesPath, IMessagePtr& iMsg)
{
	int re=0;
	CaseInfo caseinfo;
	CString emlpro;
	CString emlText;
	FILE * fs=0;
	CString emltxtfile;
	
	try
	{
		phttpinfo->m_emlfield=Getemlfield(iMsg);
		emlpro=Getemlheader(strDesPath, s->m_dceprocotol->m_HttpFileBuffer, FILEBUFFERSIZE);
// 		ASSERT(FALSE);
		CString strText = (char *)iMsg->TextBody;
		static LPCTSTR ss = _T("-------------------------------------------------");
		if (strText.GetLength()>0)
		{
			s->m_Language = CheckSPL(strText);	// 检查语种
		}
		else
		{
			s->m_Language = "";
		}
		
		emlText.Format("%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n", emlpro,
			ss, strText, ss, (char*)iMsg->HTMLBody);
/*lp		if(s->CaseID.IsEmpty() && SearchBurrishByMail(emlpro))
		{
			//垃圾过滤
			s->rubbishflag =TRUE;
		}*/
//		emlText=emlpro+"\r\n-------------------------------------------------\r\n"+GetemlText(iMsg);
		emltxtfile=strDesPath+"@AdditionalTxtFile.txt";
		fs=fopen(emltxtfile,"a+b");
		if(fs>0)
		{
			fwrite(emlText,1,emlText.GetLength(),fs);
			fclose(fs);
			fs=0;
			s->m_attfiles.Add(emltxtfile+"*1");
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "CheckHotmail() Error!!");
		if(fs>0)
			fclose(fs);
		return -3;
	}
	return 8;
}

CString Getemlfield(IMessagePtr& iMsg)
{
	
	CString  strfrom ,strto, strcc, strbcc, strreplyto, strSubject ;
	CString field;
	CString strsendtm;
	DATE sendon=0;	
	try
	{
		strfrom	=(char *)iMsg->From;
		strto	=(char *)iMsg->To;
		strcc	=(char *)iMsg->CC;
		strbcc	=(char *)iMsg->BCC;
		strreplyto=(char *)iMsg->ReplyTo;
		strSubject=(char *)iMsg->Subject;
		iMsg->get_SentOn(&sendon);
		
		COleDateTime sendtm(sendon);
		if(sendtm.GetStatus()==0 && sendon>30000)
		{
			strsendtm=sendtm.Format("%Y-%m-%d %H:%M:%S");
			if(strsendtm!="" )
				field.Format("sendtm= %s\r\n", strsendtm);	
		}	
		if (!strfrom.IsEmpty())
		{
			if (strfrom.GetLength()>800)
				field.Format("%sfrom= %s\r\n", (CString)field, strfrom.GetBuffer(800));
			else
				field.Format("%sfrom= %s\r\n", (CString)field, strfrom);
		}	
		if (!strto.IsEmpty())
		{
			if (strto.GetLength()>800)
				field.Format("%sto= %s\r\n", (CString)field, strto.GetBuffer(800));
			else
				field.Format("%sto= %s\r\n", (CString)field, strto);
		}	
		if(!strcc.IsEmpty())
		{
			if (strcc.GetLength()>800)
				field.Format("%scc= %s\r\n", (CString)field, strcc.GetBuffer(800));
			else
				field.Format("%scc= %s\r\n", (CString)field, strcc);
		}
		if(!strbcc.IsEmpty())
		{
			if (strcc.GetLength()>800)
				field.Format("%sbcc= %s\r\n", (CString)field, strbcc.GetBuffer(800));
			else
				field.Format("%sbcc= %s\r\n", (CString)field, strbcc);
		}	
		if(!strreplyto.IsEmpty())
		{
			if (strreplyto.GetLength()>800)
				field.Format("%sreplyto= %s\r\n", (CString)field, strreplyto.GetBuffer(800));
			else
				field.Format("%sreplyto= %s\r\n", (CString)field, strreplyto);
		}	
		if(!strSubject.IsEmpty())
		{
			if (strSubject.GetLength()>800)
				field.Format("%ssubject= %s\r\n", (CString)field, strSubject.GetBuffer(800));
			else
				field.Format("%ssubject= %s\r\n", (CString)field, strSubject);
		}	
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "Getemlfield() Error!!");
	}
	return field;
}

//CString GetemlText(IMessagePtr& iMsg)
//{
//	CString strText="";
//	CString strHtmText="";
//	try{
//		strText=(char *)iMsg->TextBody;
//		strText+="\n---------------------------------------------------\n";
//		strText+=(char *)iMsg->HTMLBody;
//	}
//	catch (...)
//	{
//		WriteLog(strHttp, "GetemlText() Error!!");
//	}
//	return strText;
//////////////////////////////////////////////////////////////////////////
