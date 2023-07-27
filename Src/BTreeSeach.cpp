/************************************************************
	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
	BTreeSeach   �汾:  1.0   ? ����: 2007-7-9 
	����:	2007-7-9 
	�ļ���: BTreeSeach.cpp
	����:   ����      
	ģ������:     // ģ������
	��Ҫ����:   

				BTreeSeach::AddVocable ���Ӵ�
				BTreeSeach::IniDic		��ʼ��B��
				BTreeSeach::MatchingDic	ƥ��ʵ䲢ִ�лص�����
				BTreeSeach::ClearData() ����ʵ���б�
				BTreeSeach::SetDicStrHandle �ݹ���ڵ㸳ֵ�û�����ָ��
				BTreeSeach::RecAddVocable �ݹ���ڵ������Ӵ�
***********************************************************/
#include "stdafx.h"
#include "dcepub.h"
#include "BTreeSeach.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*************************************************
  ��������:       KeyBTree::Place
  ����:          //��һ���ֽڵ��ڵ���ӽڵ�����,����������λ�ýڵ�
  �������:       bytes  �ֽ�
  �������:         
  ����ֵ:         ����������λ�ýڵ�
*************************************************/
KeyBTree * KeyBTree::Place(UCHAR bytes)
{
	KeyBTree *pTree = (KeyBTree *)this->m_NextArray;

	if(m_Mod>0)
	{
		if(pTree[bytes % m_Mod].m_cValue == 0)
		{
			pTree[bytes % m_Mod].m_cValue = bytes;
		}
		else
		{
			if(pTree[bytes % m_Mod].m_cValue != bytes)
				return NULL;
		}
		return &pTree[bytes% m_Mod];
	}
	else
	{
		pTree[bytes ].m_cValue = bytes;
	}

	return &(pTree[bytes]);
}

/*************************************************
  ��������:     KeyBTree::Seek
  ����:			����һ���ֽ����ӽڵ������λ��
  �������:     bytes  �ֽ�
  ����ֵ:       ����������λ�ýڵ�
  ����:         // ����˵��
*************************************************/
KeyBTree * KeyBTree::Seek(UCHAR bytes)
{
	KeyBTree *pTree = (KeyBTree *)this->m_NextArray;

	if(m_Mod>0)
	{
		if(pTree[bytes % m_Mod].m_cValue != bytes)
		{
			return NULL;
		}
		return &pTree[bytes % m_Mod];
	}
	else
	{
		return NULL;
	}
}


/*************************************************
  ��������:       ModTree::AddChar
  ����:            �Ż�ģ����� ����һ���ֽ�
  �������:        UCHAR ad
  ����:            ����˵��
*************************************************/
VOID ModTree::AddChar(UCHAR ad)
{
	this->m_Src[ad] = ad;
	m_SrcCount++;
}

/*************************************************
  ��������:       ModTree::GetMinMod
  ����:            ������Сģ��
  �������:        �������˵��������ÿ����������
                   �á�ȡֵ˵�����������ϵ��
  �������:        �����������˵����
  ����ֵ:          ��������ֵ��˵��
  ����:            ����˵��
*************************************************/
int ModTree::GetMinMod()
{
	m_ResCount = 0;
	for(int i = m_SrcCount ; i<MAXBTREEARRAY ; i++)
	{

		for(int k=1 ; k < MAXBTREEARRAY ; k++)
		{
			if(m_Src[k] > 0)
			{
				if(this->m_Res[k % i] == 0xff)
				{
					this->m_Res[k % i] = m_Src[k]; 
					m_ResCount++;
				}
				else
				{
					break;
				}
			}
		}
		if(m_SrcCount == m_ResCount)
		{
			return i;
		}

		memset(m_Res , 0xff , MAXBTREEARRAY);
		m_ResCount = 0;		
	}
	return 0;
}

