// LimitPara.h: interface for the LimitPara class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIMITPARA_H__C7D515F3_BD5A_4F1E_8A5A_6571F36A639A__INCLUDED_)
#define AFX_LIMITPARA_H__C7D515F3_BD5A_4F1E_8A5A_6571F36A639A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "rbtempl.h"
#include <list>
#include <map>
#include "BTreeSeach.h"
#include "Algorithm.h"
using namespace std;

typedef CMap<CString, CString, CString, CString> imsiphone;
extern	imsiphone	v_imsiphone;

// class  IndexSearch//预警数据产生时需要返回此关键字阀值
// {
// public:
//     BYTE d[MAXCHARS];
//     int m;
//     char patt[MAXCHARS];
// 	int m_quanzhi;
//     IndexSearch(const char*,int);
//     IndexSearch();
// 	void InitSearch(const char *,int quanzhi);
// 	
//     int find(const char*);
// 	int find(const char*,int len);
// };


//*****************************************************************************
//	地址类型
//	固定IP			8
//	MAC				9
//	URL				14
//	上网帐号		16
//	电话号码		17
//	关键字			18
//	技侦关键字		19	2006-3-14 add
//	IP+URL			34
//*****************************************************************************
typedef struct STRUCT_LIMITDATA
{
	LPCTSTR		caseid;			// 案件ID
	LPCTSTR		objtype;		// 对象类型
	LPCTSTR		objname;		// 对象名称
	LPCTSTR		limit;			// 地址数据
	LPCTSTR		limitid;		// 地址数据id
	LPCTSTR		nocappro;		// 不截获协议
	LPCTSTR		webalarmpro;	// 页面告警协议
	LPCTSTR		handaarmpro;	// 手机告警协议
	long		ltype;			// 地址类型
	BOOL		ifexpand;		// 扩展标志
	DWORD		AlarmState;		// 告警的状态 &2案件告警，&4手机告警，&8实时跟踪
}LIMITDATA, *LPLIMITDATA;

class  CaseInfo
{
public:	
	CaseInfo();
	CaseInfo(CString& filter);
	CaseInfo(LPLIMITDATA plimit);
	virtual ~CaseInfo();
	InitSearch(LPCTSTR strKey);
public:
	CString CaseID;			// 案件代号
	CString ObjectID;		// 对象ID
	CString LimitAddr;		// 案件地址
	CString LimitId;		// 线索id
	int		LimitType;		// 地址类型
	DWORD	AlarmState;		// 告警的状态 &2案件界面告警，&4案件手机告警，&8实时跟踪

	CString nocappro;		//不截获协议
	CString webalarmpro;	//页面告警协议
	CString handalarmpro;	//手机告警协议
// 	int		Keynum;			// 包含的关键字个数
// 	Search	*SearchKey[2];	// 关键字
};

typedef CRbArray<CaseInfo, CaseInfo&> CaseInfoArray;		// 案件信息数组

//********************** 预警线索双缓冲 **********************************//

// typedef list <IndexSearch, allocator <IndexSearch> > Warnkeylist;
// 
// //用来销毁关键字链表，有可能多条规则指向一个关键字类，不能从规则销毁关键字类
// typedef  struct _DestoryKeyList_
// {
// 	Warnkeylist		*m_warnkeylist;//存放此关键字类下边的所有关键字
// 	_DestoryKeyList_		*befor;//一个帐号对应的下一个规则
// 	_DestoryKeyList_		*last;//指向此帐号对应规则最后一个单元
// 
// 	_DestoryKeyList_()
// 	{
// 		m_warnkeylist = NULL;
// 		befor = NULL;
// 		last = NULL;
// 	};
// 	
// 	~_DestoryKeyList_()
// 	{
// 		int mm;
// 		mm = 0;
// 		if (m_warnkeylist != NULL)
// 		{
// 			delete m_warnkeylist;
// 			m_warnkeylist = NULL;
// 		}
// 	};
// 	
// }DestoryKeyList,*LpDestoryKeyList;

//extern  LpDestoryKeyList G_destorykeylist;

//重点人员帐号hash
/*typedef  struct _warnlimit_
{
//	Warnkeylist		*m_warnkeylist;//存放此关键字类下边的所有关键字
	CString			keytypename;//关键字类名称
	CString			ruleid; //规则
	CString			classtype; //分类
	CString			level;//非帐号类重点人员不比对关键字时的告警级别（只产生预警告警）
	CString			peopletype;
	CString			name;
	CString			dummytype;  //虚拟身份类型
	CString			imname;
	CString			card;
	BOOL			IsCompKey;//是否比较关键字
	BOOL			Ispeople;//是否是人员
	BOOL			IsKuoxian;//是否扩线
//	IndexSearch		SearchNoAccount;//需要搜索的重点人员的非帐号
	CString			extendkey;//扩线的帐号
	_warnlimit_		*next;//一个帐号对应的下一个规则
	_warnlimit_		*last;//指向此帐号对应规则最后一个单元
	_warnlimit_()
	{
		m_warnkeylist = NULL;
		next = NULL;
		IsCompKey = FALSE;
		Ispeople = FALSE;
		IsKuoxian = FALSE;
	};

	~_warnlimit_()
	{
//		if (m_warnkeylist != NULL)
//		{
//			delete m_warnkeylist;
//			m_warnkeylist = NULL;
//		}
	};

}Warnlimit,*Lpwarnlimit;*/

