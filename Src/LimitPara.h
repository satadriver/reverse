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

// class  IndexSearch//Ԥ�����ݲ���ʱ��Ҫ���ش˹ؼ��ַ�ֵ
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
//	��ַ����
//	�̶�IP			8
//	MAC				9
//	URL				14
//	�����ʺ�		16
//	�绰����		17
//	�ؼ���			18
//	����ؼ���		19	2006-3-14 add
//	IP+URL			34
//*****************************************************************************
typedef struct STRUCT_LIMITDATA
{
	LPCTSTR		caseid;			// ����ID
	LPCTSTR		objtype;		// ��������
	LPCTSTR		objname;		// ��������
	LPCTSTR		limit;			// ��ַ����
	LPCTSTR		limitid;		// ��ַ����id
	LPCTSTR		nocappro;		// ���ػ�Э��
	LPCTSTR		webalarmpro;	// ҳ��澯Э��
	LPCTSTR		handaarmpro;	// �ֻ��澯Э��
	long		ltype;			// ��ַ����
	BOOL		ifexpand;		// ��չ��־
	DWORD		AlarmState;		// �澯��״̬ &2�����澯��&4�ֻ��澯��&8ʵʱ����
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
	CString CaseID;			// ��������
	CString ObjectID;		// ����ID
	CString LimitAddr;		// ������ַ
	CString LimitId;		// ����id
	int		LimitType;		// ��ַ����
	DWORD	AlarmState;		// �澯��״̬ &2��������澯��&4�����ֻ��澯��&8ʵʱ����

	CString nocappro;		//���ػ�Э��
	CString webalarmpro;	//ҳ��澯Э��
	CString handalarmpro;	//�ֻ��澯Э��
// 	int		Keynum;			// �����Ĺؼ��ָ���
// 	Search	*SearchKey[2];	// �ؼ���
};

typedef CRbArray<CaseInfo, CaseInfo&> CaseInfoArray;		// ������Ϣ����

//********************** Ԥ������˫���� **********************************//

// typedef list <IndexSearch, allocator <IndexSearch> > Warnkeylist;
// 
// //�������ٹؼ��������п��ܶ�������ָ��һ���ؼ����࣬���ܴӹ������ٹؼ�����
// typedef  struct _DestoryKeyList_
// {
// 	Warnkeylist		*m_warnkeylist;//��Ŵ˹ؼ������±ߵ����йؼ���
// 	_DestoryKeyList_		*befor;//һ���ʺŶ�Ӧ����һ������
// 	_DestoryKeyList_		*last;//ָ����ʺŶ�Ӧ�������һ����Ԫ
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

