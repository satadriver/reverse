// TeSpread.cpp: implementation of the CTeSpread class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)

#include "stdafx.h"
#include "../PubVar.h"
#include "http.h"
#include "TeSpread.h"
#include "httpsession.h"
#include "PublicFun.h"
#include <map>


/*!\fn �����ϣ��ֵ��
*/
typedef struct _FG_ANALY
{
	CString curdip;		///< ��ǰ���ӵ�Ŀ��IP
	int countdip;			///< Ŀ��IP��ͬ�Ĵ���
	int countsum;			///< session�ڲ�ͬ�Ự���ظ����ܴ���
	time_t tmout;		///< �ϴ������ʱ��
	time_t tmmatch;		///< �ϴ���Ч������ʱ��
	int clientoccur;    ///< �ͻ��˳���  1 Ϊ����
	int serveroccur;    ///<����˳���   1 Ϊ����
}strct_fganaly;

///< ��ssl sip_SessionIdΪKey,���ظ���Ϊֵ
static std::map<CString, strct_fganaly>	s_mapFgSession;

/*!\fn isFreegate686p
 * \brief ����Ƿ����686p��Ϊ����
 * ����:
	1, ssl.handshake.type==1
	2, ssl.sessionid�ڳ�������������socket�й̶�
	3, �жϴﵽһ������(10��), ��ȷ��Ϊfg686p
*/
BOOL isFreegate686p( const unsigned char* pPacket, DATEHEADER& packh ,HttpProtocol* httpprotocl)
{
	if(packh.m_dport != 443 || packh.m_datalen != 102)
		return FALSE;

	if(memcmp(pPacket, "\x16\x03\x00\x00\x61\x01\x00\x00\x5d\x03\x00", 11) != 0)
		return FALSE;		// ssl client hello and len=0x00005d

	CString sdip;
	sdip.Format("%03u.%03u.%03u.%03u",
			packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3]);

	char fgsession[100];
	memset(fgsession, 0, 100);
	int ioff = sprintf(fgsession, "%03u.%03u.%03u.%03u",
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3] );
	for (int iis = 0; iis < 32 ; iis++)
		sprintf(fgsession+ioff+iis*2, "%02x", pPacket[0x2c+iis]);

	std::map<CString, strct_fganaly>::iterator It;
	It = s_mapFgSession.find(fgsession);
	if(It == s_mapFgSession.end())
	{
		strct_fganaly oneFg;
		oneFg.tmout = 0;
		oneFg.tmmatch = time(0);
		oneFg.countdip = 1;
		oneFg.countsum = 1;
		oneFg.curdip = sdip;
		s_mapFgSession[fgsession] = oneFg;
	}
	else
	{
		It->second.tmmatch = time(0);
		It->second.countsum ++;
		if(It->second.curdip.Compare(sdip) != 0)
		{
			It->second.curdip = sdip;
			It->second.countdip ++;
		}

		if(It->second.countdip > 2 && It->second.countsum > 10)
		{
			// ͬID ����2Ŀ���һỰ������5
			if(abs(time(0)-It->second.tmout) > 5*60)
			{
				// 5�����������Ự����һ��
				HttpSession tmpsession(packh, g_httpFrom);
				tmpsession.m_OpenCaseFlg = v_opencasefgurl;
				tmpsession.m_OpenWarnFlg = v_openwarnfgurl;
				tmpsession.m_OpenIndxFlg = v_openflagfgurl;
				tmpsession.m_spyaddr = g_DDEPOS;
				DWORD state = 0;
				SearchCaseOrWarn(&tmpsession, NULL, 0, state);

				// д�����ļ�
				tmpsession.m_Requestline.m_Browser = TE_FREEGATE686p;
				//tmpsession.m_Requestline.m_BrowserVer = 0x686;
				tmpsession.WriteTeUrlIndexFile();

				It->second.tmout = time(0);
				It->second.countsum = 1;

				return TRUE;
			}
		}
	}

	/// �������1Сʱ�Բ����������ĻỰ
	CStringArray arrErase;
	time_t tmnow = time(0);
	for (It = s_mapFgSession.begin(); It != s_mapFgSession.end(); It++)
	{
		if(abs(tmnow-It->second.tmmatch) > 24*60)
		{
			// 1Сʱ�����
			arrErase.Add(It->first);
		}
		else if(abs(tmnow-It->second.tmmatch) > 2*60 && It->second.countsum == 1)
		{
			// 2����δ�ظ�
			arrErase.Add(It->first);
		}
	}
	for (int iie = 0; iie < arrErase.GetSize() ; iie++)
	{
		s_mapFgSession.erase(arrErase[iie]);
	}

	return false;
}

