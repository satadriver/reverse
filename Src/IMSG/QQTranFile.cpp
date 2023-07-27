// QQTranFile.cpp: implementation of the QQTranFile class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "QQTranFile.h"
#include "imsg.h"
#include "zlib/zlib.h"

#pragma comment(lib, "zlib/zlib.lib")


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/* Uncompress gzip data */
int gzdecompress(Byte *zdata, uLong nzdata,                 
        Byte *data, uLong *ndata)
{
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */
    static char dummy_head[2] = 
    {
        0x8 + 0x7 * 0x10,
        (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;
    if(inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) return -1;
    //if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
        if(err != Z_OK )
        {
            if(err == Z_DATA_ERROR)
            {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
                {
                    return -1;
                }
            }
            else return -1;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK) return -1;
    *ndata = d_stream.total_out;
    return 0;
}
 
void md5str2ascii(char* phashstr,char*hashkey)
{

	int j=0;
	for(int i=0;i<16;i++)
	{
		int pp;
		sscanf(&phashstr[j],"%02x",&pp);
		hashkey[i]= (unsigned char)pp;
		j+=2;
	}
}


 void SWChar2Char1(WCHAR* string,char* buffer,int& buflen)
{
	buflen=WideCharToMultiByte(CP_ACP,NULL,string,-1,buffer,buflen,NULL,NULL);
}


QQTranFileSession::QQTranFileSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite=FALSE;
	m_tranfilelen=0;
	m_packetnum=0;
	m_recvpacketnum=0;
	m_filterbuf=NULL;
	m_bSaveFile=FALSE;
	m_file.m_hFile=NULL;
}

QQTranFileSession::~QQTranFileSession()
{

	if(m_filterbuf)
	{
		delete[] m_filterbuf;
	}
	
	if(m_file.m_hFile!=NULL) //收包失败,删除
	{
		m_file.Close();
		DeleteFile(m_tranfilepath);
		//还原内容失败,但要把文件名写下
		WriteQQFileIndexFile("");
	}
}

int QQTranFileSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{


	int errno1=0;
	DWORD  tmp_packetnum=0;

 	try{
			int flg=IsQQTranFilePacket(packh,pPacket);
	if(flg==1)//ssl 中转
	{
errno1=11;
		DWORD userid=0;
		userid=htonl(*(DWORD*)(pPacket+9));
errno1=12;
		m_from.Format("%u",userid);
		m_userid=m_from;
		
errno1=13;		
		unsigned char * pcur=(unsigned char *)pPacket+0x2c+0x3a;
		
		char buf[300];
		memset(buf,0,300);
		int buflen=300;
		SWChar2Char1((unsigned short *)pcur,buf,buflen);
		if(strlen(buf)>0)
		{
			m_tranfilename.Format("%s",buf);
			
			
			if(!m_iswrite)
			{
				m_iswrite=TRUE;
				if(!m_userid.IsEmpty())
					WriteQQFileIndexFile("");	 
			}
			
			
		}
errno1=14;
	}
	else if(flg==2)//tcp p2p qq号
	{
errno1=20;

		DWORD userid=0;
		userid=htonl(*(DWORD*)(pPacket+11));
		
		if(memcmp(m_csip,packh.m_sip,4)==0)
		{
			m_from.Format("%u",userid);
		}
		else 
		{
			m_to.Format("%u",userid);
		}

	}
	else if(flg==3)//tcp 点对点文件名
	{

		//没有联系人和发件人,不还原
		if(m_from.IsEmpty() || m_to.IsEmpty())
		{
			return 0;
		}
errno1=30;
		unsigned char * pcur=(unsigned char *)pPacket+0xc+4+0x17;
		if (memcmp(packh.m_sip, m_csip, 4) != 0)
		{
			// 交换IP方向
			CString tstr = m_sip;
			m_sip = m_dip;
			m_dip = tstr;
			tstr = m_dmac;
			m_dmac = m_smac;
			m_smac = tstr;
			DWORD ip;
			memcpy(&ip, m_csip, 4);
			memcpy(m_csip, m_cdip, 4);
			memcpy(m_cdip, &ip, 4);
			m_AuthInfo.SwapAuthInfo();
			
			CString tmp;
			tmp=m_to;
			m_to=m_from;
			m_from=tmp;
		}
	 	m_userid=m_from;
		
errno1=31;	
		char buf[300];
		memset(buf,0,300);
		int buflen=300;
		SWChar2Char1((unsigned short *)pcur,buf,buflen);
		if(strlen(buf)>0)
		{
			m_tranfilename.Format("%s",buf);
			
			if(!m_from.IsEmpty() && !m_to.IsEmpty())
			{
				if(!m_iswrite)
				{
					m_iswrite=TRUE;
					WriteQQFileIndexFile("");	 
				}
			}
			
		}

	}
	else if(flg==4)//udp qq号 服务器中转 //udp id为目标的  tcp id为源的
	{
errno1=40;
		DWORD userid=0;
		userid=htonl(*(DWORD*)(pPacket+0x17));
		
		if(memcmp(m_csip,packh.m_sip,4)==0)
		{
			m_to.Format("%u",userid);
		}
		else 
		{
			m_from.Format("%u",userid);
		}

	
	}
	else if(flg==5)//udp
	{

		//没有联系人和发件人,不还原
		if(m_from.IsEmpty() || m_to.IsEmpty())
		{
			return 0;
		}

		errno1=50;
		unsigned char * pcur=(unsigned char *)pPacket+0x17;	
		if (memcmp(packh.m_sip, m_csip, 4) != 0)
		{
			// 交换IP方向
			CString tstr = m_sip;
			m_sip = m_dip;
			m_dip = tstr;
			tstr = m_dmac;
			m_dmac = m_smac;
			m_smac = tstr;
			DWORD ip;
			memcpy(&ip, m_csip, 4);
			memcpy(m_csip, m_cdip, 4);
			memcpy(m_cdip, &ip, 4);
			m_AuthInfo.SwapAuthInfo();
			
			CString tmp;
			tmp=m_to;
			m_to=m_from;
			m_from=tmp;
		
		}
		m_userid=m_from;
		
errno1=51;
		m_tranfilelen=htonl(*(DWORD*)(pPacket+11));
		tmp_packetnum=htonl(*(DWORD*)(pPacket+15));
	 
		errno1=2;
	//	memcpy(m_tranfilemd5,pPacket+535,16);
	 
		char buf[300];
		memset(buf,0,300);
		int buflen=300;
		SWChar2Char1((unsigned short *)pcur,buf,buflen);
errno1=52;
		if(buflen>0)
		{
			errno1=53;
			if(m_tranfilename.IsEmpty())
			{

				CString tmpfilename;
				SYSTEMTIME now;
				GetLocalTime(&now);

				m_tranfilename.Format("%04d%02d%02d%02d%02d%02d_%s_%s_online_%s",
									now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
									m_sip, m_dip, buf);
		
				//m_tranfilename.Format("%s",buf);
				
				//如果系统类型为临针，则存储到指定目录中
				if (v_programtype == SYSTEM_SLEUTH)
				{
					SYSTEMTIME st;
					GetLocalTime(&st);
					CString tm;
					tm.Format("%04u%02u%02u",st.wYear,st.wMonth,st.wDay);

					CString tmpfolderpath,tmppath;
					tmpfolderpath.Format("v:\\netspy\\tmp");
					//tmppath.Format("V:\\netspy\\QQTranfile\\%s",tm);
					
					if(access(tmpfolderpath,0)!=0)
					{
						mkdir(tmpfolderpath);
					}

					//if(access(tmppath,0)!=0)
					//{
					//	mkdir(tmppath);
					//}

					m_tranfilepath.Format("v:\\netspy\\tmp\\%s",m_tranfilename);

				}else{

					CString tmppath = g_pImsgMissObj->GetTmpPath();
				//	CString tmppath = "v:\\netspy\\tmp\\";
					m_tranfilepath.Format("%s%s",tmppath,m_tranfilename);
				}

				errno1=54;

				WriteLog(IMSGPRO, "QQFile name=%s",m_tranfilepath);

				//只保存txt,doc,xls

				BOOL bsaveflg=FALSE;
// 				if(m_tranfilename.GetLength()>4)
// 				{
					CString tmpfix=m_tranfilename.Right(4);
/*					if(tmpfix==".txt"||tmpfix==".doc"||tmpfix==".xls")*/
						bsaveflg=TRUE;
/*				}*/
		
				if(bsaveflg && tmp_packetnum<5000) //5M以内
				{
					errno1=55;
					if (!m_file.Open(m_tranfilepath, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
					{
						return -2;
					}
					m_file.SeekToEnd();

					errno1=56;
					if(m_filterbuf != NULL)
					{
						delete [] m_filterbuf;
					}
					m_filterbuf=new char[tmp_packetnum+10];
					m_packetnum=tmp_packetnum;
					memset(m_filterbuf,0,tmp_packetnum+10);
					m_bSaveFile=TRUE;
				}
				else //文件太大只写文件名
				{				
				
						if(!m_iswrite)
						{
							WriteLog(IMSGPRO, "QQFile %s 太大，只写日志",m_tranfilename);
							m_iswrite=TRUE;
							WriteQQFileIndexFile("");	 
						}	
				}
			}
		}		
	}
	else if (flg==6) //03 60 获取qq空间好友列表
	{
		WORD qqdatalen=htons(*(WORD*)(pPacket+0x36)); 
		DWORD localqqno=htonl(*(DWORD*)(pPacket+0x3a));
		WORD qqlistnum=htons(*(WORD*)(pPacket+0x3e));
		
		int tmpnum= (packh.m_datalen-0x40)/4;
		if(	qqlistnum>0
			&&qqdatalen==(packh.m_datalen-0x38)
			&& qqlistnum==tmpnum)
		{
			CString result;
		 
			result.Format("qqlist\t%u\t",localqqno);

			for (int i=0;i<qqlistnum;i++)
			{
				DWORD tmpqqno=htonl(*(DWORD*)(pPacket+0x40+i*4));
				if(tmpqqno!=localqqno)
				{
					CString tmpstr;
					tmpstr.Format("%u\t",tmpqqno);
					result+=tmpstr;
				}
			}
			WriteArrToData(IMSG_PROTOCOL , WEBID_DATA,result);	 
		}
	}
	else if( packh.m_type ==UDPPacket 
		&& m_bSaveFile 
		&& packh.m_datalen>0x12)
	{
		errno1=57;
		//
		if(memcmp(packh.m_sip, m_csip, 4) == 0  
			&& pPacket[0]=='\x05'	
			&& memcmp(pPacket+6,"\x0\x0",2)==0  
			   )
		{
			 //写文件 ,1000一块
			if(memcmp(pPacket+4,"\x2d\x63",2)==0)
			{

				errno1=8;
				DWORD packetoffset=htonl(*(DWORD*)(pPacket+0xa ));
// 				if(packetoffset<=m_packetnum && packetoffset < 5000)// 小于 5m
// 				{
errno1=81;
					if(m_filterbuf[packetoffset]==0) //未写过
					{
errno1=82;
							if(packh.m_datalen>0x1C &&  memcmp(pPacket+0x10,"\x0\x01\x1f\x8b\x08",5)==0 ) 	//gz压缩 长度1000
							{
							
errno1=83;
								BYTE pData[2000];				
								DWORD 	destLen = 2000;
								int	nRet = gzdecompress((unsigned char*)pPacket+0x12+10, packh.m_datalen-0x12-10,pData, &destLen);
errno1=84;							
								if (nRet == Z_OK && destLen==1000)
								{
									m_filterbuf[packetoffset]=1;
									m_recvpacketnum++;
errno1=85;
									if (m_file.m_hFile)
									{
										m_file.Seek(packetoffset*1000,CFile::begin );
										m_file.Write(pData,destLen);
									}
								}
							}
							else if((packh.m_datalen-0x12)==1000) //没有压缩
							{
errno1=86;
								m_recvpacketnum++;
								m_filterbuf[packetoffset]=1;
errno1=87;
								if (m_file.m_hFile)
								{
									m_file.Seek(packetoffset*1000,CFile::begin );
									m_file.Write(pPacket+0x12,packh.m_datalen-0x12);
								}
							}			
					}
/*				}*/
			}
			else if(memcmp(pPacket+4,"\x2b\x56",2)==0) //末包
			{
				errno1=9;
				DWORD packetoffset=htonl(*(DWORD*) (pPacket+14 ));
				DWORD packetlen=htonl(*(DWORD*) (pPacket+0xa ) ); 
errno1=91;
				if(packetoffset<=m_packetnum
					&&  (packh.m_datalen-0x12)==packetlen)
				{
					if(m_filterbuf[packetoffset]==0)
					{
						m_recvpacketnum++;
						m_filterbuf[packetoffset]=1;
errno1=92;
						m_file.Seek(packetoffset*1000,CFile::begin );
						m_file.Write(pPacket+0x12,packetlen);
errno1=93;						
						if(m_recvpacketnum==(m_packetnum+1))//还原完毕
						{
							//m_attfiles.Add(m_tranfilepath);
errno1=94;
							m_file.Close();
							m_file.m_hFile=NULL;
errno1=95;
							SYSTEMTIME st;
							GetLocalTime(&st);
							CString tm;
							tm.Format("%04u%02u%02u/",st.wYear,st.wMonth,st.wDay);
				
							WriteQQFileIndexFile(tm);
						}
					}
				}
			}		
		}
	}
//上一个包
// 	memset(m_bakbuf,0,2000);
// 	memcpy(m_bakbuf,pPacket,packh.m_datalen);
// 	m_bakbuflen=packh.m_datalen;
  	}
	catch(...)
	{
		WriteLog(IMSGPRO, "QQFile() Error!!=%d",errno1);
		FILE* fp;
		fp=fopen("e:\\packet.txt","wb");
		if(fp!=NULL)
		{
			fwrite(pPacket,packh.m_datalen,1,fp);
			fclose(fp);
		}
// 		fp=fopen("e:\\lastpacket.txt","wb");
// 		if(fp!=NULL)
// 		{
// 			fwrite(m_bakbuf,m_bakbuflen,1,fp);
// 			fclose(fp);
// 		}
	}
	return 0;
}
 
void QQTranFileSession::WriteQQFileIndexFile(CString filepath)
{
	CString str;
	// 写对照文件
	// 输出活动历史
		if (v_statImsgOther)
		{
// 			if(!m_from.IsEmpty() && !m_to.IsEmpty())
// 
// 			{
// 				HistoryRecord hisRec;
//  				hisRec.SetRecord(IDX_ID,		m_from);
// 				hisRec.SetRecord(IDX_CONTACT,		m_to);
// 
// 				hisRec.SetRecord(IDX_STATE, "通迅");
// 				WriteHisData("imsg_oicq", "imsg_oicq", &hisRec, g_pImsgMissObj,	IMSG_PROTOCOL);//, HISTORY_DATA);
// 			}
		}

	// 匹配案件、预警和屏蔽信息
	DWORD state = 0;

	CString text;
	text.Format("%s\r\n%s", m_from, m_to);

	SearchCaseOrWarn(this, text, text.GetLength(), state,"imsg_qq_tranfile");

	// 匹配案件和预警
// 	m_OpenCaseFlg = v_opencaseVp;
// 	m_OpenWarnFlg = v_openwarnVp;

	// 输出txt文件
// 	m_begfiletm = m_filetm;
// 	m_filetm = time(0);

	m_OpenIndxFlg = TRUE;
	
	// 写对照文件
	m_realfname.Format("v:\\netspy\\tmp\\%s",m_tranfilename);
	m_indexfname = m_tranfilename + ".indexfile.txt";

	str.Format("user=%s\r\nfrom=%s\r\ncontact=%s\r\ntranfilename=%s\r\ntranfilelen=%d\r\n",
		m_userid,m_from,m_to,m_tranfilename,m_tranfilelen);
	WriteIndexFile("imsg_qq_tranfile", "imsg_qq_tranfile", str, g_pImsgMissObj);

	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	m_begfiletm = 0;
	m_closetm += 10;
}