//�ص���Ա�ʺ�hash
/*typedef  struct _warnlimit_
{
//	Warnkeylist		*m_warnkeylist;//��Ŵ˹ؼ������±ߵ����йؼ���
	CString			keytypename;//�ؼ���������
	CString			ruleid; //����
	CString			classtype; //����
	CString			level;//���ʺ����ص���Ա���ȶԹؼ���ʱ�ĸ澯����ֻ����Ԥ���澯��
	CString			peopletype;
	CString			name;
	CString			dummytype;  //�����������
	CString			imname;
	CString			card;
	BOOL			IsCompKey;//�Ƿ�ȽϹؼ���
	BOOL			Ispeople;//�Ƿ�����Ա
	BOOL			IsKuoxian;//�Ƿ�����
//	IndexSearch		SearchNoAccount;//��Ҫ�������ص���Ա�ķ��ʺ�
	CString			extendkey;//���ߵ��ʺ�
	_warnlimit_		*next;//һ���ʺŶ�Ӧ����һ������
	_warnlimit_		*last;//ָ����ʺŶ�Ӧ�������һ����Ԫ
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

//Ԥ���ؼ��ֺ��ص���Ա�ؼ�������������
typedef  struct _warnkeyinfo_
{
// 	CString			m_id;				//�ص���Ա��id  ccicд�����֤��
// 	CString			m_firstclass;		//�ص���Ա�Ĵ���
// 
// 	CString			m_secclass;			//�ص���Ա������
// 	CString			m_key;				//�ֵ��еĹؼ���
	char			m_id[22];				//�ص���Ա��id  ccicд�����֤��
	char			m_firstclass[20];		//�ص���Ա�Ĵ���

	char			m_secclass[20];			//�ص���Ա������
	char			m_key[20];				//�ֵ��еĹؼ���
	char*			m_ccicbuf;
	int				m_keyquanzhi;			//�ؼ��ֵ�Ȩֵ
	BOOL			m_isccic;//�Ƿ�ccic
 	DWORD			m_num;
// 	CString			m_name;			//ccic������
// 	CString			m_nickname;			//ccic���ǳ�
// 	CString			m_date;			//ccic�ĳ�������
// 	char			m_name[20];			//ccic������
// 	char			m_nickname[20];			//ccic���ǳ�
// 	char			m_date[10];			//ccic�ĳ�������
//	CString			m_ccic;
// 	CString			m_ruleid;			// ���ص���Ա���ϵ����й���
// 	CString			m_fenlei;			// ���ص���Ա���ϵ����з���
// 	CString			m_level;			// ��Ӧ�˹����Ԥ������//���ʺ����ص���Ա���ȶԹؼ���ʱ�ĸ澯����ֻ����Ԥ���澯��
// 	CString			m_card;				// ���֤
// 	CString			m_name;				// ����
// 	CString			m_peopletype;		// �ص���Ա��
// 	CString			m_dummytype;		// �����������
// 	CString			m_keytype;			// �ؼ�����
// 	CString			m_secclass;			// �ڶ���ؼ�����
// 	CString			m_thrclass;         // ������ؼ�����
// 	CString			m_IsOnlyAlarm;		// �Ƿ���ֻ�澯��������
// 	CString			m_State;			// ״̬
// 	CString			m_spyaddr;			// �ػ�ص�
// 	CString			m_imname;			// �ص���Աֵ
// 	CString			m_tm;				// ʱ�䣬���ݵ�ʱ��  2007-07-16 14:22:20
// 
// 	CString			m_key;				//�ֵ��еĹؼ���
// 	CString			m_keytypename;		//�ؼ���������
// 
// 	DWORD			m_num;
// 
// 	BOOL			m_havesearchinpeople;	//�Ѿ�ƥ�����ص���Ա
// 	BOOL			m_IsCompKey;			//�Ƿ�ȽϹؼ���
// 	BOOL			m_Ispeople;				//�Ƿ�����Ա
// 	BOOL			m_IsKuoxian;			//�Ƿ�����
// 	CString			m_extendkey;			//���ߵ��ʺ�
// 	int				m_keyquanzhi;			//�ؼ��ֵ�Ȩֵ
// 	BOOL			m_IsAddup;				//�ؼ���Ȩֵ�Ƿ��ۼ�
	_warnkeyinfo_	*m_next;				//һ���ʺŶ�Ӧ����һ������
	_warnkeyinfo_	*m_last;				//ָ����ʺŶ�Ӧ�������һ����Ԫ

	_warnkeyinfo_()
	{
//		m_havesearchinpeople = FALSE;
		m_keyquanzhi = 0;
		m_next = NULL;
		m_isccic = 0;
		m_ccicbuf=NULL;
		memset(m_id,0,22);				//�ص���Ա��id  ccicд�����֤��
		memset(m_firstclass,0,20);		//�ص���Ա�Ĵ���

		memset(m_secclass,0,20);			//�ص���Ա������
		memset(m_key,0,10);				//�ֵ��еĹؼ���
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

//Ԥ���ؼ��ֺ��ص���Ա�ؼ�������������
typedef  struct _ccicinfo_
{
	char			m_id[22];				//�ص���Ա��id  ccicд�����֤��
	char*			m_ccicbuf;
	BOOL			m_isccic;//�Ƿ�ccic
// 	DWORD			m_num;

	_ccicinfo_()
	{
		m_isccic = 0;
		m_ccicbuf=NULL;
		memset(m_id,0,22);				//�ص���Ա��id  ccicд�����֤��
	};
	
	~_ccicinfo_()
	{
		m_ccicbuf=NULL;
	};
	
}ccicinfo,*Lpccicinfo;

typedef CRbArray<warnkeyinfo, warnkeyinfo&> WarnkeyInfoArray;		// Ԥ����Ϣ����
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
	CStringArray	ruleidarr;				// ���ص���Ա���ϵ����й���
	CStringArray	fenleiarr;				// ���ص���Ա���ϵ����з���
	CStringArray	levelarr;				//��Ӧ�˹����Ԥ������
	CStringArray	cardarr;				//���֤
	CStringArray	namearr;				//����
	CStringArray	peopletypearr;			//�ص���Ա��
	CStringArray	peoplearr;				//�ص���Ա
	CStringArray	keytypearr;				//�ؼ�����
	CStringArray	IsOnlyAlarmArr;			//�Ƿ���ֻ�澯��������
	CStringArray	StateArr;			    //״̬
	CStringArray	numArr;					//��Ȩֵ
//	CStringArray    spyaddrArr;				//�ػ�ص�
	CStringArray    dummytypeArr;			//�����������

};*/
/*typedef struct _dicwarninfo_
{
	CString	ruleid;				// ���ص���Ա���ϵ����й���
	CString	fenlei;				// ���ص���Ա���ϵ����з���
	CString	level;				// ��Ӧ�˹����Ԥ������
	CString	card;				// ���֤
	CString	name;				// ����
	CString	peopletype;			// �ص���Ա��
	CString	people;				// �ص���Ա
	CString	dummytype;			// �����������
	CString	keytype;			// �ؼ�����
	CString	IsOnlyAlarm;		// �Ƿ���ֻ�澯��������
	CString	State;			    // ״̬
	CString spyaddr;			// �ػ�ص�
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

 BOOL AddExtenIp(const char * buf,char *key);//�����յ�����������
 BOOL AddExtenIp(const char * buf,int len);
 BOOL DelExtenIp(const char * buf);

//BOOL GetTypeArrFromMap(WarnInfoMap *EmphsePeopleMap,WarnInfo* warninfo);
BOOL GetTypeArrFromMap(WarnlimitMap *EmphsePeopleMap,WarnkeyInfoArray* warnkeyinfoarray);

//*************************************����keybtree��h���ݷŵ��˴���������ṹ����ʹ������

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

class AllDicInfo : public BaseAllDicInfo//��handleһ���Ĳ���
{
public:
	CaseInfoArray m_caseinfoarry;//������Ϣ
	Lpwarnkeyinfo m_warninfo;//Ԥ����Ϣ
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


// ͨ���ֶ���
// class Vocable
// {
// public:	
// 	CString vocable;
// 	INT Vhandle;	//��Handle
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
	INT Vhandle;	//��Handle
	AllDicInfo* alldicinfo;
	
	USHORT intmod;  //ģ��
	UCHAR cvalue;	//ֵ
	UCHAR deep;		//���
	KeyBTree *nextarray;	//KeyBTree����ָ��
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
	UCHAR Src[MAXBTREEARRAY];//Դ��
	UCHAR res[MAXBTREEARRAY];//����
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
	BOOL  ClearData();//��յ�ǰ���л�����
	

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
	WarnlimitMap		m_warnlimitMap[2];	// �ص���Ա�ʺ�
	WarnlimitMap		m_warnIpMap[2];		// �ص���Աip
	WarnlimitMap		m_warnMacMap[2];	// �ص���Աmac
//	WarnlimitMap		m_kuoxianmap[2];	// �ص���Ա�ؼ������� ipΪkey  
											// ����id + �ص���Ա�ؼ�����������Ϊֵ
	CMapStringToString	m_warnruleMap[2];

	
	Struct_fz			g_warnlimit[2];		// �澯��ֵ
	
//	MemDicMap			m_allwarnkeydic[2];
//	MemDicMap			m_peoplekeydic[2];
//	MemDicMap           m_warnkeydic[2];
	WarnlimitMap  m_kuoxianpeople[2];   //����id + �ص���Ա�ؼ�����������

	BaseAllDicInfo* m_lpalldicinfo[2];
	char* m_mempool[2];

	long	m_readflg;
};
extern  WarnlimitPara v_warnlimitPara;
extern  char* g_mempool[2];



//********************** ����IP MAC ��ϣ���� **********************************//
typedef	CMap<CString, LPCTSTR, CaseInfoArray*, CaseInfoArray*&> CaseIpMacMap;
inline void AFXAPI DestructElements(CaseInfo** ppElements, int nCount)
{
	delete *ppElements;
}