//预警关键字和重点人员关键字类型线索类
typedef  struct _warnkeyinfo_
{
// 	CString			m_id;				//重点人员的id  ccic写入身份证号
// 	CString			m_firstclass;		//重点人员的大类
// 
// 	CString			m_secclass;			//重点人员的子类
// 	CString			m_key;				//字典中的关键字
	char			m_id[22];				//重点人员的id  ccic写入身份证号
	char			m_firstclass[20];		//重点人员的大类

	char			m_secclass[20];			//重点人员的子类
	char			m_key[20];				//字典中的关键字
	char*			m_ccicbuf;
	int				m_keyquanzhi;			//关键字的权值
	BOOL			m_isccic;//是否ccic
 	DWORD			m_num;
// 	CString			m_name;			//ccic的姓名
// 	CString			m_nickname;			//ccic的昵称
// 	CString			m_date;			//ccic的出生年月
// 	char			m_name[20];			//ccic的姓名
// 	char			m_nickname[20];			//ccic的昵称
// 	char			m_date[10];			//ccic的出生年月
//	CString			m_ccic;
// 	CString			m_ruleid;			// 此重点人员对上的所有规则
// 	CString			m_fenlei;			// 此重点人员对上的所有分类
// 	CString			m_level;			// 对应此规则的预警级别//非帐号类重点人员不比对关键字时的告警级别（只产生预警告警）
// 	CString			m_card;				// 身份证
// 	CString			m_name;				// 姓名
// 	CString			m_peopletype;		// 重点人员类
// 	CString			m_dummytype;		// 虚拟身份类型
// 	CString			m_keytype;			// 关键字类
// 	CString			m_secclass;			// 第二层关键字类
// 	CString			m_thrclass;         // 第三层关键字类
// 	CString			m_IsOnlyAlarm;		// 是否是只告警不记数据
// 	CString			m_State;			// 状态
// 	CString			m_spyaddr;			// 截获地点
// 	CString			m_imname;			// 重点人员值
// 	CString			m_tm;				// 时间，数据的时间  2007-07-16 14:22:20
// 
// 	CString			m_key;				//字典中的关键字
// 	CString			m_keytypename;		//关键字类名称
// 
// 	DWORD			m_num;
// 
// 	BOOL			m_havesearchinpeople;	//已经匹配上重点人员
// 	BOOL			m_IsCompKey;			//是否比较关键字
// 	BOOL			m_Ispeople;				//是否是人员
// 	BOOL			m_IsKuoxian;			//是否扩线
// 	CString			m_extendkey;			//扩线的帐号
// 	int				m_keyquanzhi;			//关键字的权值
// 	BOOL			m_IsAddup;				//关键字权值是否累加
	_warnkeyinfo_	*m_next;				//一个帐号对应的下一个规则
	_warnkeyinfo_	*m_last;				//指向此帐号对应规则最后一个单元

	_warnkeyinfo_()
	{
//		m_havesearchinpeople = FALSE;
		m_keyquanzhi = 0;
		m_next = NULL;
		m_isccic = 0;
		m_ccicbuf=NULL;
		memset(m_id,0,22);				//重点人员的id  ccic写入身份证号
		memset(m_firstclass,0,20);		//重点人员的大类

		memset(m_secclass,0,20);			//重点人员的子类
		memset(m_key,0,10);				//字典中的关键字
		m_keyquanzhi=0;
		m_last=this;
// 		m_IsCompKey = FALSE;
// 		m_Ispeople = FALSE;
// 		m_IsKuoxian = FALSE;
	};
	
	~_warnkeyinfo_()
	{
		if (m_next != NULL)
		{
			delete m_next;
		}
		m_ccicbuf=NULL;
// 		if (m_ccicbuf!=NULL)
// 		{
// 			delete[] m_ccicbuf;
// 		}
	};
	
}warnkeyinfo,*Lpwarnkeyinfo;

//预警关键字和重点人员关键字类型线索类
typedef  struct _ccicinfo_
{
	char			m_id[22];				//重点人员的id  ccic写入身份证号
	char*			m_ccicbuf;
	BOOL			m_isccic;//是否ccic
// 	DWORD			m_num;

	_ccicinfo_()
	{
		m_isccic = 0;
		m_ccicbuf=NULL;
		memset(m_id,0,22);				//重点人员的id  ccic写入身份证号
	};
	
	~_ccicinfo_()
	{
		m_ccicbuf=NULL;
	};
	
}ccicinfo,*Lpccicinfo;

typedef CRbArray<warnkeyinfo, warnkeyinfo&> WarnkeyInfoArray;		// 预警信息数组
typedef	CMap<CString, LPCTSTR, Lpwarnkeyinfo, Lpwarnkeyinfo&> WarnlimitMap;
typedef	CMap<CString, LPCTSTR, Lpccicinfo, Lpccicinfo&> ccicMap;