///< ��dns sip_fakeurlΪKey,���ظ������Ϊֵ
static std::map<CString, strct_fganaly>	s_mapFgDnsSession;


typedef struct dns_header //DNS���ݱ���

{
unsigned short id; //��ʶ��ͨ�����ͻ��˿��Խ�DNS��������Ӧ����ƥ�䣻

unsigned short flags; //��־��[ QR | opcode | AA| TC| RD| RA | zero | rcode ]

unsigned short quests; //������Ŀ��

unsigned short answers; //��Դ��¼��Ŀ��

unsigned short author; //��Ȩ��Դ��¼��Ŀ��

unsigned short addition; //������Դ��¼��Ŀ��

}DNS_HEADER;


/*!\fn isFreegate689b3
 * \brief ����Ƿ�Ϊ������689��ʽ����԰�3 ����������������
 * ��������.
	1, ����dns�淶(port 53 standard query)
	2, ����512
	3, ��������60, ���� \x32 21fea5d295efdc5bb1fa8e356342067865253d0a8da5eb913a12 \x32 069608c18eecd432c73515fd4e7f35d21955c06b129bb35332 \x2 14 \x5 vcxde\x3 com
		����\x32, ��һ�� α���� ��(����1λ��2λ+������ĸ+com)���, ����Ϊ 2����+5��ĸ+.com
	4, һ������������һ��ᷢ��6�����ҵ�����, α��������. ǰ2 \x32 ����(���)
	5, ��ͬ������������α�����仯. 
*/
BOOL isFreegate689b3( const unsigned char* pPacket, DATEHEADER& packh ,HttpProtocol* httpprotocl)
{
	if(packh.m_dport != 53 || packh.m_datalen != 512)
		return FALSE;


	DNS_HEADER * dnshead=(DNS_HEADER*)pPacket;
	if(dnshead->flags!=0x001 || dnshead->quests!=0x100) //not standard query
	{
		return FALSE;
	}

 
	char fgsession[100];
	memset(fgsession, 0, 100);
	int ioff = sprintf(fgsession, "%03u.%03u.%03u.%03u",
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3] );


	int csstype=0;

	UCHAR *pcurdata=(UCHAR*)pPacket+sizeof(DNS_HEADER); // dfgvx.com
