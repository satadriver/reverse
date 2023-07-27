#ifndef struct2345623562467
#define struct2345623562467
#include <afxtempl.h>
#include "list"
using namespace std;

//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//���Post field����Ϣ��
class PostFieldinfo
{
public:
	int typeinfo;
	int httpid;//�����Ӧ��id
	CString strhttpname;//��������
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
// 	// ������Ϣ������
// 	// ����ע��0xF000,   ���õ�½0xF00, 
// 	// �û�����ע��0xF0, �û����Ƶ�¼0xF
// 	// �û�����qq��¼0xF0000
// 	UINT	InfoType;
// 	// ������Ϣ�������, 1��ID/User, 2��Pass, 3��User, 4��ID/User/Pass
// 	UINT	FieldType;	
// 
// 	// �û���������, InfoType=0xF0��0xFʱ��Ч
// 	CString strDesc1;	
// 	CString strDesc2;	
// 	// ��ϵ�½��Ϣ��, FieldType=4ʱ��Ч
// 	CString strPassField;
// 	CString strPassValue;
	PostFieldinfo(CString strf,CString strv)
	{
// 		strf.MakeLower();
		strfield	= strf;		// ����
		strvalue	= strv;		// ������ֵ
		typeinfo  = 0;
// 		priority	= 0xFFFF;	// ���ȼ�
	}
	~PostFieldinfo()
	{
	}
};
//���� fieldtype������ ,����priority(����)������
// inline BOOL AFXAPI operator< (const PostFieldinfo& s1, const PostFieldinfo& s2)
// {
// 	if(s1.FieldType!=s2.FieldType)	// �����������û�
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
//ͨ���ֶ���
class GenMatchField
{
public:
	Search strField;
	CString strKey;
	UINT fieldtype;		//2 Usr  1Pass
	UINT priority;		//���ȼ�	��ֵС������
	UINT matchtype;		//ƥ������0 ���� 1 ģ�� 2 ��׺ 3 ǰ׺
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
//���� fieldtype������ ,����priority(����)������
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
#define OFFSET_SPLITURL		1	// ע��Url ����end
#define OFFSET_SPLITREG		2	// ע������ƫ��end
extern GenMatchFieldList	v_GenKeyList;
struct DefaultHttpCus
{
	LPCTSTR		paraname;	// ����/������
	UINT		ftype;		// ����, 1Pass, 2User, 3Register, 4RegItem
	UINT		pri;		// ���ȼ�
	UINT		mtype;		// ƥ��ģʽ
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