class SocketSession;

typedef struct _matchcallpara_  
{
	CaseInfoArray*	m_casearr;
	WarnlimitMap*	m_warnlimitmap;
	SocketSession* m_socket;

	BOOL	caseflg;
	BOOL	warnflg;
	BOOL	IsRubbish;
	DWORD	state;
}matchcallpara,*lpmatchcallpara;


//typedef list <Warnlimit, allocator <Warnlimit> > PeopleList;

struct Struct_fz
{
	int		red;
	int		yellow;
	int		orange;
};



/*class  WarnInfo
{
public:	
	WarnInfo()
	{
		
	}
	~WarnInfo(){}

public:
	CStringArray	ruleidarr;				// 此重点人员对上的所有规则
	CStringArray	fenleiarr;				// 此重点人员对上的所有分类
	CStringArray	levelarr;				//对应此规则的预警级别
	CStringArray	cardarr;				//身份证
	CStringArray	namearr;				//姓名
	CStringArray	peopletypearr;			//重点人员类
	CStringArray	peoplearr;				//重点人员
	CStringArray	keytypearr;				//关键字类
	CStringArray	IsOnlyAlarmArr;			//是否是只告警不记数据
	CStringArray	StateArr;			    //状态
	CStringArray	numArr;					//总权值
//	CStringArray    spyaddrArr;				//截获地点
	CStringArray    dummytypeArr;			//虚拟身份类型

};*/
/*typedef struct _dicwarninfo_
{
	CString	ruleid;				// 此重点人员对上的所有规则
	CString	fenlei;				// 此重点人员对上的所有分类
	CString	level;				// 对应此规则的预警级别
	CString	card;				// 身份证
	CString	name;				// 姓名
	CString	peopletype;			// 重点人员类
	CString	people;				// 重点人员
	CString	dummytype;			// 虚拟身份类型
	CString	keytype;			// 关键字类
	CString	IsOnlyAlarm;		// 是否是只告警不记数据
	CString	State;			    // 状态
	CString spyaddr;			// 截获地点
	DWORD	num;
	_dicwarninfo_()
	{
		num = 0;
	}
}dicwarninfo,*Lpdicwarninfo;*/

//typedef	CMap<CString, LPCTSTR, Lpdicwarninfo, Lpdicwarninfo&> WarnInfoMap;

//extern  CMapStringToString EmphsePeopleExtendMap;
extern  CMapStringToString EmphsePeopleExtendMap;

//  int SearchIsWarnData(const char* account, const char* sip,const char* mac,const char* ptext, int tlen, 
// 								  WarnInfo& warninfo,const char*csip, CString strpro);

//  int SearchByFileIsWarnData(CString filename,const char* account, const char* sip,const char* mac, WarnInfo& warninfo,const char*csip,CString strpro);

// int PeopleOnOrOff(const char* account, const char* sip,CString state, WarnInfo& warninfo);

 BOOL AddExtenIp(const char * buf,char *key);//处理收到的扩线线索
 BOOL AddExtenIp(const char * buf,int len);
 BOOL DelExtenIp(const char * buf);

//BOOL GetTypeArrFromMap(WarnInfoMap *EmphsePeopleMap,WarnInfo* warninfo);
BOOL GetTypeArrFromMap(WarnlimitMap *EmphsePeopleMap,WarnkeyInfoArray* warnkeyinfoarray);

//*************************************关于keybtree。h内容放到此处，解决类间结构不能使用问题

#define MAXBTREEARRAY 255
class BaseAllDicInfo
{
public:
	BOOL	m_isccic;
	char*			m_ccicbuf;
public:
	BaseAllDicInfo()
	{
		m_isccic=FALSE;
		m_ccicbuf=NULL;
	}
	~BaseAllDicInfo()
	{
// 		if (m_ccicbuf!=NULL)
// 		{
// 			delete m_ccicbuf;
// 		}
	}
};

class AllDicInfo : public BaseAllDicInfo//根handle一样的参数
{
public:
	CaseInfoArray m_caseinfoarry;//案件信息
	Lpwarnkeyinfo m_warninfo;//预警信息
//	char*			m_ccicbuf;

	AllDicInfo()
	{

		m_warninfo = NULL;
// 		caseinfo = new CaseInfo;
// 		warninfo = new warnkeyinfo;
	}
	~AllDicInfo()
	{
		m_caseinfoarry.RemoveAll();
//		warnkeyinfoarray.RemoveAll();
// 		if (caseinfo != NULL)
// 		{
// 			delete caseinfo;
// 			caseinfo = NULL;
// 		}

		if(m_warninfo != NULL)
		{
			delete m_warninfo;
			m_warninfo = NULL;
		}
	}
};
typedef	CMap<CString, LPCTSTR, void*, void*&> AllDicMap;


