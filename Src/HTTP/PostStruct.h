#ifndef struct2345623562467
#define struct2345623562467
#include <afxtempl.h>
#include "list"
using namespace std;

//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//拆分Post field的信息类
class PostFieldinfo
{
public:
	int typeinfo;
	int httpid;//服务对应的id
	CString strhttpname;//服务名称
	CString strfield;
	CString strvalue;
// 	CString strqqinfo;
// 	CString strkey;
	
	CString struserid;
	CString strpassword;
	CString stremail;
	CString strqq;
	CString straddres;
	CString strmobile;
	CString strcardnum;
	CString strcardtype;

// 	UINT	priority;
// 	// 此条信息的类型
// 	// 内置注册0xF000,   内置登陆0xF00, 
// 	// 用户定制注册0xF0, 用户定制登录0xF
// 	// 用户定制qq登录0xF0000
// 	UINT	InfoType;
// 	// 此条信息入表类型, 1入ID/User, 2入Pass, 3入User, 4入ID/User/Pass
// 	UINT	FieldType;	
// 
// 	// 用户定制类用, InfoType=0xF0或0xF时有效
// 	CString strDesc1;	
// 	CString strDesc2;	
// 	// 组合登陆信息用, FieldType=4时有效
// 	CString strPassField;
// 	CString strPassValue;
	PostFieldinfo(CString strf,CString strv)
	{
// 		strf.MakeLower();
		strfield	= strf;		// 拆解符
		strvalue	= strv;		// 拆解出的值
		typeinfo  = 0;
// 		priority	= 0xFFFF;	// 优先级
	}
	~PostFieldinfo()
	{
	}
};
//按照 fieldtype逆排序 ,按照priority(优先)正排序
// inline BOOL AFXAPI operator< (const PostFieldinfo& s1, const PostFieldinfo& s2)
// {
// 	if(s1.FieldType!=s2.FieldType)	// 密码优先于用户
// 		return s1.FieldType>s2.FieldType;
// 	else
// 	{
// 		if(s1.priority==s2.priority)
// 		{
// 			return s1.strfield<s2.strfield;
// 		}
// 		else
// 			return s1.priority<s2.priority;
// 	}
// }


typedef list<PostFieldinfo, allocator<PostFieldinfo> > PostFieldinfoList;
typedef PostFieldinfoList::iterator PostFieldinfoListIt ;
//////////////////////////////////////////////////////////////////////////
//通用字段类
class GenMatchField
{
public:
	Search strField;
	CString strKey;
	UINT fieldtype;		//2 Usr  1Pass
	UINT priority;		//优先级	数值小了优先
	UINT matchtype;		//匹配类型0 等于 1 模糊 2 后缀 3 前缀
	UINT strlen;

	GenMatchField();
	GenMatchField(CString str,UINT ftype,UINT pri,UINT mtype)
	{
		strField.InitSearch(str);
		strKey=str;
		strlen=str.GetLength();
		fieldtype=ftype;
		priority=pri;
		matchtype=mtype;
	}
	GenMatchField(const GenMatchField& kkk)
	{
		strField.InitSearch(kkk.strKey);
		strKey=kkk.strKey;
		strlen=kkk.strlen;
		fieldtype=kkk.fieldtype;
		priority=kkk.priority;
		matchtype=kkk.matchtype;
	}
	~GenMatchField()
	{
	}
};
//按照 fieldtype正排序 ,按照priority(优先)正排序
inline BOOL AFXAPI operator< (const GenMatchField & s1, const GenMatchField & s2)
{
	if(s1.fieldtype!=s2.fieldtype)
		return s1.fieldtype<s2.fieldtype;
	else
		return s1.priority<s2.priority;
}


typedef list<GenMatchField, allocator<GenMatchField> > GenMatchFieldList;
typedef GenMatchFieldList::iterator GenMatchFieldListIt ;

//////////////////////////////////////////////////////////////////////////
#define OFFSET_SPLITURL		1	// 注册Url 特征end
#define OFFSET_SPLITREG		2	// 注册特征偏移end
extern GenMatchFieldList	v_GenKeyList;
struct DefaultHttpCus
{
	LPCTSTR		paraname;	// 拆解符/特征串
	UINT		ftype;		// 类型, 1Pass, 2User, 3Register, 4RegItem
	UINT		pri;		// 优先级
	UINT		mtype;		// 匹配模式
};
static DefaultHttpCus v_defaultHttpPos[] =  
{
// 	{"register",	3,1,1},
// 	{"regmail",		4,1,1},

// 	{"p",			1,0,0},
// 	{"password",	1,1,1},
// 	{"pass",		1,2,1},
// 	{"psw",			1,3,1},
// 	{"pwd",			1,4,1},
// 	{"pw",			1,5,0},
// 	{"TPL_password",1,6,1},
// 
// 	{"u",			2,0,0},
// 	{"username",	2,1,1},
// 	{"user_name",	2,2,1},
// 	{"account",		2,3,1},
// 	{"user",		2,4,1},
// 	{"usr",			2,5,1},
// 	{"name",		2,6,1},
// 	{"admin",		2,7,1},
// 	{"mail",		2,8,1},
// 	{"nick",		2,9,1},
// 	{"login",		2,10,1},
// 	{"logon",		2,11,1},
// 	{"id",			2,12,2},
// 	{"TPL_username",2,13,1},

	{0,				0,0,0}
};	

#endif