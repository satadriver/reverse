#include "stdafx.h"
#include "dcepub.h"


//HANDLE MYHEAP=NULL;
 UINT		v_programtype=0;		// 系统类型
 BOOL		v_stat=TRUE;			// 是否启用统计
// Modify by zzh 2006-10-9
BOOL		v_statIdc		= TRUE;		// IDC管理 统计开关
BOOL		v_statHttpup	= TRUE;		// 虚拟身份	Httpup统计开关
BOOL		v_statImsgQQ	= TRUE;		// 虚拟身份	即时消息QQ统计开关
BOOL		v_statImsgOther = TRUE;		// 虚拟身份	即时消息其他统计开关
BOOL		v_statMail		= TRUE;		// 虚拟身份	邮件统计开关
BOOL		v_statMedia		= TRUE;		// 虚拟身份	音视频统计开关
BOOL		v_statGame		= TRUE;		// 虚拟身份	游戏统计开关
BOOL		v_statOther		= TRUE;		// 虚拟身份	其它(除以上)统计开关
BOOL		v_httpurl		= TRUE;		// 回溯httpurl是否写开关
CString CStringEx::GetStringEx( LPCTSTR fm, ... )
{
	CString str;
	va_list args;
	va_start( args, fm );
	str.FormatV(fm, args);
	va_end( args );
	
	return str;
}
CString		v_datapath;
CString		v_RootPath("d:\\netspy");
CString		v_WorkPath("e:\\netspy");
CString		v_StatPath("e:\\netspy\\statdata");
CString		v_DdeBakPath("e:\\ddebak");
CString		g_strRootPara	= v_WorkPath+"\\para\\dce";		// 参数根目录
CString		g_DelDceParaFlg = v_WorkPath+"\\flg\\deldcepara.flg";
CString		CONFIGFILE		= v_RootPath+"\\ini\\local.ini";
CString		POSCONFIGINI	= v_WorkPath+"\\para\\Intercept.ini";
CString		ZONEPOSINI		= "d:\\netspy\\ini\\serial.dat";

CString		g_ddepath;//dde工作路径
CString		g_dcepath;//dce工作路径

BOOL		v_DceRunFlg=TRUE;
 int		v_workmode=3;				// 当前dce的工作模式(缺省:3)

 DCETEARRAY  v_DceTeSpread;				// DCE列表,扩展特流用
//  SERVERARRAY v_CaseServer;			// 案件服务器数组
//  SERVERARRAY v_IndexServer;			// 全文服务器数组
//  SERVERARRAY v_WarnServer;			// 预警服务器数组
//  SERVERARRAY v_DseServer;			// dse服务器数组

 DDELIST	 v_ddelist;					// dde机器链表
 UINT		 v_ddethreadnum=0;			// dde的线程总数
 long		 v_bddefilebak[16];			// dde文件备份的标志
 BOOL		 v_bLanguage=FALSE;			// 是否处理语种(缺省是否)
 int		 v_SWordRatio=3;			// 词正确率(维语)
 int	 	 v_EWordRatio=5;			// 词正确率(英语)
 long 		 v_mediainterval=600;		// 音视频轮换时长，作为dce吐文件间隔时间(缺省10分钟)
 long		 v_mediasample   = 1;		// 音视频抽样率
 long		 v_qqnum   = 0;				// 当前qq数量
 long		 v_ucnum   = 0;				// 当前uc数量

 long		g_bakpro = 0;	// 是否有备份协议, 如果无则0
 // 注: 此备份无删除机制, 如果已经达到最大备份量自动终止
 // 若需终止需修改配置, 若需备新数据需删旧文件
 long		g_arrbakpro[PRONUM_MAX];		// 下标为协议,值为最大备份量(单位K),取自配置