// 通用字段类
// class Vocable
// {
// public:	
// 	CString vocable;
// 	INT Vhandle;	//词Handle
// 	AllDicInfo* alldicinfo;
// 	 Vocable(LPLIMITDATA plimit);
// 	Vocable ()
// 	{
// 		alldicinfo = NULL;
// 		caseinfo = new CaseInfo;
// 	}
// 	~Vocable()
// 	{
// 	}
// public:
// 	BOOL  inilimit(LPLIMITDATA plimit);
// };
// 
// inline BOOL AFXAPI operator< (const Vocable & s1, const Vocable & s2)
// {
// 	if(s1.vocable.GetLength()==s2.vocable.GetLength())
//  		return s1.vocable<s2.vocable;
// 	else
// 	{
// 		return s1.vocable.GetLength() < s2.vocable.GetLength();
// 	}
// }
// 
// typedef list<Vocable, allocator<Vocable> > VocableList;
// typedef VocableList::iterator VocableListIt ;
// 
// typedef CMap<CString, LPCTSTR, CString, CString&> VocableMap;

/*class KeyBTree
{
public:
	INT Vhandle;	//词Handle
	AllDicInfo* alldicinfo;
	
	USHORT intmod;  //模数
	UCHAR cvalue;	//值
	UCHAR deep;		//深度
	KeyBTree *nextarray;	//KeyBTree数组指针
public:
	KeyBTree * seek(UCHAR bytes);//
	KeyBTree * Place(UCHAR bytes);//

	KeyBTree ()
	{
		
		alldicinfo = NULL;
	}
	~KeyBTree()
	{
		if (alldicinfo != NULL)
		{
			delete alldicinfo;
			alldicinfo = NULL;
		}
	}
};


class ModTree{
public:
	UCHAR Src[MAXBTREEARRAY];//源串
	UCHAR res[MAXBTREEARRAY];//余数
	int srccount;
	int rescount;
	ModTree()
	{
		memset(Src,0,MAXBTREEARRAY);
		memset(res,0xff,MAXBTREEARRAY);
		srccount=0;
		rescount=0;
	}
	~ModTree()
	{
	}
	VOID AddChar(UCHAR ad);
	int GetMinMod();
};


class MemDicMap
{
public:
	int memsize;
	int structoffset;
	VocableList vocablelist;
	KeyBTree * KeyBTreeArray;
public:
	MemDicMap()
	{
		KeyBTreeArray=0;
	}
	~MemDicMap()
	{
		if(KeyBTreeArray)
			delete[]KeyBTreeArray;
		KeyBTreeArray=0;
	}
	BOOL AddDicStr(KeyBTree * pdicbtree,UCHAR * str,UCHAR *mod,int len,int handle);
	BOOL SetDicStrHandle(KeyBTree * pdicbtree,UCHAR * str,int len,int handle,AllDicInfo* alldicinfo);
	KeyBTree * PlaceStr(CString str);
	BOOL RecAddVocable(CString basestr,Vocable * beginit,Vocable * endit);

	BOOL OptiMod(KeyBTree * basetree,ModTree & modtree);
	BOOL IniArrayMem(int size);
	BOOL IniVocable();

	BOOL LoadTextFile(CString filename);
	BOOL LoadDicFile(CString filename);
	
	BOOL RecSaveDic(CString basestr,KeyBTree *basetree,UCHAR * modarray,int len,FILE * fs);
	BOOL SaveDicFile(CString filename,UINT UnitSize);
	BOOL  ClearData();//清空当前所有缓冲区
	

};
*/
//////////////////////////////////////////////////////////////////////////
// 
//extern INT			v_CNARRAY[65536];
//extern MemDicMap	v_GobDicMap;

//////////////////////////////////////////////////////////////////////////
// 
BOOL CaseWarnMatchingKey(VOID* pUserData,VOID* lpdata);
BOOL CcicMatchingKey(VOID* pUserData,VOID* lpdata);

BOOL HttpfiltMatchingKey(VOID* pUserData,VOID* lpdata); 

//typedef void ( *pMatchingKey)(DWORD handle) ;

// int MatchingDic(MemDicMap * mppp,const unsigned char *buf, int lenbuf,WarnInfoMap *EmphsePeopleMap,int state,
// 				const char *sip);

//void MatchingDic(MemDicMap * mppp,const unsigned char *buf, int lenbuf);
//BOOL MatchingDicAndHaveReturn(MemDicMap * mppp,const unsigned char *buf, int lenbuf, CaseInfo &caseinfo,BOOL IsCase = FALSE);
//BOOL  IniDic(MemDicMap* tempm);

