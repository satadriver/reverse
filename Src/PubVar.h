/*************  ȫ�ֱ�������  ************/

#ifndef DCE_PUBLIC_VARIABLE_H__20050506__INCLUDED
#define DCE_PUBLIC_VARIABLE_H__20050506__INCLUDED


#if _MSC_VER > 1000
#pragma once
#endif

#include "Algorithm.h"

class CStringEx
{
public:
	static CString GetStringEx(LPCTSTR fm, ...);
};

// ȫ�ֱ�������
extern	int			m_ddecount;			// dde�ĸ���
extern	char		v_localip[20];		// ����IP
extern	CStringArray v_ParaServer;		// ��������������(�µ�CS)
extern	CString		v_RootPath;			// ϵͳ��·��
extern	CString		v_StatPath;			// Statdata·��
extern	CString		v_WorkPath;			// ����Ĺ���·��(���ļ������·������ʱ·��)
extern	CString		v_DdeBakPath;
extern	CString		g_strRootPara;		// ������Ŀ¼, ��e:\netspy\dce\para
extern	CString		g_DelDceParaFlg;
extern	CString		CONFIGFILE;
extern	CString		POSCONFIGINI;
extern	CString		ZONEPOSINI;			// zoneϵͳ�µĽػ�������ļ�

extern	UINT		v_programtype;		// ϵͳ����
extern	BOOL		v_DceRunFlg;		// ��������б�־
extern	DCETEARRAY  v_DceTeSpread;		// dce��չ������
extern	DDELIST		v_ddelist;			// dde��������
extern	UINT		v_ddethreadnum;		// dde���߳�����
extern	long		v_bddefilebak[16];	// dde�ļ����ݵı�־
extern	int			v_workmode;			// ��ǰdce�Ĺ���ģʽ(ȱʡ:3)
extern	BOOL		v_bLanguage;		// �Ƿ�������
extern	int			v_SWordRatio;		// ����ȷ��(ά��)
extern	int			v_EWordRatio;		// ����ȷ��(Ӣ��)
extern	long		g_bakpro;			// �Ƿ��б���Э��, �������0
extern	long		g_arrbakpro[PRONUM_MAX];	// �±�ΪЭ��,ֵΪ��󱸷���(��λK),ȡ������

extern	BOOL		v_stat;				// �Ƿ�����ͳ��(��)
extern	BOOL		v_statIdc;			// IDC���� ͳ�ƿ���
extern	BOOL		v_statHttpup;		// �������	Httpupͳ�ƿ���
extern	BOOL		v_statImsgQQ;		// �������	��ʱ��ϢQQͳ�ƿ���
extern	BOOL		v_statImsgOther;	// �������	��ʱ��Ϣ����ͳ�ƿ���
extern	BOOL		v_statMail;			// �������	�ʼ�ͳ�ƿ���
extern	BOOL		v_statMedia;		// �������	����Ƶͳ�ƿ���
extern	BOOL		v_statGame;			// �������	��Ϸͳ�ƿ���
extern	BOOL		v_statOther;		// �������	����(������)ͳ�ƿ���
extern	long		v_mediasample;		// ����Ƶ������
extern	BOOL		v_httpurl;			// ����httpurl�Ƿ�д����

extern long		v_mediainterval;		// ����Ƶ�ֻ�ʱ������Ϊdce���ļ����ʱ��
extern long		v_qqnum ;				//��ǰqq����
extern long		v_ucnum ;				//��ǰuc����


