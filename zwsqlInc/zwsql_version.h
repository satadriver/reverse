/* Copyright Abandoned 1996, 1999, 2001 zwsql AB
   This file is public domain and comes with NO WARRANTY of any kind */

/* Version numbers for protocol & zwsqld */

#ifndef _zwsql_version_h
#define _zwsql_version_h
#ifdef _CUSTOMCONFIG_
#include <custom_conf.h>
#else
#define PROTOCOL_VERSION		10
#define ZWSQL_SERVER_VERSION		"5.0.18"
#define ZWSQL_BASE_VERSION		"zwsqld-5.0"
#define ZWSQL_SERVER_SUFFIX_DEF	""
#define FRM_VER				6
#define ZWSQL_VERSION_ID		50018
#define ZWSQL_PORT			3306
#define ZWSQL_UNIX_ADDR			"/tmp/zwsql.sock"
#define ZWSQL_CONFIG_NAME		"my"
#define ZWSQL_COMPILATION_COMMENT	"zwsql Community Edition (GPL)"

/* zwsqld compile time options */
#endif /* _CUSTOMCONFIG_ */

#ifndef LICENSE
#define LICENSE				GPL
#endif /* LICENSE */

#endif /* _zwsql_version_h */