//*****************
//extern  Lpwarnlimit g_warnlimit;
class  WarnlimitPara
{
public:
	WarnlimitPara()
	{
		m_readflg=0;
		m_warnlimitMap[0].InitHashTable(1997, FALSE);
		m_warnlimitMap[1].InitHashTable(1997, FALSE);
 		m_warnIpMap[0].InitHashTable(1997, FALSE);
 		m_warnIpMap[1].InitHashTable(1997, FALSE);
 		m_warnMacMap[0].InitHashTable(1997, FALSE);
 		m_warnMacMap[1].InitHashTable(1997, FALSE);
//		m_kuoxianmap[0].InitHashTable(1997, FALSE);
//		m_kuoxianmap[1].InitHashTable(1997, FALSE);
	
		m_kuoxianpeople[0].InitHashTable(1997, FALSE);
		m_kuoxianpeople[1].InitHashTable(1997, FALSE);

		m_lpalldicinfo[0]=NULL;
		m_lpalldicinfo[1]=NULL;
		m_mempool[0]=NULL;
		m_mempool[1]=NULL;
	}
	~WarnlimitPara(){};
	
public:
	WarnlimitMap		m_warnlimitMap[2];	// 重点人员帐号
	WarnlimitMap		m_warnIpMap[2];		// 重点人员ip
	WarnlimitMap		m_warnMacMap[2];	// 重点人员mac
//	WarnlimitMap		m_kuoxianmap[2];	// 重点人员关键字类型 ip为key  
											// 规则id + 重点人员关键字类型线索为值
	CMapStringToString	m_warnruleMap[2];

	
	Struct_fz			g_warnlimit[2];		// 告警阀值
	
//	MemDicMap			m_allwarnkeydic[2];
//	MemDicMap			m_peoplekeydic[2];
//	MemDicMap           m_warnkeydic[2];
	WarnlimitMap  m_kuoxianpeople[2];   //规则id + 重点人员关键字类型线索

	BaseAllDicInfo* m_lpalldicinfo[2];
	char* m_mempool[2];

	long	m_readflg;
};
extern  WarnlimitPara v_warnlimitPara;
extern  char* g_mempool[2];



//********************** 案件IP MAC 哈希表定义 **********************************//
typedef	CMap<CString, LPCTSTR, CaseInfoArray*, CaseInfoArray*&> CaseIpMacMap;
inline void AFXAPI DestructElements(CaseInfo** ppElements, int nCount)
{
	delete *ppElements;
}

typedef CMap<DWORD, DWORD&, CaseInfoArray*, CaseInfoArray*&> CaseVlanMap;

//********************** 案件关键字链表定义 **********************************//
typedef list <CaseInfo, allocator <CaseInfo> > CaseKeyWordList;

//********************** 案件IP+URL哈希表定义 **********************************//
typedef	CMap<CString, LPCTSTR, CString*, CString*&> CaseIpUrlMap;
inline void AFXAPI DestructElements(CaseKeyWordList** ppElements, int nCount)
{
	delete *ppElements;
}

//********************** caseuser哈希表定义 **********************************//
//typedef	CMap<CString, LPCTSTR, CStringArray*, CStringArray*&> CaseUserMap;

//********************** 案件线索双缓冲定义 **********************************//

class  CaseLimitPara
{
public:
	CaseLimitPara()
	{
		m_readflg=0;
		m_ipmap[0].InitHashTable(1997, FALSE);
		m_ipmap[1].InitHashTable(1997, FALSE);
		m_macmap[0].InitHashTable(1997, FALSE);
		m_macmap[1].InitHashTable(1997, FALSE);
		m_ipportmap[0].InitHashTable(1997, FALSE);
		m_ipportmap[1].InitHashTable(1997, FALSE);
// 		m_vlanidMap[0].InitHashTable(1997, FALSE);
// 		m_vlanidMap[1].InitHashTable(1997, FALSE);
	}

	~CaseLimitPara(){}
	
public:
	long				m_readflg;
	CaseIpMacMap		m_ipmap[2];				// IP
	CaseIpMacMap		m_macmap[2];			// MAC
	CaseIpMacMap		m_phoneaccountmap[2];	// 电话帐号
	CaseIpMacMap		m_ipportmap[2];			// ip+port
	CaseIpMacMap		m_emailmap[2];			// email
	CaseIpMacMap		m_immap[2];			// 即时消息

//	CaseVlanMap			m_vlanidMap[2];			// VLanID

	CMapStringToString	m_ridreallimit[2];	// 判断实施跟踪扩展之后的原线索是否还起效
	CaseKeyWordList		m_urllist[2];		// URL列表
	BTreeSeach			m_keyworddic[2];
};

// 地址参数表
extern  CaseLimitPara v_CaseLimitPara;

class ccicpara
{
public:
	ccicpara()
	{
		m_readflg=0;
		m_filelen=0;
		m_lpalldicinfo[0]=NULL;
		m_lpalldicinfo[1]=NULL;
		m_mempool[0]=NULL;
		m_mempool[1]=NULL;
		m_ccicmap[0].InitHashTable(517619);
		m_ccicmap[1].InitHashTable(517619);
	}

	~ccicpara(){}
public:
	int					m_filelen;
	long				m_readflg;
//	BTreeSeach			m_keyworddic[2];
	ccicMap		m_ccicmap[2];

	BaseAllDicInfo* m_lpalldicinfo[2];
	char* m_mempool[2];

};
extern  ccicpara v_ccicpara;

//********************** 预警关键字链表定义 **********************************//
typedef list <Search, allocator <Search> > IndexKeyWordList;

//********************** 预警IP+URL哈希表定义 **********************************//
typedef	CMap<CString, LPCTSTR, IndexKeyWordList*, IndexKeyWordList*&> IndexIpUrlMap;
inline void AFXAPI DestructElements(IndexKeyWordList** ppElements, int nCount)
{
	delete *ppElements;
}