//	if(strstr((char*)pcurdata,"\x05\x64\x66\x67\x76\x78\x03\x63\x6f\x6d")) //free 1.3 ��ʽ��
	if(strstr((char*)pcurdata,"\x03\x63\x6f\x6d")) //free 1.3 ��ʽ�� .com 
	{
		sprintf(fgsession+ioff, "%s", "freegate");
		csstype=1;
	}
	else if(strstr((char*)pcurdata,"\x04\x69\x6e\x66\x6f")) //�޽� .info
	{
		sprintf(fgsession+ioff, "%s", "wujie");
		csstype=2;
	}
	else
	{
		return FALSE;
	}


	std::map<CString, strct_fganaly>::iterator It;
	It = s_mapFgDnsSession.find(fgsession);
	if(It == s_mapFgDnsSession.end())
	{
		strct_fganaly oneFg;
		oneFg.tmout = 0;
		oneFg.tmmatch = time(0);
		oneFg.countdip = 1;
		oneFg.countsum = 1;
		s_mapFgDnsSession[fgsession] = oneFg;
	}
	else
	{
		It->second.tmmatch = time(0);
		It->second.countsum ++;

		if(It->second.countsum > 2)
		{
			// ����3���ϴ����Ϊ5����ǰ,��5�����ڲ��ظ����
			if(abs(time(0)-It->second.tmout) > 5*60)
			{

				// 5�����������Ự����һ��
				HttpSession tmpsession(packh, g_httpFrom);
				tmpsession.m_OpenCaseFlg = v_opencasefgurl;
				tmpsession.m_OpenWarnFlg = v_openwarnfgurl;
				tmpsession.m_OpenIndxFlg = v_openflagfgurl;
				tmpsession.m_spyaddr = g_DDEPOS;
				tmpsession.m_dceprocotol=httpprotocl;
				DWORD state = 0;
				SearchCaseOrWarn(&tmpsession, NULL, 0, state);

				// д�����ļ�
				if(csstype==1)
					tmpsession.m_Requestline.m_Browser = TE_FREEGATE689b3;
				else if(csstype==2)
					tmpsession.m_Requestline.m_Browser = TE_WUJIE;
				//tmpsession.m_Requestline.m_BrowserVer = 0x686;
				tmpsession.WriteTeUrlIndexFile();

				It->second.tmout = time(0);
				It->second.countsum = 1;
			}
		}
	}

	/// �������1Сʱ�Բ����������ĻỰ
	CStringArray arrErase;
	time_t tmnow = time(0);
	for (It = s_mapFgSession.begin(); It != s_mapFgSession.end(); It++)
	{
		if(abs(tmnow-It->second.tmmatch) > 30*60)
		{
			// 30���������������, ��
			arrErase.Add(It->first);
		}
		else if(abs(tmnow-It->second.tmmatch) > 2*60 && It->second.countsum == 1)
		{
			// 2��������������ҵ�ǰ������Ϊ1, ��
			arrErase.Add(It->first);
		}
	}
	for (int iie = 0; iie < arrErase.GetSize() ; iie++)
	{
		s_mapFgDnsSession.erase(arrErase[iie]);
	}

	return TRUE;
}



BOOL isFreegate7( const unsigned char* pPacket, DATEHEADER& packh ,HttpProtocol* httpprotocl)
{
	 int offset=0x4a;
	 if(memcmp(pPacket+offset+5,"\x16\x03" ,2)!=0)
		return FALSE;
 
 
	char fgsession[100];
	memset(fgsession, 0, 100);
	int ioff = sprintf(fgsession, "%03u.%03u.%03u.%03u",
		packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3] );

	int csstype=0;
	Search  search1,search2;
	search1.InitSearch("confixx");
	search2.InitSearch("scooter-center");
	if(search1.find((char*)pPacket+offset+5,packh.m_datalen-0x4a-5)>0) //free7.08
	{
		csstype=2;
		 
	}
	else if(search2.find((char*)pPacket+offset+5,packh.m_datalen-0x4a-5)>0) //free7.07
	{
	 	csstype=1;
	}
	else 
	{
		return FALSE;
	}

	sprintf(fgsession+ioff, "%s", "freegate");

	std::map<CString, strct_fganaly>::iterator It;
	It = s_mapFgDnsSession.find(fgsession);
	if(It == s_mapFgDnsSession.end())
	{
		strct_fganaly oneFg;
		oneFg.tmout = 0;
		oneFg.tmmatch = time(0);
		oneFg.countdip = 1;
		oneFg.countsum = 1;
		s_mapFgDnsSession[fgsession] = oneFg;
	}
	else
	{
		It->second.tmmatch = time(0);
		It->second.countsum ++;

		if(It->second.countsum > 2)
		{
			// ����3���ϴ����Ϊ5����ǰ,��5�����ڲ��ظ����
			if(abs(time(0)-It->second.tmout) > 5*60)
			{

				// 5�����������Ự����һ��
				HttpSession tmpsession(packh, g_httpFrom);
				tmpsession.m_OpenCaseFlg = v_opencasefgurl;
				tmpsession.m_OpenWarnFlg = v_openwarnfgurl;
				tmpsession.m_OpenIndxFlg = v_openflagfgurl;
				tmpsession.m_spyaddr = g_DDEPOS;
				tmpsession.m_dceprocotol=httpprotocl;
				 
				//������
				CString tstr = tmpsession.m_sip;
				tmpsession.m_sip = tmpsession.m_dip;
				tmpsession.m_dip = tstr;	
				
				tstr = tmpsession.m_smac;
				tmpsession.m_smac = tmpsession.m_dmac;
				tmpsession.m_dmac = tstr;

			 

				// ����IP����
				DWORD ip;
				memcpy(&ip, tmpsession.m_csip, 4);
				memcpy(tmpsession.m_csip, tmpsession.m_cdip, 4);
				memcpy(tmpsession.m_cdip, &ip, 4);
				tmpsession.m_AuthInfo.SwapAuthInfo();





				DWORD state = 0;
				SearchCaseOrWarn(&tmpsession, NULL, 0, state);

				// д�����ļ�
				if(csstype==1)
					tmpsession.m_Requestline.m_Browser = TE_FREEGATE707;
				else if(csstype==2)
					tmpsession.m_Requestline.m_Browser = TE_FREEGATE708;
				//tmpsession.m_Requestline.m_BrowserVer = 0x686;
				tmpsession.WriteTeUrlIndexFile();

				It->second.tmout = time(0);
				It->second.countsum = 1;
			}
		}
	}

	/// �������1Сʱ�Բ����������ĻỰ
	CStringArray arrErase;
	time_t tmnow = time(0);
	for (It = s_mapFgSession.begin(); It != s_mapFgSession.end(); It++)
	{
		if(abs(tmnow-It->second.tmmatch) > 30*60)
		{
			// 30���������������, ��
			arrErase.Add(It->first);
		}
		else if(abs(tmnow-It->second.tmmatch) > 2*60 && It->second.countsum == 1)
		{
			// 2��������������ҵ�ǰ������Ϊ1, ��
			arrErase.Add(It->first);
		}
	}
	for (int iie = 0; iie < arrErase.GetSize() ; iie++)
	{
		s_mapFgDnsSession.erase(arrErase[iie]);
	}

	return TRUE;
}



