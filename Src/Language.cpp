// Language.cpp: implementation of the Language class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dcepub.h"


//CString				v_gstrWordFlag(" ,:.!?\r\n\t()-/\\;'\"");		//断词分割符

static BYTE gstrchar[256] = {0};

CMapStringToPtr		v_EngDicMap(4096);
CMapStringToPtr		v_SLDicMap(4096);
CMapStringToPtr		v_SLDicMap1(4096);

BOOL LoadDic(CMapStringToPtr* pWordMap, LPCTSTR dicFile, UINT LanguageID=1)
{
	FILE *fp;
	CString strKey;
    char line[1024];
	int i;
	int j=0;
	fp = fopen(dicFile,"rt");
	// by zjx 2003-10-25
	if (fp==NULL)
	{
		WriteLog("dce", "Load file %s fail!!", dicFile);
		return FALSE;
	}
	int count = 0;
	while (!feof(fp))
	{
		memset(line,0,sizeof(line));
		if( fgets(line, sizeof(line)-1, fp) == NULL)
			break;
		else
		{
			j++;
			i = 0;
			while(line[i]!=0 && i < 1023)
			{
				line[i]=' ';
				if(line[i+1]==0)
					break;
				line[i+1]=line[i+1] - 127;
				i+=2;
			}
			
			strKey.Format("%s",line);
			strKey.Remove(' ');
			strKey.MakeLower();
			LPVOID val = NULL;
			if (!strKey.IsEmpty())
			{
				count++;
//				printf("%d: %s\n", count, strKey);
				if(!pWordMap->Lookup(strKey, val))
				{
					val = (LPVOID)LanguageID;
					pWordMap->SetAt(strKey, val);
				}
			}
		}
	}
	WriteLog("dce", "LanguageID: %d    Word count: %d    Map Count: %d", LanguageID, count, pWordMap->GetCount());
	fclose(fp);
	return TRUE;
}

// 初始化语种字典
 BOOL InitLanguageDice(const char* wordchar/*=NULL*/, int eWordRatio/*=5*/, int sWordRatio/*=3*/)
{
	// 初始化词正确率
	v_SWordRatio = sWordRatio;		// 词正确率(维语)
	v_EWordRatio = eWordRatio;		// 词正确率(英语)
	
	
	// 初始化拆分字符
	if (wordchar==NULL || wordchar[0]==0)
	{
		wordchar = _T(" ,:.!?\r\n\t()-/\\;'\"");
	}
	WriteLog("dce", "eWordRatio = %d    sWordRatio = %d    wordchar = %s",
		v_EWordRatio, v_SWordRatio, wordchar);
	memset(gstrchar, 0, sizeof(gstrchar));
	gstrchar[0x20] = 1;	// 把' '作为分割符
	gstrchar[0x09] = 1;	// 把'\t'作为分割符
	gstrchar[0x0D] = 1;	// 把'\r'作为分割符
	gstrchar[0x0A] = 1;	// 把'\n'作为分割符

	while (*wordchar)
	{
		gstrchar[(BYTE)*wordchar] = 1;
		wordchar++;
	}
	for (int j='a'; j<='z'; j++)
		gstrchar[j] = 2;
	for (int m='A'; m<'Z'; m++)
		gstrchar[m] = 2;

	// 初始化词库哈希表
	v_EngDicMap.InitHashTable(201119);
	v_SLDicMap.InitHashTable(50891);
	v_SLDicMap1.InitHashTable(50891);
	if (!LoadDic(&v_EngDicMap,  v_RootPath+"\\bin\\dske.dll", 1))
		return FALSE;

	if (!LoadDic(&v_SLDicMap,  v_RootPath+"\\bin\\dsks.dll", 2))
		return FALSE;

	if (!LoadDic(&v_SLDicMap1,  v_RootPath+"\\bin\\dsks1.dll", 3))
		return FALSE;
	return TRUE;
}

inline int FindOneOf(char *p, int len)
{
	for (int i=0; i<len; i++, p++)
	{
		if (gstrchar[(BYTE)*p] == 1)
			break;
	}
	return i;
}