//********************** 预警线索双缓冲定义 **********************************//
class  IndexLimitPara
{
public:
	IndexLimitPara()
	{
		m_keywordreadflg=m_limitreadflg=0;
		m_ipurlmap[0].InitHashTable(1997, FALSE);
		m_ipurlmap[1].InitHashTable(1997, FALSE);
		m_ipportmap[0].InitHashTable(1997, FALSE);
		m_ipportmap[1].InitHashTable(1997, FALSE);
	}
	~IndexLimitPara(){};
	
//	INDEXLPLIMITBUFFER	LockBuffer(int& bufid);
//	void	UnLockBuffer(int bufid, BOOL bCanRead = TRUE)
//	{
//		ASSERT(bufid == 2 || bufid == 1);
//		if (bCanRead && (bufid==1 || bufid==2))
//			InterlockedExchange(&m_readflg, bufid);
//	}
//	INDEXLPLIMITBUFFER	GetBuffer()
//	{
//		ASSERT ((m_readflg == 1) || (m_readflg == 2));
//		return (m_readflg==1)? &m_buffer1:&m_buffer2;
//	}
public:
	long				m_keywordreadflg;
	long				m_limitreadflg;
	IndexIpUrlMap		m_ipurlmap[2];		// 预警IP+URL
	CMapStringToString	m_ipportmap[2];		// 预警IP+port

	IndexKeyWordList	m_keywordlist[2];	// 预警关键字
	BTreeSeach			m_keyworddic[2];
	
public:
	BOOL	UpdateRule(CString bcp);
	BOOL	CheckRule(CString sip, CString dip, WORD sport, WORD dport);

private:
	long	m_fullruleid;
	long	m_fullrulenum[2];
	map<CString,int>	m_mapfullrule[2];		/// 规则->结果, 结果=0,部分匹配 结果=1,已匹配
public:
	BYTE				m_port[2][65536];	// 预警端口
};

// 地址参数表
extern  IndexLimitPara v_IndexLimitPara;

//********************** 屏蔽信息双缓冲定义 **********************************//
class  RubbishPara
{
public:
	RubbishPara()
	{
		m_readflg=0;
		m_rubbipmap[0].InitHashTable(1997, FALSE);
		m_rubbipmap[1].InitHashTable(1997, FALSE);
		m_rubbaaamap[0].InitHashTable(1997, FALSE);
		m_rubbaaamap[1].InitHashTable(1997, FALSE);
	}
	~RubbishPara(){};
	
public:
	CMapStringToPtr		m_rubbipmap[2];			// IP屏蔽信息表
	CMapStringToPtr		m_rubbaaamap[2];		// 认证屏蔽信息表
	IndexKeyWordList	m_rubburllist[2];		// URL屏蔽信息表
	IndexKeyWordList	m_rubbmaillist[2];		// 邮件屏蔽信息表
	long	m_readflg;
};

// 地址参数表
extern  RubbishPara v_RubbishPara;

extern  CMapStringToString v_localmac;//本机mac  不保存

//*********************  IDC网站类型关键字  **********************************//
struct IDCHttpKey
{
	int		num;
	CString servertype;
	Search	keyword1;
	Search	keyword2;
};
typedef list <IDCHttpKey, allocator <IDCHttpKey> > IDCHttpKeyWordList;

extern  long	v_IDCHttpKeyflag;
extern  IDCHttpKeyWordList v_IDCHttpKeyLilt[2];

//*********************  IDC网站类型端口表  **********************************//
class  IDCSvrPort
{
public:
	IDCSvrPort()
	{
		readflg = 0;
		memset(tcpporttype, 0, sizeof(tcpporttype));
		memset(udpporttype, 0, sizeof(udpporttype));
		typetext[0].SetSize(1, 32);
		typetext[1].SetSize(1, 32);
	}
	~IDCSvrPort(){}

	void AddPort(DWORD port, BOOL btcp, CString type);
	CString GetSvrType(WORD port, BOOL btcp)
	{
		if (btcp)
			return typetext[readflg].ElementAt(tcpporttype[readflg][port]);
		else
			return typetext[readflg].ElementAt(udpporttype[readflg][port]);
	}
	
	void Clear(long bufid)
	{
		ASSERT(bufid==1 || bufid==0);
		ASSERT(bufid != readflg);
		typetext[bufid].SetSize(1, 32);
		memset(tcpporttype[bufid], 0, sizeof(tcpporttype)/2);
		memset(tcpporttype[bufid], 0, sizeof(tcpporttype)/2);
	}
public:

	long   readflg;
protected:
	int tcpporttype[2][65536];
	int udpporttype[2][65536];
	CStringArray typetext[2];
};

extern  IDCSvrPort v_IDCSvrPort;

//**********************  QQ密码双缓冲定义  **********************************//
typedef struct QQPWD
{
	QQPWD*	pNext;
//	char	password[16];
	char	pwd2md5[16];
	char	pwdqqnomd5[16];
	char longpwd[16];
}QQPWD;

typedef	CMap<DWORD, DWORD, QQPWD*, QQPWD*&> OicqPasswrdMap;