BOOL isFreegate740( const unsigned char* pPacket, DATEHEADER& packh ,HttpProtocol* httpprotocl)
{
	 int csstype=0;
 	// int offset=0x4a;

	 if(memcmp(pPacket,"\x16\x03\x01\x00\x48\x01",6) ==0)
		 csstype = 1;   //client to server
	 else if (memcmp(pPacket,"\x16\x03\x01\x00\x51\x02",6) ==0 &&
				packh.m_datalen>0x3e8 &&
				memcmp(pPacket+188,"\x47\x6f\x6f\x67\x6c\x65\x20\x49\x6e\x74\x65\x72\x6e\x65\x74\x20\x41\x75\x74\x68\x6f\x72",22) ==0)
	 {
		 csstype = 2;   //server to client
	 }
	 else if (memcmp(pPacket,"\x16\x03\x01\x00\x51\x02",6) ==0 &&
				packh.m_datalen>0x3e8 &&
				memcmp(pPacket+186,"\x47\x6f\x6f\x67\x6c\x65\x20\x49\x6e\x74\x65\x72\x6e\x65\x74\x20\x41\x75\x74\x68\x6f\x72",22) ==0)
	 {
		 csstype = 3;
	 }
	 else
		return FALSE;
 
 
	char fgsession[100];
	int ioff;
	memset(fgsession, 0, 100);
	if (csstype ==1)
	{
		 ioff = sprintf(fgsession, "%03u.%03u.%03u.%03u_%03u.%03u.%03u.%03u",
			packh.m_sip[0],packh.m_sip[1],packh.m_sip[2],packh.m_sip[3],packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3]);
	}
	else if (csstype == 2 || csstype == 3)
	{
		 ioff = sprintf(fgsession, "%03u.%03u.%03u.%03u_%03u.%03u.%03u.%03u",
			packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3],packh.m_sip[0],packh.m_sip[1],packh.m_sip[2],packh.m_sip[3]);
	}

	sprintf(fgsession+ioff, "%s", "freegate");

