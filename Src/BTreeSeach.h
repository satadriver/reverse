/***************************************************************
Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
    BTreeSeach   版本:  1.0   ? 日期: 2007-7-9 
	建立:	2007-7-9 
	文件名: 	BTreeSeach
	作者:		万晴
	主要功能
***************************************************************/ 
#if !defined(AFX_BTREESEACH_H__A1FF89CA_5162_41D7_B3AA_21E0E7D79AE3__INCLUDED_)
#define AFX_BTREESEACH_H__A1FF89CA_5162_41D7_B3AA_21E0E7D79AE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <list>
#include "io.h"
using namespace std;

#define MAXBTREEARRAY 255 //B树最大数组
typedef BOOL ( *pMatchingKey)(LPVOID pUserData1, LPVOID pUserData2);

//通用词典的单词类
class Vocable
{
public:	
	CString m_Vocable;		//字典的词
	VOID *  m_pUserData;	//用户自定义结构指针

	Vocable ()
	{
		m_pUserData=0;
	}

	~Vocable()
	{
	}
};

//重载VocableList的排序回调函数
inline BOOL AFXAPI operator< (const Vocable & s1, const Vocable & s2)
{
 	return s1.m_Vocable<s2.m_Vocable;
}

typedef list<Vocable, allocator<Vocable> > VocableList;
typedef VocableList::iterator VocableListIt ;

//词典的B树节点类
class KeyBTree
{
public:
	VOID * m_pUserData;	//用户自定义结构指针
	USHORT m_Mod;		//模数
	UCHAR m_cValue;		//值
	UCHAR m_Deep;		//深度
	KeyBTree *m_NextArray;	//KeyBTree数组指针

public:
	KeyBTree * Seek(UCHAR bytes); //m_NextArray在类的查找一个字节
	KeyBTree * Place(UCHAR bytes);//在m_NextArraypan放一个字节

	KeyBTree ()
	{
		m_NextArray=NULL;
	}

	~KeyBTree()
	{
	}
};

//优化的余数的类
class ModTree
{
public:
	UCHAR m_Src[MAXBTREEARRAY];//源串
	UCHAR m_Res[MAXBTREEARRAY];//余数
	int m_SrcCount;			//源串内的计数
	int m_ResCount;			//余数的计数

	ModTree()
	{
		memset(m_Src,0,MAXBTREEARRAY);
		memset(m_Res,0xff,MAXBTREEARRAY);
		m_SrcCount=0;
		m_ResCount=0;
	}
	~ModTree()
	{
	}

	VOID AddChar(UCHAR ad);
	int GetMinMod();
};

//词典的维护 搜索类
class BTreeSeach
{
public:
	int m_MemSize;			//初始化m_KeyBTreeArray的大小
	int m_StructOffset;		//当前的数组偏移
	VocableList m_VocableList;	//词的列表
	KeyBTree * m_KeyBTreeArray; //B树的根节点

public:
	BTreeSeach()
	{
		m_KeyBTreeArray=0;
	}
	~BTreeSeach()
	{
		if(m_KeyBTreeArray)
		{
			delete[]m_KeyBTreeArray;
		}
		m_KeyBTreeArray=0;
	}
	void AddVocable(CString str,VOID* m_pUserData);			//
	BOOL IniKeyBTree(int size);										//词增加完成后初始化BTree
	BOOL ClearData();										//清除m_KeyBTreeArray 和词列表
	//传入需分析文本，进行字典匹配。参数3是回调，4是调用方需要存储数据的指针
	int  MatchingDic(const unsigned char *buf, int lenbuf, pMatchingKey mathmothod, VOID* lpdata);

private:
	//递归处理排序好的词典列表
	BOOL RecAddVocable(CString basestr,Vocable * beginit,Vocable * endit);
	BOOL OptiMod(KeyBTree * basetree,ModTree & modtree);	//按照取模求余数的办法优化
	BOOL IniArrayMem(int size);								//初始化m_KeyBTreeArray的大小	
	BOOL IniVocable();										/***初始化词典,
															m_KeyBTreeArray结构处理**/
	//给一个词赋予用户的数据结构指针
	BOOL SetDicStrHandle(KeyBTree * pdicbtree,UCHAR * str,int len,VOID *  m_pUserData);
	KeyBTree * PlaceStr(CString str);		//根据参数返回树的节点,

};


#endif // !defined(AFX_BTREESEACH_H__A1FF89CA_5162_41D7_B3AA_21E0E7D79AE3__INCLUDED_)