class  OicqPwdPara
{
public:
	OicqPwdPara()
	{
		m_pFreeList = NULL;
		m_readflg = 0;
		m_qqpwdmap[0].InitHashTable(2813353);
		m_qqpwdmap[1].InitHashTable(2813353);
	}
	~OicqPwdPara(){}
	long	m_readflg;
	OicqPasswrdMap m_qqpwdmap[2];

	QQPWD*	m_pFreeList;

	QQPWD* GetNewUnit();
	void ReleaseUnit(QQPWD* pQQPwd);
};

extern  OicqPwdPara v_OicqPassWord;
inline void AFXAPI DestructElements(QQPWD** ppElements, int nCount)
{
	v_OicqPassWord.ReleaseUnit(*ppElements);
}
BOOL SearchOicqPwd(DWORD OicqNum, QQPWD*& pQQPwdList);

//**********************  taobaowangwang密码双缓冲定义  **********************************//
typedef struct TAOBAOPWD
{
	TAOBAOPWD*	pNext;
	char	password[20];
	char	pwd2md5[16];
	CString longpwd;
}TAOBAOPWD;

typedef	CMap<CString, CString, TAOBAOPWD*, TAOBAOPWD*&> TaobaoPasswrdMap;

class  TaobaoPwdPara
{
public:
	TaobaoPwdPara()
	{
		m_pFreeList = NULL;
		m_readflg = 0;
		m_taobaopwdmap[0].InitHashTable(50051);
		m_taobaopwdmap[1].InitHashTable(50051);
	}
	~TaobaoPwdPara(){}
	long	m_readflg;
	TaobaoPasswrdMap m_taobaopwdmap[2];

	TAOBAOPWD*	m_pFreeList;

	TAOBAOPWD* GetNewUnit();
	void ReleaseUnit(TAOBAOPWD* pTAOBAOPwd);
};

extern  TaobaoPwdPara v_TaobaoPassWord;

inline void AFXAPI DestructElements(TAOBAOPWD** ppElements, int nCount)
{
	v_TaobaoPassWord.ReleaseUnit(*ppElements);
}
BOOL SearchTaobaoPwd(CString TaobaoUser, TAOBAOPWD*& pTAOBAOPwdList);


//////////////////////////////////////////////////////////////////////////
//
//	Post 拆解, 用户自定义, 双缓冲
//	定制的Url登陆信息匹配类
class CustomUrlField
{
public:
	int InfoType;	// 1登陆, 2注册，腾讯QQ登录
	int FieldType;	// 1ID, 2密码, 3用户名
	CString strhttpname;
	CString struserid;
	CString strpassword;
	CString stremail;
	CString strqq;
	CString straddres;
	CString strmobile;
	CString strcardnum;
	CString strcardtype;

	Search	srhDomain;
	CString strdomain;

	CustomUrlField(
		int infotype, CString httpname,CString domain,CString userid, CString password,
		CString email="",CString qq="",CString addres="",CString mobile="",
		CString strcardnum="",CString cardtype="")
	{
		strhttpname=httpname;
		InfoType = infotype;
		strdomain=domain;
		struserid=userid;
		strpassword=password;
		stremail=email;
		strqq=qq;
		straddres=addres;
		strmobile=mobile;
		strcardnum=strcardnum;
		strcardtype=cardtype;
	}
	~CustomUrlField()
	{
		
	}
};
typedef list<CustomUrlField, allocator<CustomUrlField> > CustomUrlFieldList;
typedef CustomUrlFieldList::iterator CustomUrlFieldListIt ;
extern  CustomUrlFieldList	v_CustUrlList[2];
extern  long				v_CustUrlAccID;
extern  CustomUrlFieldList	v_HttpregistList[2];
extern  long				v_HttpregistAccID;

typedef struct sindexlimit
{
	CString m_port;
	int typeinfo;
	CString no;
}sindexlimit;

typedef	CMap<CString, CString, sindexlimit, sindexlimit&> IndexMap;


extern	IndexMap		  v_Indexlimtmap[2];
extern  long				v_IdexlimitID;

//********************** 微博全局结构定义 **********************************//
typedef struct weibostruct
{
	CString			m_islogin;		//是否登录
	CString			m_oid;			//访问页ID
	CString			m_onick;		//访问页昵称
	CString			m_mid;			//会员账号
	CString			m_uid;			//微博发布人UID
	CString			m_nick;			//昵称
	CString			m_netmark;		//主页
	CString			m_sex;			//性别
	CString			m_lang;			//使用语言
	CString			m_clientmsg;	//客户端信息
	CString			m_msgtype;		//信息类型
	CString			m_msg;			//信息内容
	CString			m_tm;			//最后一次更新时间
}weibostruct;

typedef CMap<CString, CString, weibostruct, weibostruct&> WeiboMap;
extern	WeiboMap	v_WeiboMap;

//********************** 手机QQ全局结构定义 **********************************//
struct QQINFO 
{
	byte	key[0x10];
	time_t	lasttime;
};
typedef CMap<CString, CString, QQINFO, QQINFO&> MobileqqMap;
extern	MobileqqMap	g_mobileqqMap;

typedef CMap<CString, CString, CString, CString> MobileqqStateMap;
extern	MobileqqStateMap	g_mobileqqstateMap;

