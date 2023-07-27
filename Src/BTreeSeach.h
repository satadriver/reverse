/***************************************************************
Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
    BTreeSeach   �汾:  1.0   ? ����: 2007-7-9 
	����:	2007-7-9 
	�ļ���: 	BTreeSeach
	����:		����
	��Ҫ����
***************************************************************/ 
#if !defined(AFX_BTREESEACH_H__A1FF89CA_5162_41D7_B3AA_21E0E7D79AE3__INCLUDED_)
#define AFX_BTREESEACH_H__A1FF89CA_5162_41D7_B3AA_21E0E7D79AE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <list>
#include "io.h"
using namespace std;

#define MAXBTREEARRAY 255 //B���������
typedef BOOL ( *pMatchingKey)(LPVOID pUserData1, LPVOID pUserData2);

//ͨ�ôʵ�ĵ�����
class Vocable
{
public:	
	CString m_Vocable;		//�ֵ�Ĵ�
	VOID *  m_pUserData;	//�û��Զ���ṹָ��

	Vocable ()
	{
		m_pUserData=0;
	}

	~Vocable()
	{
	}
};

//����VocableList������ص�����
inline BOOL AFXAPI operator< (const Vocable & s1, const Vocable & s2)
{
 	return s1.m_Vocable<s2.m_Vocable;
}

typedef list<Vocable, allocator<Vocable> > VocableList;
typedef VocableList::iterator VocableListIt ;

//�ʵ��B���ڵ���
class KeyBTree
{
public:
	VOID * m_pUserData;	//�û��Զ���ṹָ��
	USHORT m_Mod;		//ģ��
	UCHAR m_cValue;		//ֵ
	UCHAR m_Deep;		//���
	KeyBTree *m_NextArray;	//KeyBTree����ָ��

public:
	KeyBTree * Seek(UCHAR bytes); //m_NextArray����Ĳ���һ���ֽ�
	KeyBTree * Place(UCHAR bytes);//��m_NextArraypan��һ���ֽ�

	KeyBTree ()
	{
		m_NextArray=NULL;
	}

	~KeyBTree()
	{
	}
};

//�Ż�����������
class ModTree
{
public:
	UCHAR m_Src[MAXBTREEARRAY];//Դ��
	UCHAR m_Res[MAXBTREEARRAY];//����
	int m_SrcCount;			//Դ���ڵļ���
	int m_ResCount;			//�����ļ���

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

//�ʵ��ά�� ������
class BTreeSeach
{
public:
	int m_MemSize;			//��ʼ��m_KeyBTreeArray�Ĵ�С
	int m_StructOffset;		//��ǰ������ƫ��
	VocableList m_VocableList;	//�ʵ��б�
	KeyBTree * m_KeyBTreeArray; //B���ĸ��ڵ�

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
	BOOL IniKeyBTree(int size);										//��������ɺ��ʼ��BTree
	BOOL ClearData();										//���m_KeyBTreeArray �ʹ��б�
	//����������ı��������ֵ�ƥ�䡣����3�ǻص���4�ǵ��÷���Ҫ�洢���ݵ�ָ��
	int  MatchingDic(const unsigned char *buf, int lenbuf, pMatchingKey mathmothod, VOID* lpdata);

private:
	//�ݹ鴦������õĴʵ��б�
	BOOL RecAddVocable(CString basestr,Vocable * beginit,Vocable * endit);
	BOOL OptiMod(KeyBTree * basetree,ModTree & modtree);	//����ȡģ�������İ취�Ż�
	BOOL IniArrayMem(int size);								//��ʼ��m_KeyBTreeArray�Ĵ�С	
	BOOL IniVocable();										/***��ʼ���ʵ�,
															m_KeyBTreeArray�ṹ����**/
	//��һ���ʸ����û������ݽṹָ��
	BOOL SetDicStrHandle(KeyBTree * pdicbtree,UCHAR * str,int len,VOID *  m_pUserData);
	KeyBTree * PlaceStr(CString str);		//���ݲ����������Ľڵ�,

};


#endif // !defined(AFX_BTREESEACH_H__A1FF89CA_5162_41D7_B3AA_21E0E7D79AE3__INCLUDED_)
