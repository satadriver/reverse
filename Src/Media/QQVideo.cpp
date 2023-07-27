#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "QQVideo.h"


BOOL analyseid2(UCHAR *buf,int len ,CString &s_srcqqid ,CString &s_dstqqid);

BOOL analyseid(UCHAR *buf,int len ,CString &s_srcqqid ,CString &s_dstqqid );//有id时显示出id号

int ParserQQSipPacket(BYTE *psip,int len,CString & qqsip,CString &qqdip)
{
	//From: <sip:469992@232.43.7.0:5060>;tag=42535;tencent=tencentsipstack;index=0;ver=1
	//Contact: <sip:452684788@244.107.251.26:5060>
	//a=rtpmap:100 TCODECAG/8000
	CString strSIPP((CHAR *)psip,len);
	strSIPP.MakeLower();
	//o=QQUser 4321 1234 244.107.251.26 IP4 244.107.251.26
	if(strSIPP.Find("o=qquser") > 0)
	{
		int i = 0, j = 0;
		i = strSIPP.Find("a=rtpmap:", i);
		if(i)
		{
			j = strSIPP.Find(" ", i);	
			if(j > 0)
			{
				CString tmpcode((LPCSTR)strSIPP+i+sizeof("a=rtpmap:")-1,j-sizeof("a=rtpmap:")-i+1);
				int avcode		=0;
				avcode=atoi((LPCSTR)tmpcode);
				if (avcode==100 || avcode==99 || avcode==98)
				{
					
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}

		i=0;
		j=0;
		i=strSIPP.Find("from: <sip:",i);
		if(i)
		{
			j=strSIPP.Find("@",i);
			if(j>0)
			{
				CString tmpqqsip((LPCSTR)strSIPP+i+sizeof("from: <sip:")-1,j-sizeof("from: <sip:")-i+1);
				qqsip=tmpqqsip;
			}
		}

		i=strSIPP.Find("contact: <sip:",i);
		if(i)
		{
			j=strSIPP.Find("@",i);
			if(j>0)
			{
				CString tmpqqsip((LPCSTR)strSIPP+i+sizeof("contact: <sip:")-1,j-sizeof("contact: <sip:")-i+1);
				qqdip=tmpqqsip;
			}
		}
		return 3;
	}
	else
	{
		return 0;
	}
	return 0;
}

int isQQVideo(DATEHEADER& dateHeader, BYTE* tp)
{
	//TCP	//音视频  //sip
	if(dateHeader.m_type == TCPPacket)
	{
		if(dateHeader.m_datalen>0x200 && strnicmp((CHAR*)(tp+0xb),"sip/2.0 200 ok",14)==0)//sip
		{
			CString sipqq,dipqq;
			return ParserQQSipPacket(tp+0xb,dateHeader.m_datalen-0xb,sipqq,dipqq);			
		}
		else if(dateHeader.m_datalen>0x200 && strnicmp((CHAR*)(tp+31+0xb),"sip/2.0 200 ok",14)==0)
		{
			if( tp[0]==4 && tp[3]==0 && tp[5]==0)
			{
				CString sipqq,dipqq;
				return ParserQQSipPacket(tp+31+0xb,dateHeader.m_datalen-31-0xb-1,sipqq,dipqq);
			}
			
		}
		else if((tp[2]==0 && tp[3]==0 && tp[4]==0) && tp[5]==05 && 
			((tp[0x8] ==1 && (tp[0xc]==0x67 || tp[0xc]==0x63 )) ||
			(tp[0x8] ==3 ) ) && tp[0xb]==0x80 )
		{
			return 4;
		}
		else if(dateHeader.m_datalen>50 &&  tp[0]==4 && tp[dateHeader.m_datalen-1]==3)
		{
			BYTE * tmpPacket=tp+31;
			//qq2011bate2 tcp 中转包
			if(memcmp(tp+0x10,"\x00\x00\x00\x00",4)==0 && memcmp(tmpPacket+2,"\x00\x00\x00",3)==0
				&& (memcmp(tmpPacket+8,"\x03\x03\x80",3)==0 || memcmp(tmpPacket+8,"\x01\x01\x80",3)==0))
			{
				 return 4;
			}

			if(memcmp(tp+0x10,"\x00\x00\x00\x00",4)==0 && memcmp(tmpPacket+2,"\x00\x00\x00",3)==0
				&& (memcmp(tmpPacket+13,"\x04\x00\x04",3)==0 || memcmp(tmpPacket+20,"\x05\x00\x04",3)==0))
			{
				return 4;
			}




			if((tmpPacket[2]==0 && tmpPacket[3]==0 && tmpPacket[4]==0) && tmpPacket[5]==05 && 
				((tmpPacket[0x8] ==1 && (tmpPacket[0xc]==0x67 || tmpPacket[0xc]==0x63 )) ||
				(tmpPacket[0x8] ==3 ) ) && tmpPacket[0xb]==0x80 )
			{
				return 4;
			}
		}
		else
		{
	
			DWORD tlen = *((DWORD*)tp);
			if(dateHeader.m_datalen>0x30 && tlen == dateHeader.m_datalen )	// +1 屁股后面有个 03
			{
				if(
					(
					memcmp(tp+8,"\x01\x01",2)==0 
					||memcmp(tp+8,"\x03\x03",2)==0 
					)
					&&
					(memcmp(tp+10,"\x80\x67",2)==0 
					|| memcmp(tp+10,"\x80\xe7",2)==0 
					||memcmp(tp+10,"\x80\x75",2)==0 
					|| memcmp(tp+10,"\x80\x6c",2)==0
					||memcmp(tp+10,"\x80\xec",2)==0
					)
					)
				{
					return 13;
				}
			}

		}
		if(DataHeader.m_datalen >= 32)
		{
			if(DataHeader.m_datalen > 50)
			{
				if(memcmp(tp+0x11, "\x04\x00\x04", 3) == 0 && memcmp(tp+0x11+7, "\x05\x00\x04", 3) == 0)
				{
					return 15;
				}
			}
			// tcp qq2009sp2
			if(memcmp(tp+13, "\x04\x00\x04", 3) == 0 && memcmp(tp+13+7, "\x05\x00\x04", 3) == 0)
			{
				return 15;
			}
		}
	}
	//UDP   //音视频  //sip
	else if(dateHeader.m_type == UDPPacket)
	{
		if(dateHeader.m_datalen>0x200 && strnicmp((CHAR*)(tp+0x12),"sip/2.0 200 ok",14)==0)//sip
		{
			CString sipqq,dipqq;
			return ParserQQSipPacket(tp+0x12,dateHeader.m_datalen-0x12,sipqq,dipqq);
		}
		else if(dateHeader.m_datalen>0x12 && (tp[0]==0x3 ) && tp[0xe]==05 && tp[0xd]==0 && tp[0x12]==0x80  )
		{
			if((tp[0x11] ==1 && (tp[0x13]==0x67 || tp[0x13]==0x63 )))
			{
				return 2;
			}
			else if(tp[0x11] ==3 )
			{
				return 1;	
			}
			
		}
		else if(dateHeader.m_datalen>0x200 && strnicmp((CHAR*)(tp+0x12+31),"sip/2.0 200 ok",14)==0)
		{
			CString sipqq,dipqq;
			return ParserQQSipPacket(tp+0x12+31,dateHeader.m_datalen-0x12-31-1,sipqq,dipqq);
		}
		else if(dateHeader.m_datalen >50 && tp[0]==4 && tp[dateHeader.m_datalen-1]==3)
		{
			BYTE * tmpPacket=tp+31;

			if(tmpPacket[0]==0x5 &&  (tmpPacket[0xf]==0x03 ||  tmpPacket[0xf]==0x01) && 
				(memcmp(tmpPacket+0x10,"\x80\x67",2)==0 ||  memcmp(tmpPacket+0x10,"\x80\xe7",2)==0 
				|| memcmp(tmpPacket+0x10,"\x80\x6c",2)==0 || memcmp(tmpPacket+0x10,"\x80\xec",2)==0)
				|| memcmp(tmpPacket+0x10, "\x80\x6b", 2) == 0)
			{
				return 15;
			}



			if( (tmpPacket[0]==0x3 ) && tmpPacket[0xe]==05 && tmpPacket[0xd]==0 && tmpPacket[0x12]==0x80  )
			{
				if((tmpPacket[0x11] ==1 && (tmpPacket[0x13]==0x67 || tmpPacket[0x13]==0x63 )))
				{
					return 12;
				}
				else if(tmpPacket[0x11] == 3)
				{
					return 11;	
				}
			}

		

			// 80 67 qq2008beta1后, 04
			DWORD tlen = tp[0x2b]*256+tp[0x2c];
			if(tlen + 0x2d +1 == dateHeader.m_datalen)	// +1 屁股后面有个 03
			{
				if(memcmp(tp+0x2d,"\x80\x67",2)==0 || memcmp(tp+0x2d,"\x80\xe7",2)==0)
				{
					return 13;
				}
			}
		}
		else if(memcmp(tp,"\x05\x05\x03\x00",4)==0 && dateHeader.m_datalen > 50) //2008正式视频
		{
			if(memcmp(tp+15, "\x80\xe7", 2) == 0 ||memcmp(tp+15, "\x80\x67", 2) == 0)
			{
				DWORD tlen = tp[13]*256+tp[14];
				if(tlen + 15 == dateHeader.m_datalen)
				{
					return 14;
				}
			}
		}
		else if(memcmp(tp,"\x05\x05\x01\x00",4)==0 && dateHeader.m_datalen > 50)// 2008正式音频
		{
			if(memcmp(tp+15, "\x80\x67", 2) == 0 || memcmp(tp+15, "\x80\x75", 2) == 0 || memcmp(tp+15, "\x80\xe7", 2) == 0)
			{
				return 2;
			}
		}
		else if(tp[0]==5 && (tp[5]==3 || tp[5]==1))// 2009sp2
		{
			if(memcmp(tp+6, "\x80\x67", 2) == 0 || memcmp(tp+6, "\x80\x75", 2) == 0 || memcmp(tp+6, "\x80\xe7", 2) == 0)
			{
				return 16;
			}
		}
		else if(tp[0]==5 && (tp[0xf]==3 || tp[0xf]==1)) //2010sp3 & 2011 bate
		{
			if(memcmp(tp+0x10, "\x80\x67", 2) == 0 
				|| memcmp(tp+0x10, "\x80\x75", 2) == 0 
				|| memcmp(tp+0x10, "\x80\xe7", 2) == 0
				|| memcmp(tp+0x10, "\x80\x6c", 2) == 0
				|| memcmp(tp+0x10, "\x80\xec", 2) == 0
				||memcmp(tp+0x10, "\x80\x6b", 2) == 0
				)
			{
				return 15;
			}
		}


		if(DataHeader.m_datalen >= 32) //2010sp3  定位qq  号的包.
		{
			// 04 00 04 源QQ 05 00 04 目的QQ
			if(DataHeader.m_datalen  > 50)
			{
				if(memcmp(tp+0x11, "\x04\x00\x04", 3) == 0 && memcmp(tp+0x11+7, "\x05\x00\x04", 3) == 0)
				{
					return 15;
				}

				if(memcmp(tp+0x2f, "\x04\x00\x04", 3) == 0 && memcmp(tp+0x2f+7, "\x05\x00\x04", 3) == 0)
				{
					return 15;
				}
				if(memcmp(tp+20, "\x04\x00\x04", 3) == 0 && memcmp(tp+20+7, "\x05\x00\x04", 3) == 0)
				{
					return 15;
				}
				//qq2011bate2
				if(tp[0]==0x4 && tp[DataHeader.m_datalen-1]==0x03)
				{
					if(tp[0x1f]==0x03)
					{
						if(memcmp(tp+0x1f+0x14,"\x04\x00\x04",3)==0 
							&& memcmp(tp+0x1f+0x14+7,"\x05\x00\x04",3)==0 )
						{
							return 15;
						}
					}
				}

			}
			else
			{
				// udp qq2009sp2
				if(memcmp(tp+20, "\x04\x00\x04", 3) == 0 && memcmp(tp+20+7, "\x05\x00\x04", 3) == 0)
				{
					return 15;
				}
				else if(memcmp(tp+13, "\x04\x00\x04", 3) == 0 && memcmp(tp+13+7, "\x05\x00\x04", 3) == 0)
				{
					return 15;
				}
			}
		}

	}
	return 0;
}


QQSession::QQSession(DATEHEADER& packh, int aT)
: MediaSession(packh, aT)
{
	m_MediaPro = "media_QQ";
	m_MediaDirection = "<<==>>";
	m_AvType = 3;
	m_b_haveID=FALSE;
	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype = packh.m_type;
	//TCP	//音视频  //sip
	if(packh.m_type == TCPPacket)
	{
		if(packh.m_datalen>0x200 && strnicmp((CHAR*)(g_Packet+0xb),"sip/2.0 200 ok",14)==0)//sip
		{
			CString sipqq,dipqq;
			int vtype =ParserQQSipPacket((BYTE*)g_Packet+0xb,packh.m_datalen-0xb,sipqq,dipqq);			
			if(vtype==3)
			{
				memcpy(&this->m_sipheader, &packh.m_type2, sizeof(m_sipheader));
				memcpy(this->m_sipPacket, g_Packet,packh.m_datalen);
				this->m_sipPacket[packh.m_datalen]=0;
				m_userid = sipqq;
				m_LinkMan = dipqq;
			}
			else
			{
				memset(&this->m_sipheader, 0, sizeof(m_sipheader));
			}
		}
		else if(packh.m_datalen>0x200 && strnicmp((CHAR*)(g_Packet+31+0xb),"sip/2.0 200 ok",14)==0)
		{
			CString sipqq,dipqq;
			int vtype = ParserQQSipPacket((BYTE*)g_Packet+31+0xb,packh.m_datalen-31-0xb-1,sipqq,dipqq);
			if(vtype==3)
			{
				memcpy(&this->m_sipheader, &packh.m_type2, sizeof(m_sipheader));
				memcpy(this->m_sipPacket, g_Packet,packh.m_datalen);
				this->m_sipPacket[packh.m_datalen]=0;
				m_userid = sipqq;
				m_LinkMan = dipqq;
			}
			else
			{
				memset(&this->m_sipheader,0,sizeof(m_sipheader));
			}
		}
		else
		{
			memset(&this->m_sipheader,0,sizeof(m_sipheader));
		}
	}
	//UDP   //音视频  //sip
	else if(packh.m_type == UDPPacket)
	{
		if(packh.m_datalen>0x200 && strnicmp((CHAR*)(g_Packet+0x12),"sip/2.0 200 ok",14)==0)//sip
		{
			CString sipqq,dipqq;
			int vtype = ParserQQSipPacket((BYTE*)g_Packet+0x12,packh.m_datalen-0x12,sipqq,dipqq);
			if(vtype==3)
			{
				memcpy(&this->m_sipheader,&packh.m_type2,sizeof(m_sipheader));
				memcpy(this->m_sipPacket, g_Packet,packh.m_datalen);
				this->m_sipPacket[packh.m_datalen]=0;
			}
			else
			{
				memset(&this->m_sipheader,0,sizeof(m_sipheader));
			}
		}	
		else if(packh.m_datalen >50 && g_Packet[0]==4 && g_Packet[packh.m_datalen-1]==3 && g_Packet[3]==0 && g_Packet[5]==0)
		{
			BYTE * tmpPacket=(BYTE*)(g_Packet+31);
			CString sipqq,dipqq;
			int vtype = ParserQQSipPacket((BYTE*)tmpPacket+0x12,packh.m_datalen-0x12-31-1,sipqq,dipqq);
			if(vtype==3)
			{
				memcpy(&this->m_sipheader,&packh.m_type2,sizeof(m_sipheader));
				memcpy(this->m_sipPacket, g_Packet,packh.m_datalen);
				this->m_sipPacket[packh.m_datalen]=0;
			}
			else
			{
				memset(&this->m_sipheader,0,sizeof(m_sipheader));
			}			
		}
		else
		{
			memset(&this->m_sipheader,0,sizeof(m_sipheader));
		}
	}
#ifdef _DEBUG
	m_cntPacket = 0;
#endif
}

int QQSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	MEDIAPACKET qqpack(packh, (CHAR *)pPacket);
	if(packh.m_type == UDPPacket)
	{
		int ret = isQQVideo(packh, (BYTE *)pPacket);
		if ((ret == 1) || (ret == 2))
		{
			qqpack.m_serial = Swap2Byte(*(WORD*)(pPacket+0x0f));
			qqpack.m_vtype = ret;
		}
		else if( ret==11  || ret==12)
		{
			qqpack.m_serial = Swap2Byte(*(WORD*)(pPacket+46));
			qqpack.m_vtype = ret;
		}
		else if(ret==13)
		{
			qqpack.m_serial = Swap2Byte(*(WORD*)(pPacket+47));
			qqpack.m_vtype = 12;
		}
		else if(ret==14)
		{
			qqpack.m_serial = Swap2Byte(*(WORD*)(pPacket+17));
			qqpack.m_vtype = 12;
		}
		else if(ret==15)
		{
			qqpack.m_serial = 0;
			qqpack.m_vtype = 12;
		}
		else if(ret==16)
		{
			qqpack.m_serial = 0;
			qqpack.m_vtype = 12;
		}
	
		else
		{
			return 0;
		}
	}
	if (qqpack.m_buf==NULL)
	{
		return 0;
	}
#ifdef _DEBUG
	m_cntPacket ++;
#endif

	if(packh.m_type == UDPPacket) //udp
	{

		if(!m_b_haveID)
		{
			if(analyseid((UCHAR*)pPacket, packh.m_datalen, m_userid, m_LinkMan))
			{
				m_b_haveID=TRUE;
			}
			else if(analyseid2((UCHAR*)pPacket, packh.m_datalen, m_userid, m_LinkMan))
			{
				m_b_haveID=TRUE;
			}
		}


		if(m_userid.IsEmpty())
		{
			if(pPacket[0]==0x04 && pPacket[packh.m_datalen-1]==0x03)
			{
				 
				if(memcmp(pPacket+0x10,"\x00\x00\x00\x00",4)==0)
				{
					BYTE * tmpPacket=(BYTE*)pPacket+0x1f;
					
					if(tmpPacket[0]==0x5 &&  (tmpPacket[0xf]==0x03 ||  tmpPacket[0xf]==0x01) && 
						(memcmp(tmpPacket+0x10,"\x80\x67",2)==0 ||  memcmp(tmpPacket+0x10,"\x80\xe7",2)==0 
						|| memcmp(tmpPacket+0x10,"\x80\x6c",2)==0 || memcmp(tmpPacket+0x10,"\x80\xec",2)==0) )
					{
						DWORD userid=ntohl(*(DWORD*)(pPacket+9));
						m_userid.Format("%d",userid);
					}
				}
			}
			
		}
	}
	else
	{
		if(DataHeader.m_datalen >= 0x20)
		{
			int offset=0;
			if(pPacket[0]==0x04 && pPacket[packh.m_datalen-1]==0x03)
			{
				if(memcmp(pPacket+0x10,"\x00\x00\x00\x00",4)==0)
				{
					offset=0x1f;
					BYTE * tmpPacket=(BYTE*)pPacket+offset;
					if(m_userid.IsEmpty())
					{
						if(memcmp(tmpPacket+2,"\x00\x00\x00",3)==0)
						{
							DWORD userid=ntohl(*(DWORD*)(pPacket+9));
							m_userid.Format("%d",userid);
						}
					}
				}
			}


			// 04 00 04 源QQ 05 00 04 目的QQ
			if(memcmp(pPacket+offset+13, "\x04\x00\x04", 3) == 0 && memcmp(pPacket+offset+13+7, "\x05\x00\x04", 3) == 0)
			{
				if(!m_b_haveID)
				{

					CString qqno1,qqno2;
					if(analyseid((UCHAR*)pPacket+offset, packh.m_datalen-offset, qqno1, qqno2))
					{
						if(m_userid.IsEmpty())
						{
							m_userid=qqno1;
							m_LinkMan=qqno2;
						}
						else 
						{
							if(m_userid==qqno1)
							{
								m_LinkMan=qqno2;
							}
							else //交换 
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
									
								
							 


								m_LinkMan=qqno1;
							}

						}
						m_b_haveID=TRUE;
					}
				}
			}
		}

	 
	}

	if (memcmp(packh.m_sip, m_cdip, 4) == 0)
	{
		qqpack.m_vtype |= 0x40;
	}
	m_packetlist.push_back(qqpack);
	qqpack.m_buf = NULL;
	m_lastpacktm = packh.m_tm;
	if (abs(packh.m_tm - m_processtm) > 60)
	{
		// 写文件
		m_processtm = packh.m_tm-20;
		ListToFile();
	}
	return 0;
}