//********************** 手机QQ微博结构定义 **********************************//
typedef struct mobileqqweibostruct
{
	CString			username;		//登陆账号
	CString			m_tm;			//最后一次更新时间
}mobileqqweibostruct;

typedef CMap<CString, CString, mobileqqweibostruct, mobileqqweibostruct&> mobileqqWeiboMap;
extern	mobileqqWeiboMap	v_mobileqqWeiboMap;

//********************** momo结构定义 **********************************//
typedef CMap<CString, CString, CString, CString> MomoMap;
extern	MomoMap	v_MomoMap;

//********************** kuaidi结构定义 **********************************//
typedef CMap<CString, CString, CString, CString> KuaidiMap;
extern	KuaidiMap	v_kuaidi;

typedef CMap<CString, CString, CString, CString> DiDiMap;
extern	DiDiMap	v_DiDiMap;

//********************** ssh结构定义 **********************************//
typedef CMap<CString, CString, CString, CString> sshMap;
extern	sshMap	v_sshMap;

typedef struct
{
	unsigned int qq;
	unsigned char password[16];
	unsigned char pwdmd5[16];
	unsigned char conststr[16];
	unsigned char unuse[16];
}qqpwd;



// typedef CMap<unsigned int, unsigned int, unsigned int, unsigned int> MobileqqKeyMap;
// MobileqqKeyMap	g_mobileqqkeymap;

typedef CMap<unsigned int, unsigned int,qqpwd *,  qqpwd*> MobileqqpwdkeyMap;
extern MobileqqpwdkeyMap	g_mobileqqpwdkeymap;

typedef CMap<unsigned int,unsigned int,unsigned int,unsigned int>MobileqqKeyMap;
extern MobileqqKeyMap g_mobileqqkeymap;



//********************** 案件匹配类函数定义 **********************************//
// AuthInfo *GetAAAInfo(DWORD ip1, DWORD ip2 ,AAAInfo & aaainfo, time_t nowtm);

 BOOL SearchIndexByIPPORT(CString sipport, CString dipport);
 BOOL SearchIndexByIPURL(const char* ip1, const char* ip2, const char* strurl, int urllen/*, CaseInfo& caseinfo*/);

 BOOL SearchCaseByURL(const char* ptext, int tlen, CaseInfo& caseinfo);
 int  SearchCaseByIP(const char* strsip, const char* strdip, CaseInfoArray& caseinfoarr, CMapStringToString *m_ridcasemap = NULL);
 int  SearchCaseByMAC(const char* strsmac, const char* strdmac, CaseInfoArray& caseinfoarr, CMapStringToString *m_ridcasemap = NULL);
 BOOL SearchCaseByKey(const char* ptext, int tlen, CaseInfo& caseinfo, BOOL isjz=FALSE);
 BOOL SearchCaseFileByKey(const char* filename,  CaseInfo& caseinfo, BOOL isjz=FALSE);
 BOOL SearchCaseByIPURL(const char* ip1, const char* ip2, const char* strurl, int urllen, CaseInfo& caseinfo);
 BOOL SearchCaseByAAA(LPCTSTR phone, LPCTSTR account, CaseInfoArray& caseinfoarr, CMapStringToString *m_ridcasemap);
 BOOL SearchCaseByVlan(WORD vlanid, CaseInfo& caseinfo);
 int SearchCaseByIPPORT(CString sipport, CString dipport, CaseInfoArray& caseinfoarr);

 BOOL SearchCaseEmailIm(SocketSession* s, LPCTSTR userid, int tlen, DWORD state);

int  SearchCaseByMail(const char* userid,
					CaseInfoArray& caseinfoarr);
int  SearchCaseByIm(const char* userid,
					CaseInfoArray& caseinfoarr);

 BOOL SearchHttpFilter(const char* filename,const char* sip,const char* dip,const char* url,const char* host,CString pro,CStringArray* arrfile/*, CaseInfo& caseinfo*/);
// BOOL SearchHttpFilter(const char* str, int strlen);

 BOOL SearchBruuishByAAA(const char* strphone, const char* struser);
 BOOL SearchBurrishByURL(CString strurl);
 BOOL SearchBurrishByMail(CString strmail);
 BOOL SearchBurrishByIP(const char* ip1, const char* ip2);
//  BOOL SearchUnipPort(WORD port1, WORD port2, int packettype);

 int  SearchRealIP(LPCTSTR sip, LPCTSTR dip, CaseInfoArray& caseinfoarr);
 BOOL AddRealIP(LPCTSTR rip, CString caseid, CString limit, int ltype);
 CString SearchIDCHttpKey(const char* filename);
 CString GetCdmaAccount(const BYTE* buf);


//案件ipmacaccout，及案件预警关键字匹配
 BOOL SearchCaseOrWarn(SocketSession* s, LPCTSTR text, int tlen, DWORD &state,CString subpro="");
BOOL SearchLocalmac(CString smac,CString dmac);
CString getruletype(CString subpro,Lpwarnkeyinfo lpwarnkeyinfo,int &classno);
BOOL CheckId(char* buf,int buflen,WarnlimitMap *EmphsePeopleMap);










#endif