// ȫ�Ŀ��ű�־
extern BOOL v_openflagsmtp;
extern BOOL v_openflagpop3;
extern BOOL v_openflaghttpup;
extern BOOL v_openflaghttpdown;
extern BOOL v_openflagftp;
extern BOOL v_openflagtelnet;
extern BOOL v_openflagoicq;
extern BOOL v_openflagyahoo;
extern BOOL v_openflagmsn;
extern BOOL v_openflagaol;
extern BOOL v_openflagchuanqi;
extern BOOL v_openflaglzgame;
extern BOOL v_openflagnog;
extern BOOL v_openflagindeximage;
extern BOOL v_openflagvgate;
extern BOOL v_openflagbf;
extern BOOL v_openflagwow;
extern BOOL v_openflagSoQ;
extern BOOL v_openflagEht;
extern BOOL v_openflagVp;
extern BOOL v_openflagemule;
extern BOOL v_openflagvnc;
extern BOOL v_openflagrtsp;
extern BOOL v_openflaghf;
extern BOOL v_openflagunip;
extern BOOL v_openflagtaobao;
extern BOOL v_openflaguc;
extern BOOL v_openflagfetion;
extern BOOL v_openflagpopo;
extern BOOL v_openflagqqgame;
extern BOOL v_openflagtrojan;
extern BOOL v_openflagwebmail		;
extern BOOL v_openflaghttpurl		;
extern BOOL v_openflagremote		;
extern BOOL v_openflagterminal	;
extern BOOL v_openflaghttps		;
extern BOOL v_openflagoicqcontent	;
extern BOOL v_openflagoicqraw		;
extern BOOL v_openflaggameperfect	;
extern BOOL v_openflagXunTongMedia;
extern BOOL v_openflagwujieup		;
extern BOOL v_openflagwujiedn		;
extern BOOL v_openflagfirephoenix	;
extern BOOL v_openflagimsg51		;
extern BOOL v_openflagdiicall		;
extern BOOL v_openflagwcall		;
extern BOOL v_openflagstarnet		;
extern BOOL v_openflageyebeam		;
extern BOOL v_openflaggizmo		;
extern BOOL v_openflagmtalk		;
extern BOOL v_openflagmrtalk		;
extern BOOL v_openflagheyoo		;
extern BOOL v_openflagteltel		;
extern BOOL v_openflag66call		;
extern BOOL v_openflagqqtranfile	;
extern BOOL v_openflagdayingjia	;
extern BOOL v_openflagbaiduhi		;

// Ԥ�����ű�־
extern BOOL v_openwarnsmtp;
extern BOOL v_openwarnpop3;
extern BOOL v_openwarnhttpup;
extern BOOL v_openwarnhttpdown;
extern BOOL v_openwarnftp;
extern BOOL v_openwarntelnet;
extern BOOL v_openwarnoicq;
extern BOOL v_openwarnyahoo;
extern BOOL v_openwarnmsn;
extern BOOL v_openwarnaol;
extern BOOL v_openwarnchuanqi;
extern BOOL v_openwarnlzgame;
extern BOOL v_openwarnnog;
extern BOOL v_openwarnindeximage;
extern BOOL v_openwarnvgate;
extern BOOL v_openwarnbf;
extern BOOL v_openwarnwow;
extern BOOL v_openwarnSoQ;
extern BOOL v_openwarnEht;
extern BOOL v_openwarnVp;
extern BOOL v_openwarnemule;
extern BOOL v_openwarnvnc;
extern BOOL v_openwarnrtsp;
extern BOOL v_openwarnhf;
extern BOOL v_openwarnunip;
extern BOOL v_openwarntaobao;
extern BOOL v_openwarnuc;
extern BOOL v_openwarnfetion;
extern BOOL v_openwarnpopo;
extern BOOL v_openwarnqqgame;

extern BOOL v_openwarntrojan;

// ȫ��NOG����
extern BOOL v_openflagfgup;	
extern BOOL v_openflagfgdn;
extern BOOL v_openflagfgurl;
extern BOOL v_openflagdynaup;
extern BOOL v_openflagdynadn;
extern BOOL v_openflagdynaurl;
extern BOOL v_openflagwujie;

// Ԥ��NOG����
extern BOOL v_openwarnfgup;	
extern BOOL v_openwarnfgdn;
extern BOOL v_openwarnfgurl;
extern BOOL v_openwarndynaup;
extern BOOL v_openwarndynadn;
extern BOOL v_openwarndynaurl;
extern BOOL v_openwarnwujie;

// ȫ����Ƶ���ű�־
extern BOOL v_openflagQQMedia;			
extern BOOL v_openflagBLMedia;			
extern BOOL v_openflagEPHMedia;		
extern BOOL v_openflagMSNMedia;		
extern BOOL v_openflagPoPoMedia;		
extern BOOL v_openflagFlashMedia;		
extern BOOL v_openflagSoQMedia;		
extern BOOL v_openflagUCMedia;			
extern BOOL v_openflagYahooMedia;		
extern BOOL v_openflagNetMetMedia;		
extern BOOL v_openflagSkypeMedia;		
extern BOOL v_openflagVoip;			
extern BOOL v_openflagBDCMedia;		
extern BOOL v_openflagASFMedia;		
extern BOOL v_openflagOYesCamMedia;	
extern BOOL v_openflagVPMedia;			
extern BOOL v_openUUTongMedia;	