int QQSession::ListToFile()
{
	m_OpenWarnFlg = v_openwarnQQMedia;
	m_OpenCaseFlg = v_opencaseQQMedia;
	m_OpenIndxFlg = v_openflagQQMedia;

	m_packetlist.sort();
	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("qqv1", m_sip, m_dip);
		if (m_realfname.IsEmpty())
		{
			return -2;
		}
		if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return -2;
		}
		file.SeekToEnd();

	 

		//写sip包
		if(m_sipheader.m_tm != 0)
		{
			WriteMediaFile_old(file,m_sipheader,(unsigned char*)m_sipPacket);
			//file.Write(&m_sipheader, sizeof(m_sipheader));
			//file.Write(m_sipPacket, m_sipheader.m_datalen);
		}
	}
	else
	{
		if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return -2;
		}
	}
	file.SeekToEnd();

	try
	{
#ifdef _DEBUG
		printf("pass out MediaFile %s 当前队列 %d packet, 插入 %d Packet.\n", 
			m_realfname, m_packetlist.size(), m_cntPacket);
#endif
		OLDDATEHEADER h1, h2;
		memset(&h1, 0, sizeof(h1));
		memset(&h2, 0, sizeof(h2));
		h1.m_type = h2.m_type = this->m_Packettype;
		memcpy(h1.m_sip, this->m_csip, 4);
		memcpy(h1.m_dip, this->m_cdip, 4);
		memcpy(h2.m_dip, this->m_csip, 4);
		memcpy(h2.m_sip, this->m_cdip, 4);
		h1.m_sport = h2.m_dport = atoi(this->m_sip.Right(5));
		h1.m_dport = h2.m_sport = atoi(this->m_dip.Right(5));
		while(m_packetlist.size()>10)
		{
			begin = m_packetlist.begin();
			end = m_packetlist.end();
			item = begin;
			item++;
#ifdef		_DEBUG
			MEDIAPACKET	*p1, *p2;
			p1 = &(*item);
			p2 = &(*begin);
#endif
			if(m_Packettype == UDPPacket)
			{
				for (int i = 1; i < min(m_packetlist.size(), 40); i++)
				{
					if(item == end)
					{
						break;
					}
					if (begin->m_vtype == item->m_vtype)
					{
						if(begin->m_serial ==0 && item->m_serial==0)
						{
							break;
						}
						else
						{
							if ((begin->m_serial-item->m_serial)==0 && abs(begin->m_tm-item->m_tm)<=3)
							{
								MediaPacketListIt itemtmp;
								itemtmp=item;
								item++;							
								m_packetlist.erase(itemtmp);
								continue;
							}
							else if((WORD)(begin->m_serial-item->m_serial)<0x8000 && abs(begin->m_tm-item->m_tm)<=3)
							{
								//QQPacketListIt itemtmp;
								begin=item;
								item++;
								continue;
							}
						}
					}
					item++;
				}
				if (begin->m_tm > m_processtm)
				{
					break;
				}
			}
			m_endfiletm = begin->m_tm;
			WriteMediaFile(file, (begin->m_vtype & 0x40)? h2: h1, *begin);
			m_packetlist.erase(begin);
		}
		file.Close();
	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "LstToQQFile() Error!!");
		return -3;
	}
	return 0;
}