/*************************************************
  ��������:       BTreeSeach::SetDicStrHandle
  ����:            �ݹ���ڵ㸳ֵ�û�����ָ��
  �������:        �ڵ� pkeybtree
                   �Ӵ� str
				   �Ӵ��� len
				   �û�����ָ��	pUserData
  ����ֵ:          BOOLֵ
  ����:            ����˵��
*************************************************/
BOOL BTreeSeach::SetDicStrHandle(KeyBTree * pkeybtree , UCHAR * str , 
								 int len , VOID * pUserData)
{
	KeyBTree * pTree = 0;
	int CurrDeep = pkeybtree->m_Deep;
	if(pkeybtree->m_Mod == 0)
	{
		return TRUE;	
	}

	//������Ϊ�±��ȡ�ڵ�
	pTree = &(pkeybtree->m_NextArray[str[0] % pkeybtree->m_Mod]);
	if(pTree->m_cValue == 0)
	{
		pTree->m_cValue = str[0];
	}
	else
	{
		if(pTree->m_cValue != str[0])
		{
			return FALSE;
		}
	}

	pTree->m_Deep = CurrDeep+1;

	//����len �ж��Ƿ�ݹ�
	if(len == 1)
	{
		pTree->m_pUserData = pUserData;
	}
	if(len > 1 )
	{
		return SetDicStrHandle(pTree , str+1 , len-1 , pUserData);
	}

	return TRUE;
}

/*************************************************
  ��������:       BTreeSeach::IniArrayMem
  ����:            ��ʼ��m_KeyBTreeArray�Ĵ�С
  �������:        size
*************************************************/
BOOL BTreeSeach::IniArrayMem(int size)
{
	this->m_KeyBTreeArray = new KeyBTree[size];
	this->m_StructOffset = 1;

	if(this->m_KeyBTreeArray)
	{
		this->m_MemSize = size;
	}
	memset(m_KeyBTreeArray , 0 , sizeof(KeyBTree)*size);
	return TRUE;
}

/*************************************************
  ��������:       BTreeSeach::PlaceStr
  ����:            ���һ����ڵ�
  �������:        �ַ��� str
  �������:        �����Ӵ�û�нڵ�� ����ڵ㡣
  ����ֵ:          ���ؽڵ�
  ����:            ����˵��
*************************************************/
KeyBTree * BTreeSeach::PlaceStr(CString str)
{

	KeyBTree *pTree = 0;
 	if(str.GetLength() == 0)
 	{
		//���ظ��ڵ�
		return m_KeyBTreeArray;
 	}

	if(str.GetLength() > 0)
	{
		pTree = m_KeyBTreeArray;
		//�ݹ���ҽڵ�
		for(int len = 0 ; len < str.GetLength() ; len++)
		{ 
			if(pTree->m_Mod == 0)
			{
				//����ڵ� ,Ҫ�ƶ����������±�ƫ��
				pTree->m_Mod = MAXBTREEARRAY;
				pTree->m_NextArray = this->m_KeyBTreeArray + this->m_StructOffset;
				this->m_StructOffset += MAXBTREEARRAY;
				memset((VOID*)pTree->m_NextArray , 0 , sizeof(KeyBTree)*MAXBTREEARRAY);
			}
			pTree = pTree->Place(*((UCHAR*)(LPCSTR)str + len));			
		}
	}
	return pTree;
}