//检查一个单词是否含有高八位
inline int IsWordNeedCheck(LPCTSTR strWord, int len)
{
	if (len > 40)
		return 0;
	for (int j=0; j<len; j++)
	{
		if (gstrchar[(BYTE)*strWord] != 2)
			return 0;
	}
	return 1;
}

inline int CheckWordByDIC(CMapStringToPtr *theMap,LPCTSTR strWord)
{
	LPVOID val;
	if(theMap->Lookup(strWord,val))
		return 1;
	else
		return 0;
}

 LPCTSTR CheckSPL(LPCTSTR strArticle, int len)
{
	if(!v_bLanguage)
		return "";
	long i=0;
	
	int ECount=0;
	int SCount=0;
	
	unsigned long pos=0;
	//	CString strLimit="\r\n|||||||||||||||||||||||||||||||||||||||||||||||\r\n";
	char* strWord;
	CString strAllWord="";
	CString strEWord="";
	char *p = (char*)strArticle;
	CString s;
	
	while(pos < len)
	{
		i=FindOneOf(p+pos, len-pos);
		if(i >= 0)
		{
			p[pos+i]=0;
			strWord = p+pos;
			pos+=i+1;
		}
		else
		{
			strWord = p+pos;
			pos = len;
		}
		char *tp = strchr(strWord, '=');
		if (tp!=NULL)
			strWord=tp+1;
		if(strWord[0] == 0)
			continue;
		if(IsWordNeedCheck(strWord, strlen(strWord)))
		{
			strlwr(strWord);
			if (1==CheckWordByDIC(&v_SLDicMap1,strWord))
				return "维语";
			
			if (1==CheckWordByDIC(&v_SLDicMap,strWord))
			{
				s.Format("%s|", strWord);
				if (strAllWord.Find(s)<0)
				{
					SCount++;
					strAllWord = strAllWord + s;
					if(SCount > v_SWordRatio)
						return "维语";
				}
			}
			

			if (1==CheckWordByDIC(&v_EngDicMap,strWord))
			{
				s.Format("%s|", strWord);
				if (strEWord.Find(s)<0)
				{
					ECount++;
					strEWord = strEWord+s;
				}
			}
		}
	}
	
	if(ECount > v_EWordRatio)
		return "英语";
	else
		return "";
}

 LPCTSTR CheckSPL(CString strArticle)
{
	if(!v_bLanguage)
		return "";
	long i=0;
	
	float ECount=0;
	float SCount=0;
	float SCount1=0;

	unsigned long pos=0;
//	CString strLimit="\r\n|||||||||||||||||||||||||||||||||||||||||||||||\r\n";
	CString strWord;
	CString strAllWord="";
	CString strEWord="";
	char *p;
	float Len;



	Len = strArticle.GetLength();
	p=strArticle.GetBuffer(Len);
	while(pos <Len)
	{
		i=FindOneOf(p+pos, Len-pos);
		if(i >= 0)
		{

			strWord = strArticle.Mid(pos,i);
			pos+=i;
			pos++;
		}
		else
		{
			strWord=strArticle;
			pos=Len;
		}
		if(strWord.IsEmpty())
			continue;
		strWord=strWord.Mid(strWord.Find('=')+1);

		if((strWord.GetLength() > 1) && (IsWordNeedCheck(strWord, strWord.GetLength())))
		{

			if(v_bLanguage)
			{
				SCount1+=CheckWordByDIC(&v_SLDicMap1,strWord);
				if(SCount1 > 0)
					return "维语";
			
				int a=CheckWordByDIC(&v_SLDicMap,strWord);
				if(a > 0 && strAllWord.Find(strWord+"|") < 0)
				{

					SCount +=a;
					strAllWord=strAllWord+strWord+"|";
				}
				//SCount+=CheckWordByDIC(SLDicMap,strWord);
				if(SCount > v_SWordRatio)
					return "维语";
			}
			if(v_bLanguage)
			{
				int a=CheckWordByDIC(&v_EngDicMap,strWord);
				if(a > 0 && strEWord.Find(strWord+"|") < 0)
				{

					ECount +=a;
					strEWord=strEWord+strWord+"|";
				}
			}
		}

	}
	
	if(ECount > v_EWordRatio)
		return "英语";
	else
		return "";
}