BOOL analyseid2(UCHAR *buf,int len ,CString &s_srcqqid ,CString &s_dstqqid)
{
	if(buf[0]==0x4 && buf[len-1]==0x03 )
	{
		if(len>0x30 && buf[0x1f]==0x03)
		{
			DWORD qqno1=ntohl(*(DWORD*)(buf+9));
			DWORD qqno11=ntohl(*(DWORD*)(buf+0x23));
			DWORD qqno2=ntohl(*(DWORD*)(buf+0x27));
			if(qqno1==qqno11)
			{
				DWORD srcqqid,dstqqid;
				srcqqid=qqno1;
				dstqqid=qqno2;

				s_srcqqid.Format("%d",srcqqid);
				s_dstqqid.Format("%d",dstqqid);

				return TRUE;	
				
			}
			else if(qqno1==qqno2)
			{
				s_srcqqid.Format("%d",qqno2);
				s_dstqqid.Format("%d",qqno1);
				return TRUE;
			}
		}
	}	
	return FALSE;
	
}




BOOL analyseid(UCHAR *buf,int len ,CString &s_srcqqid ,CString &s_dstqqid )//有id时显示出id号
{
	if(buf[0]==0x3 && buf[0xe]==0x5 && buf[0x12]==0x80)
	{
		BYTE checkbyte;
		checkbyte=buf[3];
		DWORD srcqqid,dstqqid;
		DWORD tmpsrcqqid,tmpdstqqid;
		DWORD tmpcheck;
		tmpsrcqqid=*(DWORD*)(buf+4);
		tmpdstqqid=*(DWORD*)(buf+8);
		
		memset(&tmpcheck,checkbyte,4);
		srcqqid = Swap4Byte((DWORD)~(tmpsrcqqid^tmpcheck));
		dstqqid = Swap4Byte((DWORD)~(tmpdstqqid^tmpcheck));

		s_srcqqid.Format("%d",srcqqid);
		s_dstqqid.Format("%d",dstqqid);
		return TRUE;
	}
	else
	{
		int offset = 13;
		if(buf[0]==4)
		{
			offset = 0x2f;
		}
		else if(buf[0]==5)
		{
			offset = 0x11;
		}
		else if(buf[0]==0x3)
		{
			offset = 0x14;
		}

		if(offset > 0)
		{
			if( memcmp(buf+offset, "\x04\x00\x04", 3) == 0
			 && memcmp(buf+offset+7, "\x05\x00\x04", 3) == 0)
			{
				DWORD srcqqid,dstqqid;
				srcqqid=Swap4Byte(*(DWORD*)(buf+offset+3));
				dstqqid=Swap4Byte(*(DWORD*)(buf+offset+10));

				s_srcqqid.Format("%d",srcqqid);
				s_dstqqid.Format("%d",dstqqid);
				return TRUE;
			}
		}
	}


	return FALSE;
}