typedef CMap<DWORD, DWORD&, CaseInfoArray*, CaseInfoArray*&> CaseVlanMap;

//********************** �����ؼ��������� **********************************//
typedef list <CaseInfo, allocator <CaseInfo> > CaseKeyWordList;

//********************** ����IP+URL��ϣ���� **********************************//
typedef	CMap<CString, LPCTSTR, CString*, CString*&> CaseIpUrlMap;
inline void AFXAPI DestructElements(CaseKeyWordList** ppElements, int nCount)
{
	delete *ppElements;
}

//********************** caseuser��ϣ���� **********************************//
//typedef	CMap<CString, LPCTSTR, CStringArray*, CStringArray*&> CaseUserMap;

//********************** ��������˫���嶨�� **********************************//

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
	CaseIpMacMap		m_phoneaccountmap[2];	// �绰�ʺ�
	CaseIpMacMap		m_ipportmap[2];			// ip+port
	CaseIpMacMap		m_emailmap[2];			// email
	CaseIpMacMap		m_immap[2];			// ��ʱ��Ϣ

//	CaseVlanMap			m_vlanidMap[2];			// VLanID

	CMapStringToString	m_ridreallimit[2];	// �ж�ʵʩ������չ֮���ԭ�����Ƿ���Ч
	CaseKeyWordList		m_urllist[2];		// URL�б�
	BTreeSeach			m_keyworddic[2];
};