/*************************************************
  ��������:       BTreeSeach::OptiMod
  ����:            ����ȡģ�������İ취�Ż�
  �������:        �ڵ� basetree
                   �����ڵ� modtree
  �������:        ���¶�basetree�Ż�,���������С��
  ����ֵ:          BOOL
  ����:            ����˵��
*************************************************/
BOOL BTreeSeach::OptiMod(KeyBTree * basetree,ModTree & modtree)
{
	int imod = modtree.GetMinMod();
	ASSERT(imod != 0);
	if(imod>0)
	{
		memset(basetree->m_NextArray,0,basetree->m_Mod*sizeof(KeyBTree));
		this->m_StructOffset -= MAXBTREEARRAY;
		this->m_StructOffset += imod;
		basetree->m_Mod = imod;
		for(int i = 0 ; i <MAXBTREEARRAY ; i++)
		{
			if(modtree.m_Src[i] > 0 && modtree.m_Src[i] < 0xff )
			{
				
				if(!basetree->Place(modtree.m_Src[i]))
				{	
					//�ظ�ֵ,�Ż�ʧ��
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
/*************************************************
  ��������:       BTreeSeach::RecAddVocable
  ����:            �ݹ���ڵ������Ӵ�
  �������:        �ַ��� basestr
					��ʼ�Ĵ� beginit
					�����Ĵ� endit

  �������:        �����������˵����
  ����ֵ:          ��������ֵ��˵��
  ����:            ����˵��
*************************************************/
BOOL BTreeSeach::RecAddVocable(CString basestr,Vocable * beginit,Vocable * endit)
{
	Vocable *tempit , *newstartlit , *preit;
	CString tempstr;
	KeyBTree  *pTree;
	INT len = basestr.GetLength();

	tempit = beginit;
	//�������� len+1���ַ���
	while(TRUE)
	{
		if(tempit->m_Vocable.GetLength() > len)
		{
			if(memcmp((LPCSTR)basestr , (LPCSTR)(tempit->m_Vocable) , (len)) == 0)
			{
				CString tempstr2 = tempit->m_Vocable.Left(len + 1);
				PlaceStr(tempstr2);
			}
		}

		if(tempit == endit)
		{
			break;
		}
		tempit++;
	}


	//�Ż�ģ��
	if(basestr.GetLength() >= 0)
	{
		pTree = PlaceStr(basestr);
		ModTree * ptemp = new ModTree ;
		for(int i=0 ; i<MAXBTREEARRAY ; i++)
		{
			if(pTree->m_NextArray[i].m_cValue > 0)
			{
				ptemp->AddChar(pTree->m_NextArray[i].m_cValue);
			}
		}
		OptiMod(pTree,*ptemp);
		delete ptemp;
	}

	//������֦�ݹ�
	tempit = beginit;
	newstartlit = tempit;
	preit = tempit;
	tempstr = "";
	while(TRUE)
	{
		if(tempit->m_Vocable.GetLength() > (len+1)
			&& tempit->m_Vocable.Left(len) == basestr)
		{
			if(tempstr.IsEmpty())
			{
				tempstr = tempit->m_Vocable.Left(len+1);
			}

			if(memcmp((LPCSTR)tempstr , (LPCSTR)(tempit->m_Vocable) , (len+1)) != 0 )
			{

				RecAddVocable(tempstr , newstartlit , preit);
				newstartlit = tempit;
				preit = tempit;
				tempstr = tempit->m_Vocable.Left(len+1);
				if(tempit == endit)
				{
					if( !tempstr.IsEmpty())
					{
						RecAddVocable(tempstr , newstartlit , preit);
					}
					break;
				}

			}
			else
			{
				preit = tempit;
			}
		} /*end of if(tempit->m_Vocable.GetLength()>(len+1)...*/

		if(tempit == endit )
		{
			if( !tempstr.IsEmpty())
			{
				RecAddVocable(tempstr , newstartlit , preit);
			}
			break;
		}
		tempit++;
	} /*end of while(TRUE)*/
	return TRUE;
}

/*************************************************
  ��������:       BTreeSeach::IniVocable
  ����:            ��ʼ���ʵ�,m_KeyBTreeArray�ṹ����
*************************************************/
BOOL BTreeSeach::IniVocable()
{
	VocableListIt beginit , endit , tempit , oldtemplit;
	KeyBTree * Oldtree = 0;
	CString basestr = "";
	int bytes = 0;
	int sum = 0;

//	this->m_VocableList.sort();
	beginit = this->m_VocableList.begin();
	endit = this->m_VocableList.end();
	tempit = beginit;

	Vocable *pvo = new Vocable[this->m_VocableList.size()];

	while(tempit != endit)
	{
		pvo[sum].m_Vocable = tempit->m_Vocable;
		pvo[sum].m_pUserData = tempit->m_pUserData;
		sum++;
		tempit++;
	}
	//�ݹ���ڵ������Ӵ�
	RecAddVocable(basestr , &pvo[0] , &(pvo[this->m_VocableList.size()-1]));
	delete []pvo;
	return TRUE;
}

/*************************************************
  ��������:       BTreeSeach::ClearData
  ����:            ���m_KeyBTreeArray �ʹ��б�
*************************************************/
BOOL BTreeSeach::ClearData()
{
	m_VocableList.clear();
	if(m_KeyBTreeArray)
	{
		delete[]m_KeyBTreeArray;
	}
	m_KeyBTreeArray = 0;
	m_MemSize = 0;
	m_StructOffset = 0;
	return TRUE;
}


/*************************************************
  ��������:       BTreeSeach::IniKeyBTree
  ����:            //�����ݷŵ����б����Ҫִ�д˹��̳�ʼ��
  �������:        �����������˵����
  ����ֵ:          BOOL
  ����:            �������ݷŵ����б��
*************************************************/
BOOL BTreeSeach::IniKeyBTree(int size)
{
	//���䵥Ԫ
	this->IniArrayMem(size);//200000*20
	//���ɴʵ�
	this->IniVocable();
	
	VocableListIt beginit , endit , tempit , oldtemplit;

//	this->m_VocableList.sort();
	beginit = this->m_VocableList.begin();
	endit = this->m_VocableList.end();
	tempit = beginit;

	int sum = 0;
	while(tempit != endit)
	{
		//���þ�� ���ظ���
		if(!this->SetDicStrHandle(this->m_KeyBTreeArray ,
					(UCHAR*)(LPCSTR)tempit->m_Vocable ,
					tempit->m_Vocable.GetLength() ,
					tempit->m_pUserData))
		{
			break;
		}
		sum++;
		tempit++;
	}
	return TRUE;
}

/*************************************************
  ��������:       BTreeSeach::MatchingDic
  ����:            ƥ��ʵ䲢ִ�лص�����
  �������:        buffer
                   lenbuf
				   �ص����� mathmothod
  ����ֵ:          ��������ֵ��˵��
  ����:            ����˵��
*************************************************/
int BTreeSeach::MatchingDic(const unsigned char *buf, int lenbuf, pMatchingKey mathmothod, VOID* lpdata)
{

	INT oldpos = 0;
	int bufPos = 0;

	while(bufPos < lenbuf)
	{

		oldpos = bufPos;
		KeyBTree * pTree = this->m_KeyBTreeArray;
		if (pTree == NULL)
		{
			return 0;
		}
		//�����ӽڵ�
		while(oldpos < lenbuf)
		{
				if(buf[oldpos] <= ' ')
				{
					break;
				}
				
				pTree = pTree->Seek((UCHAR)buf[oldpos]);
				if(pTree>0)
				{
					//�Ƿ�ƥ��
					if((INT)pTree->m_pUserData>0)
					{
						//ִ�лص�
						if(mathmothod)
						{
							if(!mathmothod(pTree->m_pUserData, lpdata))
							{
								return oldpos;
							}
						}
 					}
				}
				else
				{
					break;
				}
				oldpos ++;
		}/*end while(oldpos< lenbuf)*/

// 		if(buf[bufPos] > 0x80)
// 		{
// 			bufPos += 2;
// 		}
// 		else
		{
			bufPos ++;
		}

	}  /*end while(bufPos < lenbuf)*/
	return bufPos;
}

/*************************************************
  ��������:       BTreeSeach::AddVocable
  ����:            ���Ӵ�
  �������:        str
                   �û����ݽṹָ��
  �������:        �б���������
  ����:            ����˵��
*************************************************/
void BTreeSeach::AddVocable(CString str,VOID* m_pUserData)
{
	Vocable temp;
 	temp.m_Vocable = str;
 	temp.m_pUserData = m_pUserData;
 	this->m_VocableList.push_front(temp);
}