// 全文开放标志
 BOOL v_openflagsmtp		= TRUE;
 BOOL v_openflagpop3		= TRUE;
 BOOL v_openflaghttpup		= TRUE;
 BOOL v_openflaghttpdown	= TRUE;
 BOOL v_openflagftp			= TRUE;
 BOOL v_openflagtelnet		= TRUE;
 BOOL v_openflagoicq		= TRUE;
 BOOL v_openflagyahoo		= TRUE;
 BOOL v_openflagmsn			= TRUE;
 BOOL v_openflagaol			= TRUE;
 BOOL v_openflagchuanqi		= TRUE;
 BOOL v_openflaglzgame		= TRUE;
 BOOL v_openflagnog			= TRUE;
 BOOL v_openflagvgate		= TRUE;
 BOOL v_openflagindeximage	=FALSE;
 BOOL v_openflagbf			= TRUE;
 BOOL v_openflagwow			= TRUE;
 BOOL v_openflagSoQ			= TRUE;	
 BOOL v_openflagEht			= TRUE;
 BOOL v_openflagVp			= TRUE;
 BOOL v_openflagemule		= TRUE;
 BOOL v_openflagvnc			= TRUE;
 BOOL v_openflagrtsp		= TRUE;
 BOOL v_openflaghf			= TRUE;
 BOOL v_openflagunip		= TRUE;
 BOOL v_openflagtaobao		= TRUE;
 BOOL v_openflaguc			= TRUE;
 BOOL v_openflagfetion		= TRUE;
 BOOL v_openflagpopo		= TRUE;
 BOOL v_openflagqqgame		= TRUE;

 BOOL v_openflagtrojan		= TRUE;

 BOOL v_openflagwebmail			= TRUE;
 BOOL v_openflaghttpurl		= TRUE;
 BOOL v_openflagremote		= TRUE;
 BOOL v_openflagterminal			= TRUE;
 BOOL v_openflaghttps		= TRUE;
 BOOL v_openflagoicqcontent		= TRUE;
 BOOL v_openflagoicqraw		= TRUE;
 BOOL v_openflaggameperfect			= TRUE;
 BOOL v_openflagXunTongMedia		= TRUE;
 BOOL v_openflagwujieup		= TRUE;
 BOOL v_openflagwujiedn		= TRUE;
 BOOL v_openflagfirephoenix			= TRUE;
 BOOL v_openflagimsg51		= TRUE;
 BOOL v_openflagdiicall		= TRUE;
 BOOL v_openflagwcall			= TRUE;
 BOOL v_openflagstarnet		= TRUE;
 BOOL v_openflageyebeam		= TRUE;
 BOOL v_openflaggizmo		= TRUE;
 BOOL v_openflagmtalk		= TRUE;
 BOOL v_openflagmrtalk		= TRUE;
 BOOL v_openflagheyoo		= TRUE;
 BOOL v_openflagteltel		= TRUE;
 BOOL v_openflag66call		= TRUE;
 BOOL v_openflagqqtranfile		= TRUE;
 BOOL v_openflagdayingjia		= TRUE;
 BOOL v_openflagbaiduhi		= TRUE;




// 预警开放标志
 BOOL v_openwarnsmtp;
 BOOL v_openwarnpop3;
 BOOL v_openwarnhttpup;
 BOOL v_openwarnhttpdown;
 BOOL v_openwarnftp;
 BOOL v_openwarntelnet;
 BOOL v_openwarnoicq;
 BOOL v_openwarnyahoo;
 BOOL v_openwarnmsn;
 BOOL v_openwarnaol;
 BOOL v_openwarnchuanqi;
 BOOL v_openwarnlzgame;
 BOOL v_openwarnnog;
 BOOL v_openwarnindeximage;
 BOOL v_openwarnvgate;
 BOOL v_openwarnbf;
 BOOL v_openwarnwow;
 BOOL v_openwarnSoQ;
 BOOL v_openwarnEht;
 BOOL v_openwarnVp;
 BOOL v_openwarnemule;
 BOOL v_openwarnvnc;
 BOOL v_openwarnrtsp;
 BOOL v_openwarnhf;
 BOOL v_openwarnunip;
 BOOL v_openwarntaobao;
 BOOL v_openwarnuc;
 BOOL v_openwarnfetion;
 BOOL v_openwarnpopo;
 BOOL v_openwarnqqgame;

 BOOL v_openwarntrojan;

// 全文NOG部分
 BOOL v_openflagfgup		= TRUE;	
 BOOL v_openflagfgdn		= TRUE;
 BOOL v_openflagfgurl		= TRUE;
 BOOL v_openflagdynaup		= TRUE;
 BOOL v_openflagdynadn		= TRUE;
 BOOL v_openflagdynaurl		= TRUE;
 BOOL v_openflagwujie		= TRUE;

// 预警NOG部分
 BOOL v_openwarnfgup;	
 BOOL v_openwarnfgdn;
 BOOL v_openwarnfgurl;
 BOOL v_openwarndynaup;
 BOOL v_openwarndynadn;
 BOOL v_openwarndynaurl;
 BOOL v_openwarnwujie;


// 预警视频开放标志
 BOOL v_openflagQQMedia		= TRUE;
 BOOL v_openflagBLMedia		= TRUE;
 BOOL v_openflagEPHMedia	= TRUE;
 BOOL v_openflagMSNMedia	= TRUE;
 BOOL v_openflagPoPoMedia	= TRUE;
 BOOL v_openflagFlashMedia	= TRUE;
 BOOL v_openflagSoQMedia	= TRUE;
 BOOL v_openflagUCMedia		= TRUE;
 BOOL v_openflagYahooMedia	= TRUE;
 BOOL v_openflagNetMetMedia	= TRUE;
 BOOL v_openflagSkypeMedia	= TRUE;
 BOOL v_openflagVoip		= TRUE;
 BOOL v_openflagBDCMedia	= TRUE;
 BOOL v_openflagOYesCamMedia= TRUE;
 BOOL v_openflagVPMedia		= TRUE;
 BOOL v_openflagASFMedia	= TRUE;
 BOOL v_openUUTongMedia		= TRUE;