// ��ַ������
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

//********************** Ԥ���ؼ��������� **********************************//
typedef list <Search, allocator <Search> > IndexKeyWordList;

//********************** Ԥ��IP+URL��ϣ���� **********************************//
typedef	CMap<CString, LPCTSTR, IndexKeyWordList*, IndexKeyWordList*&> IndexIpUrlMap;
inline void AFXAPI DestructElements(IndexKeyWordList** ppElements, int nCount)
{
	delete *ppElements;
}

//********************** Ԥ������˫���嶨�� **********************************//
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
	IndexIpUrlMap		m_ipurlmap[2];		// Ԥ��IP+URL
	CMapStringToString	m_ipportmap[2];		// Ԥ��IP+port

	IndexKeyWordList	m_keywordlist[2];	// Ԥ���ؼ���
	BTreeSeach			m_keyworddic[2];
	
public:
	BOOL	UpdateRule(CString bcp);
	BOOL	CheckRule(CString sip, CString dip, WORD sport, WORD dport);

private:
	long	m_fullruleid;
	long	m_fullrulenum[2];
	map<CString,int>	m_mapfullrule[2];		/// ����->���, ���=0,����ƥ�� ���=1,��ƥ��
public:
	BYTE				m_port[2][65536];	// Ԥ���˿�
};

// ��ַ������
extern  IndexLimitPara v_IndexLimitPara;

