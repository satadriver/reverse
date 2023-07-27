/*************  全局变量定义  ************/

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

// 全局变量定义
extern	int			m_ddecount;			// dde的个数
extern	char		v_localip[20];		// 本机IP
extern	CStringArray v_ParaServer;		// 参数服务器数组(新的CS)
extern	CString		v_RootPath;			// 系统根路径
extern	CString		v_StatPath;			// Statdata路径
extern	CString		v_WorkPath;			// 程序的工作路径(即文件的输出路径和临时路径)
extern	CString		v_DdeBakPath;
extern	CString		g_strRootPara;		// 参数根目录, 如e:\netspy\dce\para
extern	CString		g_DelDceParaFlg;
extern	CString		CONFIGFILE;
extern	CString		POSCONFIGINI;
extern	CString		ZONEPOSINI;			// zone系统下的截获点配置文件

extern	UINT		v_programtype;		// 系统类型
extern	BOOL		v_DceRunFlg;		// 程序的运行标志
extern	DCETEARRAY  v_DceTeSpread;		// dce扩展特流用
extern	DDELIST		v_ddelist;			// dde机器链表
extern	UINT		v_ddethreadnum;		// dde的线程总数
extern	long		v_bddefilebak[16];	// dde文件备份的标志
extern	int			v_workmode;			// 当前dce的工作模式(缺省:3)
extern	BOOL		v_bLanguage;		// 是否处理语种
extern	int			v_SWordRatio;		// 词正确率(维语)
extern	int			v_EWordRatio;		// 词正确率(英语)
extern	long		g_bakpro;			// 是否有备份协议, 如果无则0
extern	long		g_arrbakpro[PRONUM_MAX];	// 下标为协议,值为最大备份量(单位K),取自配置

extern	BOOL		v_stat;				// 是否启用统计(总)
extern	BOOL		v_statIdc;			// IDC管理 统计开关
extern	BOOL		v_statHttpup;		// 虚拟身份	Httpup统计开关
extern	BOOL		v_statImsgQQ;		// 虚拟身份	即时消息QQ统计开关
extern	BOOL		v_statImsgOther;	// 虚拟身份	即时消息其他统计开关
extern	BOOL		v_statMail;			// 虚拟身份	邮件统计开关
extern	BOOL		v_statMedia;		// 虚拟身份	音视频统计开关
extern	BOOL		v_statGame;			// 虚拟身份	游戏统计开关
extern	BOOL		v_statOther;		// 虚拟身份	其它(除以上)统计开关
extern	long		v_mediasample;		// 音视频抽样率
extern	BOOL		v_httpurl;			// 回溯httpurl是否写开关

extern long		v_mediainterval;		// 音视频轮换时长，作为dce吐文件间隔时间
extern long		v_qqnum ;				//当前qq数量
extern long		v_ucnum ;				//当前uc数量


// 全文开放标志
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

// 预警开放标志
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

// 全文NOG部分
extern BOOL v_openflagfgup;	
extern BOOL v_openflagfgdn;
extern BOOL v_openflagfgurl;
extern BOOL v_openflagdynaup;
extern BOOL v_openflagdynadn;
extern BOOL v_openflagdynaurl;
extern BOOL v_openflagwujie;

// 预警NOG部分
extern BOOL v_openwarnfgup;	
extern BOOL v_openwarnfgdn;
extern BOOL v_openwarnfgurl;
extern BOOL v_openwarndynaup;
extern BOOL v_openwarndynadn;
extern BOOL v_openwarndynaurl;
extern BOOL v_openwarnwujie;

// 全文视频开放标志
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

// 预警视频开放标志
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

// 案件开放标志
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

// 全文NOG部分
extern BOOL v_opencasefgup;	
extern BOOL v_opencasefgdn;
extern BOOL v_opencasefgurl;
extern BOOL v_opencasedynaup;
extern BOOL v_opencasedynadn;
extern BOOL v_opencasedynaurl;
extern BOOL v_opencasewujie;

// 案件视频开放标志
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

extern BYTE	*v_pLeaglIP;		// 合法IP表

// 各个插件的运行状态变量
#ifdef		ADD_RUNSTATE_INFO
	extern RUNDEBUG	v_httprunstate;
	extern RUNDEBUG	v_imsgrunstate;
#endif

extern STAT v_statdata[TOTALPRONUM];
extern long warnacc;
extern BOOL V_WriteZ;

extern BYTE	g_test[16];		// 播放文件的密钥
extern char* g_encodedkeybuf;//音视频RSA加密的密钥的缓冲区
extern int  g_encodedlen;    //音视频RSA后密钥长度

extern CString		g_ddepath;//dde工作路径
extern CString		g_dcepath;//dce工作路径
extern CString		v_datapath;

extern int		g_processAAA; //是否需要写认证

// add by cxl
extern bool g_b3g;			//是否g
extern bool g_bAAA;			//是否 AAA
extern bool g_bNormal;		//普通
 // end

#endif