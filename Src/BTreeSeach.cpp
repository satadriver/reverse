/************************************************************
	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
	BTreeSeach   版本:  1.0   ? 日期: 2007-7-9 
	建立:	2007-7-9 
	文件名: BTreeSeach.cpp
	作者:   万晴      
	模块描述:     // 模块描述
	主要函数:   

				BTreeSeach::AddVocable 增加词
				BTreeSeach::IniDic		初始化B树
				BTreeSeach::MatchingDic	匹配词典并执行回调函数
				BTreeSeach::ClearData() 清除词典和列表
				BTreeSeach::SetDicStrHandle 递归向节点赋值用户数据指针
				BTreeSeach::RecAddVocable 递归向节点增加子串
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
  函数名称:       KeyBTree::Place
  描述:          //放一个字节到节点的子节点数组,并返回它的位置节点
  输入参数:       bytes  字节
  输出参数:         
  返回值:         并返回它的位置节点
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
  函数名称:     KeyBTree::Seek
  描述:			查找一个字节在子节点数组的位置
  输入参数:     bytes  字节
  返回值:       并返回它的位置节点
  其它:         // 其它说明
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
  函数名称:       ModTree::AddChar
  描述:            优化模数相关 增加一个字节
  输入参数:        UCHAR ad
  其它:            其它说明
*************************************************/
VOID ModTree::AddChar(UCHAR ad)
{
	this->m_Src[ad] = ad;
	m_SrcCount++;
}

/*************************************************
  函数名称:       ModTree::GetMinMod
  描述:            计算最小模数
  输入参数:        输入参数说明，包括每个参数的作
                   用、取值说明及参数间关系。
  输出参数:        对输出参数的说明。
  返回值:          函数返回值的说明
  其它:            其它说明
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
  函数名称:       BTreeSeach::SetDicStrHandle
  描述:            递归向节点赋值用户数据指针
  输入参数:        节点 pkeybtree
                   子串 str
				   子串长 len
				   用户数据指针	pUserData
  返回值:          BOOL值
  其它:            其它说明
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

	//以余数为下标获取节点
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

	//根据len 判断是否递归
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
  函数名称:       BTreeSeach::IniArrayMem
  描述:            初始化m_KeyBTreeArray的大小
  输入参数:        size
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
  函数名称:       BTreeSeach::PlaceStr
  描述:            查找或分配节点
  输入参数:        字符串 str
  输出参数:        根据子串没有节点的 分配节点。
  返回值:          返回节点
  其它:            其它说明
*************************************************/
KeyBTree * BTreeSeach::PlaceStr(CString str)
{

	KeyBTree *pTree = 0;
 	if(str.GetLength() == 0)
 	{
		//返回根节点
		return m_KeyBTreeArray;
 	}

	if(str.GetLength() > 0)
	{
		pTree = m_KeyBTreeArray;
		//递归查找节点
		for(int len = 0 ; len < str.GetLength() ; len++)
		{ 
			if(pTree->m_Mod == 0)
			{
				//分配节点 ,要移动数组的最大下标偏移
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
  函数名称:       BTreeSeach::OptiMod
  描述:            按照取模求余数的办法优化
  输入参数:        节点 basetree
                   余数节点 modtree
  输出参数:        重新对basetree优化,减少数组大小。
  返回值:          BOOL
  其它:            其它说明
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
					//重复值,优化失败
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
/*************************************************
  函数名称:       BTreeSeach::RecAddVocable
  描述:            递归向节点增加子串
  输入参数:        字符串 basestr
					开始的词 beginit
					结束的词 endit

  输出参数:        对输出参数的说明。
  返回值:          函数返回值的说明
  其它:            其它说明
*************************************************/
BOOL BTreeSeach::RecAddVocable(CString basestr,Vocable * beginit,Vocable * endit)
{
	Vocable *tempit , *newstartlit , *preit;
	CString tempstr;
	KeyBTree  *pTree;
	INT len = basestr.GetLength();

	tempit = beginit;
	//遍历所有 len+1的字符串
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


	//优化模数
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

	//遍历树枝递归
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
  函数名称:       BTreeSeach::IniVocable
  描述:            初始化词典,m_KeyBTreeArray结构处理
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
	//递归向节点增加子串
	RecAddVocable(basestr , &pvo[0] , &(pvo[this->m_VocableList.size()-1]));
	delete []pvo;
	return TRUE;
}

/*************************************************
  函数名称:       BTreeSeach::ClearData
  描述:            清除m_KeyBTreeArray 和词列表
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
  函数名称:       BTreeSeach::IniKeyBTree
  描述:            //把数据放到词列表后，需要执行此过程初始化
  输出参数:        对输出参数的说明。
  返回值:          BOOL
  其它:            必须数据放到词列表后
*************************************************/
BOOL BTreeSeach::IniKeyBTree(int size)
{
	//分配单元
	this->IniArrayMem(size);//200000*20
	//生成词典
	this->IniVocable();
	
	VocableListIt beginit , endit , tempit , oldtemplit;

//	this->m_VocableList.sort();
	beginit = this->m_VocableList.begin();
	endit = this->m_VocableList.end();
	tempit = beginit;

	int sum = 0;
	while(tempit != endit)
	{
		//设置句柄 不重复词
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
  函数名称:       BTreeSeach::MatchingDic
  描述:            匹配词典并执行回调函数
  输入参数:        buffer
                   lenbuf
				   回调函数 mathmothod
  返回值:          函数返回值的说明
  其它:            其它说明
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
		//查找子节点
		while(oldpos < lenbuf)
		{
				if(buf[oldpos] <= ' ')
				{
					break;
				}
				
				pTree = pTree->Seek((UCHAR)buf[oldpos]);
				if(pTree>0)
				{
					//是否匹配
					if((INT)pTree->m_pUserData>0)
					{
						//执行回调
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
  函数名称:       BTreeSeach::AddVocable
  描述:            增加词
  输入参数:        str
                   用户数据结构指针
  输出参数:        列表增加数据
  其它:            其它说明
*************************************************/
void BTreeSeach::AddVocable(CString str,VOID* m_pUserData)
{
	Vocable temp;
 	temp.m_Vocable = str;
 	temp.m_pUserData = m_pUserData;
 	this->m_VocableList.push_front(temp);
}