//********************** ������Ϣ˫���嶨�� **********************************//
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
	CMapStringToPtr		m_rubbipmap[2];			// IP������Ϣ��
	CMapStringToPtr		m_rubbaaamap[2];		// ��֤������Ϣ��
	IndexKeyWordList	m_rubburllist[2];		// URL������Ϣ��
	IndexKeyWordList	m_rubbmaillist[2];		// �ʼ�������Ϣ��
	long	m_readflg;
};

// ��ַ������
extern  RubbishPara v_RubbishPara;

extern  CMapStringToString v_localmac;//����mac  ������

//*********************  IDC��վ���͹ؼ���  **********************************//
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

//*********************  IDC��վ���Ͷ˿ڱ�  **********************************//
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

//**********************  QQ����˫���嶨��  **********************************//
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

//**********************  taobaowangwang����˫���嶨��  **********************************//
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
//	Post ���, �û��Զ���, ˫����
//	���Ƶ�Url��½��Ϣƥ����
class CustomUrlField
{
public:
	int InfoType;	// 1��½, 2ע�ᣬ��ѶQQ��¼
	int FieldType;	// 1ID, 2����, 3�û���
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

//********************** ΢��ȫ�ֽṹ���� **********************************//
typedef struct weibostruct
{
	CString			m_islogin;		//�Ƿ��¼
	CString			m_oid;			//����ҳID
	CString			m_onick;		//����ҳ�ǳ�
	CString			m_mid;			//��Ա�˺�
	CString			m_uid;			//΢��������UID
	CString			m_nick;			//�ǳ�
	CString			m_netmark;		//��ҳ
	CString			m_sex;			//�Ա�
	CString			m_lang;			//ʹ������
	CString			m_clientmsg;	//�ͻ�����Ϣ
	CString			m_msgtype;		//��Ϣ����
	CString			m_msg;			//��Ϣ����
	CString			m_tm;			//���һ�θ���ʱ��
}weibostruct;

typedef CMap<CString, CString, weibostruct, weibostruct&> WeiboMap;
extern	WeiboMap	v_WeiboMap;

//********************** �ֻ�QQȫ�ֽṹ���� **********************************//
struct QQINFO 
{
	byte	key[0x10];
	time_t	lasttime;
};
typedef CMap<CString, CString, QQINFO, QQINFO&> MobileqqMap;
extern	MobileqqMap	g_mobileqqMap;

typedef CMap<CString, CString, CString, CString> MobileqqStateMap;
extern	MobileqqStateMap	g_mobileqqstateMap;

//********************** �ֻ�QQ΢���ṹ���� **********************************//
typedef struct mobileqqweibostruct
{
	CString			username;		//��½�˺�
	CString			m_tm;			//���һ�θ���ʱ��
}mobileqqweibostruct;

typedef CMap<CString, CString, mobileqqweibostruct, mobileqqweibostruct&> mobileqqWeiboMap;
extern	mobileqqWeiboMap	v_mobileqqWeiboMap;

//********************** momo�ṹ���� **********************************//
typedef CMap<CString, CString, CString, CString> MomoMap;
extern	MomoMap	v_MomoMap;

//********************** kuaidi�ṹ���� **********************************//
typedef CMap<CString, CString, CString, CString> KuaidiMap;
extern	KuaidiMap	v_kuaidi;

typedef CMap<CString, CString, CString, CString> DiDiMap;
extern	DiDiMap	v_DiDiMap;

//********************** ssh�ṹ���� **********************************//
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



//********************** ����ƥ���ຯ������ **********************************//
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


//����ipmacaccout��������Ԥ���ؼ���ƥ��
 BOOL SearchCaseOrWarn(SocketSession* s, LPCTSTR text, int tlen, DWORD &state,CString subpro="");
BOOL SearchLocalmac(CString smac,CString dmac);
CString getruletype(CString subpro,Lpwarnkeyinfo lpwarnkeyinfo,int &classno);
BOOL CheckId(char* buf,int buflen,WarnlimitMap *EmphsePeopleMap);










#endif
