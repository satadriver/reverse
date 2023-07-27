
#ifndef AFX_FLASHVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#define AFX_FLASHVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_

/***************************  Flash视频处理  ******************************/

#if _MSC_VER > 1000
#pragma once
#endif

enum chunktype
{
	enumhttpchunk,
	enummmstcpchunk,
	enummmsudpchunk
};

struct videoinfo
{
public:
	ULONGLONG headsize;
	ULONGLONG tcpseq;//udpseq
	ULONG	  headoffset;
	//////////////////////////////////////////////////////////////////////////
	chunktype echunktype;
	ULONGLONG filesize;
	ULONGLONG DataPacketsCount;
	DWORD	MaximumDataPacketSize;
	DWORD	MinimumDataPacketSize;
	DWORD   currentdatachunkdequence;
	
};
// 下行使用
struct flashstatus
{
	UCHAR commandtype[2];//0 4 8
	UCHAR mediatype[2]	;//类型 08 09 ...
	UCHAR separator[2]	;//分隔符号0xC..
	UINT  medialength[2];//长度
	UCHAR residual[2]	;//余数
	
	UINT  timestram[2]	;
	BOOL  saveok[2]		;
};
struct amfobj{
	INT amfnum;//2--f
	INT amfcmd;
	USHORT chunksize; //
	USHORT bodysize;// 0 ,4 
	UINT timestramp;
	//USHORT remmainbytes;//0 4
	BOOL  bstart;
};

class MediaCommInfo
{
public:
	unsigned int	m_Ssequence	;
	unsigned int	m_Sack		;
	time_t			m_Stm			;	
	UINT			m_nseq		;	//指下一个包的seq
	UINT			m_fileseq		;
	BOOL			m_havehead	;
	
	UINT			m_timestramp;//=0;
	// 下行使用
	UINT			m_videotimestramp;//=0;
	UINT			m_deltavideotimes;
	
	// 下行使用
	videoinfo  videoinfo;
	
	//wq
	amfobj m_pamfobg[0x40];
	UCHAR  *m_videobuf ;
	UCHAR  *m_audiobuf ;
	UCHAR  *m_tvideobuf ;
	UCHAR  *m_taudiobuf ;
	UCHAR  *m_reserverbuf;

	int m_videolen;
	int m_audiolen;
	int m_tvideolen;
	int m_taudiolen;
	int m_Reservedlen;

	USHORT m_currentchunksize;
	
	MediaCommInfo()
	{
		// 下行使用
		memset(&this->videoinfo,0,sizeof(videoinfo));
		//up
		memset (m_pamfobg,0,sizeof(amfobj)*0x40);
		
		this->m_havehead=0;
		this->m_Ssequence=0;
		this->m_nseq=0;
		this->m_fileseq=0;
		this->m_Stm=0;
		m_timestramp=0;

		// 下行使用
		m_videotimestramp=0;
		m_deltavideotimes=0;

		m_videobuf=0 ;
		m_audiobuf =0;
		m_tvideobuf =0;
		m_taudiobuf =0;
		m_reserverbuf=0;		
		m_videolen=0;
		m_audiolen=0;
		m_tvideolen=0;
		m_taudiolen=0;
		m_Reservedlen=0;
		m_currentchunksize=0x80;
	}
	
	void Clear()
	{
		m_Ssequence=m_fileseq;
		m_nseq=m_fileseq;
		m_Sack = 0;
		m_havehead = 0;
		m_timestramp=0;
		
		// 下行使用
		m_videotimestramp=0;
		m_deltavideotimes=0;
		memset(&videoinfo, 0, sizeof(videoinfo));

	}
	
	~MediaCommInfo()
	{
		if(m_videobuf!=0)
			delete[]m_videobuf;
		if(m_audiobuf!=0)
			delete[]m_audiobuf;		 
		if(m_tvideobuf!=0)
			delete[]m_tvideobuf;		 
		if(m_taudiobuf!=0)
			delete[]m_taudiobuf;		 
		if(m_reserverbuf!=0)
			delete[]m_reserverbuf;		 

	}
};

class FlashSession : public MediaSession
{
public:
	FlashSession(DATEHEADER& packh, int aT);

	virtual ~FlashSession()
		{ }

	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

//	virtual BOOL CloseSession(time_t nowtm);

	virtual BOOL NeedAckSocket()
		{return FALSE;}

	virtual int ListToFile();

public:
	BOOL			m_bfromserver;
	TcpPacketList	m_Packetlst	;	// 包链表
	DWORD			m_CSeqOffset	;
	DWORD			m_CAckOffset	;
	time_t			m_filetm		;
	BOOL            m_havesep;
	// 下行使用
	UCHAR			m_AVseparator[2]	;//分隔符号0xC..
	MediaCommInfo	m_Responseline;
	//add by wq 20070514////////////////////////////////////////////////////////////////////////
// 	CHAR * remainbuf;
// 	BOOL  checkamfobj;
// 	int startbuflen;
// 	BOOL bloastpack;
	//
	UCHAR  *m_videobuf ;
	UCHAR  *m_audiobuf ;
	UCHAR  *m_tvideobuf ;
	UCHAR  *m_taudiobuf ;


	int m_videolen;
	int m_audiolen;
	int m_tvideolen;
	int m_taudiolen;
	
private:
	int lstToFlvFile(UCHAR * buf,UCHAR * savefilebuf,BOOL bclose=FALSE);
	int WriteFlashStream(FILE ** fs,UCHAR * buf,INT Length,UCHAR mediatype,  MediaCommInfo *pheader);
	int ParserFlashFormat(UCHAR * buf,int buflen );
	//BOOL ParserFlashCommand(UCHAR *p,flashstatus& flashdatastatus,UCHAR suffix);
	//下行		
	int lstToFlvFileOld(UCHAR * buf,UCHAR * savefilebuf, UINT Seq);
	BOOL ParserFlashCommand(UCHAR *p,flashstatus& flashdatastatus,UCHAR suffix);

	void SWAPinfo();
};

inline BOOL IsFlvVideo(UCHAR * p, USHORT len)
{
	if ((len == 16)  
		&& (p[0] == 2) 
		&& (memcmp(p + 4, "\x0\x0\x4\x5\x0\x0\x0\x0", 8) == 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

inline BOOL ParserFlashCommand1(UCHAR * p, USHORT len)
{
	if ((p[0] > 0) && (p[0] < 8) && (p[1] == 0))
	{
		if(*(USHORT*)(p + 4)+12 < len && *(USHORT*)(p+4) < 0x80)
		{
			if ((p[7] >= 2) && (p[7] <= 0x1F))
			{
				return TRUE;
			}
		}
	}
	else if((p[0]>>4 == 4) && (p[1] == 0))
	{
		if(p[7] == 0x08 || p[7] == 0x09)
		{
			return TRUE;
		}
	}
	return FALSE;
}

#endif