// Ԥ����Ƶ���ű�־
extern BOOL v_openwarnQQMedia;			
extern BOOL v_openwarnBLMedia;			
extern BOOL v_openwarnEPHMedia;		
extern BOOL v_openwarnMSNMedia;		
extern BOOL v_openwarnPoPoMedia;		
extern BOOL v_openwarnFlashMedia;		
extern BOOL v_openwarnSoQMedia;		
extern BOOL v_openwarnUCMedia;			
extern BOOL v_openwarnYahooMedia;		
extern BOOL v_openwarnNetMetMedia;		
extern BOOL v_openwarnSkypeMedia;		
extern BOOL v_openwarnVoip;			
extern BOOL v_openwarnBDCMedia;		
extern BOOL v_openwarnASFMedia;		
extern BOOL v_openwarnOYesCamMedia;	
extern BOOL v_openwarnVPMedia;			
extern BOOL v_openwarnUUTongMedia;	

// �������ű�־
extern BOOL v_opencasesmtp;
extern BOOL v_opencasepop3;
extern BOOL v_opencasehttpup;
extern BOOL v_opencasehttpdown;
extern BOOL v_opencaseftp;
extern BOOL v_opencasetelnet;
extern BOOL v_opencasehttpimage;
extern BOOL v_opencaseoicq;
extern BOOL v_opencaseyahoo;
extern BOOL v_opencasemsn;
extern BOOL v_opencaseaol;
extern BOOL v_opencasechuanqi;
extern BOOL v_opencaselzgame;
extern BOOL v_opencasenog;
extern BOOL v_opencasevgate;
extern BOOL v_opencasebf;
extern BOOL	v_opencasewow;
extern BOOL v_opencaseSoQ;
extern BOOL v_opencaseEht;
extern BOOL v_opencaseVp;
extern BOOL v_opencaseemule;
extern BOOL v_opencasevnc;
extern BOOL v_opencasertsp;
extern BOOL v_opencasehf;
extern BOOL v_opencaseunip;
extern BOOL v_opencasetaobao;
extern BOOL v_opencaseuc;
extern BOOL v_opencasefetion;
extern BOOL v_opencasepopo;
extern BOOL v_opencaseqqgame;
extern BOOL v_opencasetrojan;

// ȫ��NOG����
extern BOOL v_opencasefgup;	
extern BOOL v_opencasefgdn;
extern BOOL v_opencasefgurl;
extern BOOL v_opencasedynaup;
extern BOOL v_opencasedynadn;
extern BOOL v_opencasedynaurl;
extern BOOL v_opencasewujie;

// ������Ƶ���ű�־
extern BOOL v_opencaseQQMedia;			
extern BOOL v_opencaseBLMedia;			
extern BOOL v_opencaseEPHMedia;		
extern BOOL v_opencaseMSNMedia;		
extern BOOL v_opencasePoPoMedia;		
extern BOOL v_opencaseFlashMedia;		
extern BOOL v_opencaseSoQMedia;		
extern BOOL v_opencaseUCMedia;			
extern BOOL v_opencaseYahooMedia;		
extern BOOL v_opencaseNetMetMedia;		
extern BOOL v_opencaseSkypeMedia;		
extern BOOL v_opencaseVoip;			
extern BOOL v_opencaseBDCMedia;		
extern BOOL v_opencaseASFMedia;		
extern BOOL v_opencaseOYesCamMedia;	
extern BOOL v_opencaseVPMedia;			
extern BOOL v_opencaseUUTongMedia;	

extern BOOL v_Istz;	

extern Search	SearchCRLF;
extern Search	Search2CRLF;
extern Search	SearchCRLFUnix;
extern Search	Search2CRLFUnix;
extern Search	SearchEmlReceived;

extern BYTE	*v_pLeaglIP;		// �Ϸ�IP��

// �������������״̬����
#ifdef		ADD_RUNSTATE_INFO
	extern RUNDEBUG	v_httprunstate;
	extern RUNDEBUG	v_imsgrunstate;
#endif

extern STAT v_statdata[TOTALPRONUM];
extern long warnacc;
extern BOOL V_WriteZ;

extern BYTE	g_test[16];		// �����ļ�����Կ
extern char* g_encodedkeybuf;//����ƵRSA���ܵ���Կ�Ļ�����
extern int  g_encodedlen;    //����ƵRSA����Կ����

extern CString		g_ddepath;//dde����·��
extern CString		g_dcepath;//dce����·��
extern CString		v_datapath;

extern int		g_processAAA; //�Ƿ���Ҫд��֤

// add by cxl
extern bool g_b3g;			//�Ƿ�g
extern bool g_bAAA;			//�Ƿ� AAA
extern bool g_bNormal;		//��ͨ
 // end

#endif