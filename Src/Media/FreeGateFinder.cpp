
#include "StdAfx.h"
#include "FreeGateFinder.h"

#include "MediaSession.h"
#include "Media.h"

#include "../http/FreeGate757.h"
FreeGateFinder::FreeGateFinder(DATEHEADER& packh, int aT)  : SocketSession(packh, aT)
{

}

FreeGateFinder::~FreeGateFinder()
{

}

int FreeGateFinder::IsFreeGateFinder(DATEHEADER& packh, const char* pPacket)
{
	static FreeGate757 *fg757 = FreeGate757::GetInstance();
	static unsigned short lastpacketlen = 0;
	static unsigned short samepacketcount = 0;
	static unsigned short samecount = 0;
	static CString lastdip;
	CString sip;
	sip.Format("%03d.%03d.%03d.%03d",packh.m_sip[0],packh.m_sip[1],packh.m_sip[2],packh.m_sip[3]);
	CString dip;
	dip.Format("%03d.%03d.%03d.%03d",packh.m_dip[0],packh.m_dip[1],packh.m_dip[2],packh.m_dip[3]);
//	if (fg757->FindIp(sip))
	{
		if (samepacketcount == 0)
		{
			lastpacketlen = packh.m_datalen;
		}
		if ( (packh.m_datalen == lastpacketlen ) && (lastdip != dip))
		{
			samepacketcount++;
			lastpacketlen = packh.m_datalen;
			lastdip = dip;
		}
		else
		{
			samepacketcount = 0;
		}

		if (samepacketcount == 3)
		{
			samepacketcount = 0;
			samecount++;
		}
		if (samecount == 5)
		{
			samepacketcount = 0;
			samecount = 0;
			lastpacketlen = 0;
			lastdip = "";
			return TRUE;
		}
		
	}
	return FALSE;
}

int FreeGateFinder::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	WriteFGIndexFile();
	return TRUE;
}


void FreeGateFinder::WriteFGIndexFile()
{
	m_indexfname = CreateMediaFilename("freegateurl.indexfile.txt",m_sip, m_dip );	
	CString txt;
	txt.Format("state=7.57\r\n");
	WriteIndexFile("freegateurl", "freegateurl", txt, g_MediaMissObj);
	
}