//	WriteLog("dce","fgsession:%s",fgsession);

	try
	{
	
		std::map<CString, strct_fganaly>::iterator It;
		It = s_mapFgDnsSession.find(fgsession);
		if(It == s_mapFgDnsSession.end())
		{
			strct_fganaly oneFg;
			oneFg.tmout = 0;
			oneFg.tmmatch = time(0);
			oneFg.countdip = 1;
			oneFg.countsum = 1;
			if (csstype == 1)
			{
				oneFg.clientoccur =1;
			}
			else if (csstype == 2 || csstype == 3)
			{
				oneFg.serveroccur =1;
			}
			s_mapFgDnsSession[fgsession] = oneFg;
		}
		else
		{
			It->second.tmmatch = time(0);
			It->second.countsum ++;

			if (csstype == 1)
			{
				It->second.clientoccur =1;
			}
			else if (csstype == 2 || csstype == 3)
			{
				It->second.serveroccur =1;
			}
		}

		/// �������1Сʱ�Բ����������ĻỰ
		CStringArray arrErase;
		time_t tmnow = time(0);
		for (It = s_mapFgDnsSession.begin(); It != s_mapFgDnsSession.end(); It++)
		{
			//��������Ự�������ˣ���д�ļ���Ȼ�����
			if (It->second.clientoccur==1 && It->second.serveroccur==1)
			{
				HttpSession tmpsession(packh, g_httpFrom);
				tmpsession.m_OpenCaseFlg = v_opencasefgurl;
				tmpsession.m_OpenWarnFlg = v_openwarnfgurl;
				tmpsession.m_OpenIndxFlg = v_openflagfgurl;
				tmpsession.m_spyaddr = g_DDEPOS;
				tmpsession.m_dceprocotol=httpprotocl;
				
				if (csstype == 2 || csstype == 3)
				{
					//������
					CString tstr = tmpsession.m_sip;
					tmpsession.m_sip = tmpsession.m_dip;
					tmpsession.m_dip = tstr;	
					
					tstr = tmpsession.m_smac;
					tmpsession.m_smac = tmpsession.m_dmac;
					tmpsession.m_dmac = tstr;

					// ����IP����
					DWORD ip;
					memcpy(&ip, tmpsession.m_csip, 4);
					memcpy(tmpsession.m_csip, tmpsession.m_cdip, 4);
					memcpy(tmpsession.m_cdip, &ip, 4);
				}
				tmpsession.m_AuthInfo.SwapAuthInfo();

				DWORD state = 0;
				SearchCaseOrWarn(&tmpsession, NULL, 0, state);

				// д�����ļ�
				if(csstype == 2)
				{
					tmpsession.m_Requestline.m_Browser = TE_FREEGATE740;
				}
				else if(csstype == 3)
				{
					tmpsession.m_Requestline.m_Browser = TE_FREEGATE742;
				}
				
				tmpsession.WriteTeUrlIndexFile();

				It->second.clientoccur = 0;
				It->second.serveroccur = 0;

			//	WriteLog("dce","It->first:%s",It->first);
				arrErase.Add(It->first);
			}

			if(abs(tmnow-It->second.tmmatch) > 30*60)
			{
				// 30���������������, ��
				arrErase.Add(It->first);
			}
			else if(abs(tmnow-It->second.tmmatch) > 2*60 && It->second.countsum == 1)
			{
				// 2��������������ҵ�ǰ������Ϊ1, ��
				arrErase.Add(It->first);
			}
		}

	//	WriteLog("dce","����ǰ��hash��СΪ :%d",arrErase.GetSize());
		for (int iie = 0; iie < arrErase.GetSize() ; iie++)
		{
			s_mapFgDnsSession.erase(arrErase[iie]);
		}

// 		int icount=0;
// 		for (It = s_mapFgDnsSession.begin(); It != s_mapFgDnsSession.end(); It++)
// 		{
// 			icount++;
// 		}
	
//		WriteLog("dce","�����hash��СΪ :%d",icount);
	}
	catch (...)
	{
		WriteLog("dce","isFreegate740 catch error!");
	}
	return TRUE;
}
 

 