
#ifndef _BM_H_200002250903_DOG_
#define _BM_H_200002250903_DOG_


#define MAXCHARS 256

class RbSearch
{
public:
    int d[MAXCHARS];
    int m;
    char patt[MAXCHARS];
	
	
	RbSearch::RbSearch();
    RbSearch(const char*);
	void InitSearch(const char *);
    int find(const char*);
	int find(const char*,int len);
};

extern RbSearch	g_chk200Search200OK;
extern RbSearch	g_chk200SearchDate;
extern RbSearch	g_chk200SearchCRLF;

#endif//_BM_H_200002250903_DOG_