// 案件数据开关
 BOOL v_opencasesmtp	= TRUE;
 BOOL v_opencasepop3	= TRUE;
 BOOL v_opencasehttpup	= TRUE;
 BOOL v_opencasehttpdown= TRUE;
 BOOL v_opencaseftp		= TRUE;
 BOOL v_opencasetelnet	= TRUE;
 BOOL v_opencasehttpimage=FALSE;
 BOOL v_opencaseoicq	= TRUE;
 BOOL v_opencaseyahoo	= TRUE;
 BOOL v_opencasemsn		= TRUE;
 BOOL v_opencaseaol		= TRUE;
 BOOL v_opencasechuanqi	= TRUE;
 BOOL v_opencaselzgame	= TRUE;
 BOOL v_opencasenog		= TRUE;
 BOOL v_opencasevgate	= TRUE;
 BOOL v_opencasebf		= TRUE;
 BOOL v_opencasewow		= TRUE;
 BOOL v_opencaseSoQ		= TRUE;	
 BOOL v_opencaseEht		= TRUE;
 BOOL v_opencaseVp		= TRUE;
 BOOL v_opencaseemule	= TRUE;
 BOOL v_opencasevnc		= TRUE;
 BOOL v_opencasertsp	= TRUE;
 BOOL v_opencasehf		= TRUE;
 BOOL v_opencaseunip	= TRUE;
 BOOL v_opencasetaobao	= TRUE;
 BOOL v_opencaseuc		= TRUE;
 BOOL v_opencasefetion	= TRUE;
 BOOL v_opencasepopo	= TRUE;
 BOOL v_opencaseqqgame	= TRUE;

 BOOL v_opencasetrojan = TRUE;

// 案件NOG部分
 BOOL v_opencasefgup		= TRUE;	
 BOOL v_opencasefgdn		= TRUE;
 BOOL v_opencasefgurl		= TRUE;
 BOOL v_opencasedynaup		= TRUE;
 BOOL v_opencasedynadn		= TRUE;
 BOOL v_opencasedynaurl		= TRUE;
 BOOL v_opencasewujie		= TRUE;


// 案件视频开放标志
 BOOL v_opencaseQQMedia		= TRUE;
 BOOL v_opencaseBLMedia		= TRUE;
 BOOL v_opencaseEPHMedia	= TRUE;
 BOOL v_opencaseMSNMedia	= TRUE;
 BOOL v_opencasePoPoMedia	= TRUE;
 BOOL v_opencaseFlashMedia	= TRUE;
 BOOL v_opencaseSoQMedia	= TRUE;
 BOOL v_opencaseUCMedia		= TRUE;
 BOOL v_opencaseYahooMedia	= TRUE;
 BOOL v_opencaseNetMetMedia	= TRUE;
 BOOL v_opencaseSkypeMedia	= TRUE;
 BOOL v_opencaseVoip		= TRUE;
 BOOL v_opencaseBDCMedia	= TRUE;
 BOOL v_opencaseOYesCamMedia= TRUE;
 BOOL v_opencaseVPMedia		= TRUE;
 BOOL v_opencaseASFMedia	= TRUE;
 BOOL v_opencaseUUTongMedia	= TRUE;

// 预警视频开放标志
 BOOL v_openwarnQQMedia;			
 BOOL v_openwarnBLMedia;			
 BOOL v_openwarnEPHMedia;		
 BOOL v_openwarnMSNMedia;		
 BOOL v_openwarnPoPoMedia;		
 BOOL v_openwarnFlashMedia;		
 BOOL v_openwarnSoQMedia;		
 BOOL v_openwarnUCMedia;			
 BOOL v_openwarnYahooMedia;		
 BOOL v_openwarnNetMetMedia;		
 BOOL v_openwarnSkypeMedia;		
 BOOL v_openwarnVoip;			
 BOOL v_openwarnBDCMedia;		
 BOOL v_openwarnASFMedia;		
 BOOL v_openwarnOYesCamMedia;	
 BOOL v_openwarnVPMedia;			
 BOOL v_openwarnUUTongMedia;	

 BOOL v_Istz;

//  FILE* v_dstfilenamefp;
//  FILE* v_dstindexfilenamefp;
//  int v_filenamenum=0;


 CCrc32	g_crc32Calc;

 Search	SearchCRLF("\r\n");
 Search	Search2CRLF("\r\n\r\n");
 Search	SearchCRLFUnix("\n");
 Search	Search2CRLFUnix("\n\n");
 Search	SearchEmlReceived("\nReceived: ");

 BYTE *v_pLeaglIP = new BYTE[65536];

// 各个插件的运行状态变量
#ifdef		ADD_RUNSTATE_INFO
	 RUNDEBUG	v_httprunstate;
	 RUNDEBUG	v_imsgrunstate;
#endif

 STAT v_statdata[TOTALPRONUM];
long warnacc = 0;
 BOOL V_WriteZ = TRUE;

// DWORD	v_flag = GetTickCount();

 BYTE	g_test[16];		// 播放文件的密钥

 char* g_encodedkeybuf=NULL;
 int  g_encodedlen;
// CHECKDOGPROC v_Proc=NULL;

// DWORD v_totalPower;
// DWORD v_modPowertoDse[100];

 int		g_processAAA; //是否需要写认证


 
 // add by cxl
 bool g_b3g = 0; //是否g
 bool g_bAAA = 0;//是否 AAA
 bool g_bNormal = 0;// putong
 // end

