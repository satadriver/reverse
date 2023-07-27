/* Copyright (C) 2000-2003 zwsql AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifndef _zwsql_h
#define _zwsql_h

#ifdef __CYGWIN__     /* CYGWIN implements a UNIX API */
#undef WIN
#undef _WIN
#undef _WIN32
#undef _WIN64
#undef __WIN__
#endif

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _global_h				/* If not standard header */
#include <sys/types.h>
#ifdef __LCC__
#include <winsock.h>				/* For windows */
#endif
typedef char my_bool;
#if (defined(_WIN32) || defined(_WIN64)) && !defined(__WIN__)
#define __WIN__
#endif
#if !defined(__WIN__)
#define STDCALL
#else
#define STDCALL __stdcall
#endif
typedef char * gptr;

#ifndef my_socket_defined
#ifdef __WIN__
#define my_socket SOCKET
#else
typedef int my_socket;
#endif /* __WIN__ */
#endif /* my_socket_defined */
#endif /* _global_h */

#include "zwsql_com.h"
#include "zwsql_time.h"
#include "zwsql_version.h"
#include "typelib.h"

#include "zw_list.h" /* for LISTs used in 'ZWSQL' and 'ZWSQL_STMT' */

extern unsigned int zwsql_port;
extern char *zwsql_unix_port;

#define CLIENT_NET_READ_TIMEOUT		365*24*3600	/* Timeout on read */
#define CLIENT_NET_WRITE_TIMEOUT	365*24*3600	/* Timeout on write */

#ifdef __NETWARE__
#pragma pack(push, 8)		/* 8 byte alignment */
#endif

#define IS_PRI_KEY(n)	((n) & PRI_KEY_FLAG)
#define IS_NOT_NULL(n)	((n) & NOT_NULL_FLAG)
#define IS_BLOB(n)	((n) & BLOB_FLAG)
#define IS_NUM(t)	((t) <= FIELD_TYPE_INT24 || (t) == FIELD_TYPE_YEAR || (t) == FIELD_TYPE_NEWDECIMAL)
#define IS_NUM_FIELD(f)	 ((f)->flags & NUM_FLAG)
#define INTERNAL_NUM_FIELD(f) (((f)->type <= FIELD_TYPE_INT24 && ((f)->type != FIELD_TYPE_TIMESTAMP || (f)->length == 14 || (f)->length == 8)) || (f)->type == FIELD_TYPE_YEAR)


typedef struct st_zwsql_field {
  char *name;                 /* Name of column */
  char *org_name;             /* Original column name, if an alias */
  char *table;                /* Table of column if column was a field */
  char *org_table;            /* Org table name, if table was an alias */
  char *db;                   /* Database for table */
  char *catalog;	      /* Catalog for table */
  char *def;                  /* Default value (set by zwsql_list_fields) */
  unsigned long length;       /* Width of column (create length) */
  unsigned long max_length;   /* Max width for selected set */
  unsigned int name_length;
  unsigned int org_name_length;
  unsigned int table_length;
  unsigned int org_table_length;
  unsigned int db_length;
  unsigned int catalog_length;
  unsigned int def_length;
  unsigned int flags;         /* Div flags */
  unsigned int decimals;      /* Number of decimals in field */
  unsigned int charsetnr;     /* Character set */
  enum enum_field_types type; /* Type of field. See zwsql_com.h for types */
} ZWSQL_FIELD;

typedef char **ZWSQL_ROW;		/* return data as array of strings */
typedef unsigned int ZWSQL_FIELD_OFFSET; /* offset to current field */

#ifndef _global_h
#if defined(NO_CLIENT_LONG_LONG)
typedef unsigned long my_ulonglong;
#elif defined (__WIN__)
typedef unsigned __int64 my_ulonglong;
#else
typedef unsigned long long my_ulonglong;
#endif
#endif

#define ZWSQL_COUNT_ERROR (~(my_ulonglong) 0)

/* backward compatibility define - to be removed eventually */
#define ER_WARN_DATA_TRUNCATED WARN_DATA_TRUNCATED

typedef struct st_zwsql_rows {
  struct st_zwsql_rows *next;		/* list of rows */
  ZWSQL_ROW data;
  unsigned long length;
} ZWSQL_ROWS;

typedef ZWSQL_ROWS *ZWSQL_ROW_OFFSET;	/* offset to current row */

#include "zw_alloc.h"

typedef struct st_zwsql_data {
  my_ulonglong rows;
  unsigned int fields;
  ZWSQL_ROWS *data;
  MEM_ROOT alloc;
#if !defined(CHECK_EMBEDDED_DIFFERENCES) || defined(EMBEDDED_LIBRARY)
  ZWSQL_ROWS **prev_ptr;
#endif
} ZWSQL_DATA;

enum zwsql_option 
{
  ZWSQL_OPT_CONNECT_TIMEOUT, ZWSQL_OPT_COMPRESS, ZWSQL_OPT_NAMED_PIPE,
  ZWSQL_INIT_COMMAND, ZWSQL_READ_DEFAULT_FILE, ZWSQL_READ_DEFAULT_GROUP,
  ZWSQL_SET_CHARSET_DIR, ZWSQL_SET_CHARSET_NAME, ZWSQL_OPT_LOCAL_INFILE,
  ZWSQL_OPT_PROTOCOL, ZWSQL_SHARED_MEMORY_BASE_NAME, ZWSQL_OPT_READ_TIMEOUT,
  ZWSQL_OPT_WRITE_TIMEOUT, ZWSQL_OPT_USE_RESULT,
  ZWSQL_OPT_USE_REMOTE_CONNECTION, ZWSQL_OPT_USE_EMBEDDED_CONNECTION,
  ZWSQL_OPT_GUESS_CONNECTION, ZWSQL_SET_CLIENT_IP, ZWSQL_SECURE_AUTH,
  ZWSQL_REPORT_DATA_TRUNCATION, ZWSQL_OPT_RECONNECT
};

struct st_zwsql_options {
  unsigned int connect_timeout, read_timeout, write_timeout;
  unsigned int port, protocol;
  unsigned long client_flag;
  char *host,*user,*password,*unix_socket,*db;
  struct st_dynamic_array *init_commands;
  char *my_cnf_file,*my_cnf_group, *charset_dir, *charset_name;
  char *ssl_key;				/* PEM key file */
  char *ssl_cert;				/* PEM cert file */
  char *ssl_ca;					/* PEM CA file */
  char *ssl_capath;				/* PEM directory of CA-s? */
  char *ssl_cipher;				/* cipher to use */
  char *shared_memory_base_name;
  unsigned long max_allowed_packet;
  my_bool use_ssl;				/* if to use SSL or not */
  my_bool compress,named_pipe;
 /*
   On connect, find out the replication role of the server, and
   establish connections to all the peers
 */
  my_bool rpl_probe;
 /*
   Each call to zwsql_real_query() will parse it to tell if it is a read
   or a write, and direct it to the slave or the master
 */
  my_bool rpl_parse;
 /*
   If set, never read from a master, only from slave, when doing
   a read that is replication-aware
 */
  my_bool no_master_reads;
#if !defined(CHECK_EMBEDDED_DIFFERENCES) || defined(EMBEDDED_LIBRARY)
  my_bool separate_thread;
#endif
  enum zwsql_option methods_to_use;
  char *client_ip;
  /* Refuse client connecting to server if it uses old (pre-4.1.1) protocol */
  my_bool secure_auth;
  /* 0 - never report, 1 - always report (default) */
  my_bool report_data_truncation;

  /* function pointers for local infile support */
  int (*local_infile_init)(void **, const char *, void *);
  int (*local_infile_read)(void *, char *, unsigned int);
  void (*local_infile_end)(void *);
  int (*local_infile_error)(void *, char *, unsigned int);
  void *local_infile_userdata;
};

enum zwsql_status 
{
  ZWSQL_STATUS_READY,ZWSQL_STATUS_GET_RESULT,ZWSQL_STATUS_USE_RESULT
};

enum zwsql_protocol_type 
{
  ZWSQL_PROTOCOL_DEFAULT, ZWSQL_PROTOCOL_TCP, ZWSQL_PROTOCOL_SOCKET,
  ZWSQL_PROTOCOL_PIPE, ZWSQL_PROTOCOL_MEMORY
};
/*
  There are three types of queries - the ones that have to go to
  the master, the ones that go to a slave, and the adminstrative
  type which must happen on the pivot connectioin
*/
enum zwsql_rpl_type 
{
  ZWSQL_RPL_MASTER, ZWSQL_RPL_SLAVE, ZWSQL_RPL_ADMIN
};

typedef struct character_set
{
  unsigned int      number;     /* character set number              */
  unsigned int      state;      /* character set state               */
  const char        *csname;    /* collation name                    */
  const char        *name;      /* character set name                */
  const char        *comment;   /* comment                           */
  const char        *dir;       /* character set directory           */
  unsigned int      mbminlen;   /* min. length for multibyte strings */
  unsigned int      mbmaxlen;   /* max. length for multibyte strings */
} MY_CHARSET_INFO;

struct st_zwsql_methods;

typedef struct st_zwsql
{
  NET		net;			/* Communication parameters */
  gptr		connector_fd;		/* ConnectorFd for SSL */
  char		*host,*user,*passwd,*unix_socket,*server_version,*host_info,*info;
  char          *db;
  struct charset_info_st *charset;
  ZWSQL_FIELD	*fields;
  MEM_ROOT	field_alloc;
  my_ulonglong affected_rows;
  my_ulonglong insert_id;		/* id if insert on table with NEXTNR */
  my_ulonglong extra_info;		/* Not used */
  unsigned long thread_id;		/* Id for connection in server */
  unsigned long packet_length;
  unsigned int	port;
  unsigned long client_flag,server_capabilities;
  unsigned int	protocol_version;
  unsigned int	field_count;
  unsigned int 	server_status;
  unsigned int  server_language;
  unsigned int	warning_count;
  struct st_zwsql_options options;
  enum zwsql_status status;
  my_bool	free_me;		/* If free in zwsql_close */
  my_bool	reconnect;		/* set to 1 if automatic reconnect */

  /* session-wide random string */
  char	        scramble[SCRAMBLE_LENGTH+1];

 /*
   Set if this is the original connection, not a master or a slave we have
   added though zwsql_rpl_probe() or zwsql_set_master()/ zwsql_add_slave()
 */
  my_bool rpl_pivot;
  /*
    Pointers to the master, and the next slave connections, points to
    itself if lone connection.
  */
  struct st_zwsql* master, *next_slave;

  struct st_zwsql* last_used_slave; /* needed for round-robin slave pick */
 /* needed for send/read/store/use result to work correctly with replication */
  struct st_zwsql* last_used_con;

  LIST  *stmts;                     /* list of all statements */
  const struct st_zwsql_methods *methods;
  void *thd;
  /*
    Points to boolean flag in ZWSQL_RES  or ZWSQL_STMT. We set this flag 
    from zwsql_stmt_close if close had to cancel result set of this object.
  */
  my_bool *unbuffered_fetch_owner;
} ZWSQL;

typedef struct st_zwsql_res {
  my_ulonglong row_count;
  ZWSQL_FIELD	*fields;
  ZWSQL_DATA	*data;
  ZWSQL_ROWS	*data_cursor;
  unsigned long *lengths;		/* column lengths of current row */
  ZWSQL		*handle;		/* for unbuffered reads */
  MEM_ROOT	field_alloc;
  unsigned int	field_count, current_field;
  ZWSQL_ROW	row;			/* If unbuffered read */
  ZWSQL_ROW	current_row;		/* buffer to current row */
  my_bool	eof;			/* Used by zwsql_fetch_row */
  /* zwsql_stmt_close() had to cancel this result */
  my_bool       unbuffered_fetch_cancelled;  
  const struct st_zwsql_methods *methods;
} ZWSQL_RES;

#define MAX_ZWSQL_MANAGER_ERR 256  
#define MAX_ZWSQL_MANAGER_MSG 256

#define MANAGER_OK           200
#define MANAGER_INFO         250
#define MANAGER_ACCESS       401
#define MANAGER_CLIENT_ERR   450
#define MANAGER_INTERNAL_ERR 500

#if !defined(ZWSQL_SERVER) && !defined(ZWSQL_CLIENT)
#define ZWSQL_CLIENT
#endif


typedef struct st_zwsql_manager
{
  NET net;
  char *host,*user,*passwd;
  unsigned int port;
  my_bool free_me;
  my_bool eof;
  int cmd_status;
  int last_errno;
  char* net_buf,*net_buf_pos,*net_data_end;
  int net_buf_size;
  char last_error[MAX_ZWSQL_MANAGER_ERR];
} ZWSQL_MANAGER;

typedef struct st_zwsql_parameters
{
  unsigned long *p_max_allowed_packet;
  unsigned long *p_net_buffer_length;
} ZWSQL_PARAMETERS;

#if !defined(ZWSQL_SERVER) && !defined(EMBEDDED_LIBRARY)
#define max_allowed_packet (*zwsql_get_parameters()->p_max_allowed_packet)
#define net_buffer_length (*zwsql_get_parameters()->p_net_buffer_length)
#endif

/*
  Set up and bring down the server; to ensure that applications will
  work when linked against either the standard client library or the
  embedded server library, these functions should be called.
*/
int STDCALL zwsql_server_init(int argc, char **argv, char **groups);
void STDCALL zwsql_server_end(void);
/*
  zwsql_server_init/end need to be called when using libzwsqld or
  libzwsqlclient (exactly, zwsql_server_init() is called by zwsql_init() so
  you don't need to call it explicitely; but you need to call
  zwsql_server_end() to free memory). The names are a bit misleading
  (zwsql_SERVER* to be used when using libzwsqlCLIENT). So we add more general
  names which suit well whether you're using libzwsqld or libzwsqlclient. We
  intend to promote these aliases over the zwsql_server* ones.
*/
#define zwsql_library_init zwsql_server_init
#define zwsql_library_end zwsql_server_end

ZWSQL_PARAMETERS *STDCALL zwsql_get_parameters(void);

/*
  Set up and bring down a thread; these function should be called
  for each thread in an application which opens at least one zwsql
  connection.  All uses of the connection(s) should be between these
  function calls.
*/
my_bool STDCALL zwsql_thread_init(void);
void STDCALL zwsql_thread_end(void);

/*
  Functions to get information from the ZWSQL and ZWSQL_RES structures
  Should definitely be used if one uses shared libraries.
*/

my_ulonglong STDCALL zwsql_num_rows(ZWSQL_RES *res);
unsigned int STDCALL zwsql_num_fields(ZWSQL_RES *res);
my_bool STDCALL zwsql_eof(ZWSQL_RES *res);
ZWSQL_FIELD *STDCALL zwsql_fetch_field_direct(ZWSQL_RES *res,
					      unsigned int fieldnr);
ZWSQL_FIELD * STDCALL zwsql_fetch_fields(ZWSQL_RES *res);
ZWSQL_ROW_OFFSET STDCALL zwsql_row_tell(ZWSQL_RES *res);
ZWSQL_FIELD_OFFSET STDCALL zwsql_field_tell(ZWSQL_RES *res);

unsigned int STDCALL zwsql_field_count(ZWSQL *zwsql);
my_ulonglong STDCALL zwsql_affected_rows(ZWSQL *zwsql);
my_ulonglong STDCALL zwsql_insert_id(ZWSQL *zwsql);
unsigned int STDCALL zwsql_errno(ZWSQL *zwsql);
const char * STDCALL zwsql_error(ZWSQL *zwsql);
const char *STDCALL zwsql_sqlstate(ZWSQL *zwsql);
unsigned int STDCALL zwsql_warning_count(ZWSQL *zwsql);
const char * STDCALL zwsql_info(ZWSQL *zwsql);
unsigned long STDCALL zwsql_thread_id(ZWSQL *zwsql);
const char * STDCALL zwsql_character_set_name(ZWSQL *zwsql);
int          STDCALL zwsql_set_character_set(ZWSQL *zwsql, const char *csname);

ZWSQL *		STDCALL zwsql_init(ZWSQL *zwsql);
my_bool		STDCALL zwsql_ssl_set(ZWSQL *zwsql, const char *key,
				      const char *cert, const char *ca,
				      const char *capath, const char *cipher);
my_bool		STDCALL zwsql_change_user(ZWSQL *zwsql, const char *user, 
					  const char *passwd, const char *db);
ZWSQL *		STDCALL zwsql_real_connect(ZWSQL *zwsql, const char *host,
					   const char *user,
					   const char *passwd,
					   const char *db,
					   unsigned int port,
					   const char *unix_socket,
					   unsigned long clientflag);
int		STDCALL zwsql_select_db(ZWSQL *zwsql, const char *db);
int		STDCALL zwsql_query(ZWSQL *zwsql, const char *q);
int		STDCALL zwsql_send_query(ZWSQL *zwsql, const char *q,
					 unsigned long length);
int		STDCALL zwsql_real_query(ZWSQL *zwsql, const char *q,
					unsigned long length);
ZWSQL_RES *     STDCALL zwsql_store_result(ZWSQL *zwsql);
ZWSQL_RES *     STDCALL zwsql_use_result(ZWSQL *zwsql);

/* perform query on master */
my_bool		STDCALL zwsql_master_query(ZWSQL *zwsql, const char *q,
					   unsigned long length);
my_bool		STDCALL zwsql_master_send_query(ZWSQL *zwsql, const char *q,
						unsigned long length);
/* perform query on slave */  
my_bool		STDCALL zwsql_slave_query(ZWSQL *zwsql, const char *q,
					  unsigned long length);
my_bool		STDCALL zwsql_slave_send_query(ZWSQL *zwsql, const char *q,
					       unsigned long length);
void        STDCALL zwsql_get_character_set_info(ZWSQL *zwsql,
                           MY_CHARSET_INFO *charset);

/* local infile support */

#define LOCAL_INFILE_ERROR_LEN 512

void
zwsql_set_local_infile_handler(ZWSQL *zwsql,
                               int (*local_infile_init)(void **, const char *,
                            void *),
                               int (*local_infile_read)(void *, char *,
							unsigned int),
                               void (*local_infile_end)(void *),
                               int (*local_infile_error)(void *, char*,
							 unsigned int),
                               void *);

void
zwsql_set_local_infile_default(ZWSQL *zwsql);


/*
  enable/disable parsing of all queries to decide if they go on master or
  slave
*/
void            STDCALL zwsql_enable_rpl_parse(ZWSQL* zwsql);
void            STDCALL zwsql_disable_rpl_parse(ZWSQL* zwsql);
/* get the value of the parse flag */  
int             STDCALL zwsql_rpl_parse_enabled(ZWSQL* zwsql);

/*  enable/disable reads from master */
void            STDCALL zwsql_enable_reads_from_master(ZWSQL* zwsql);
void            STDCALL zwsql_disable_reads_from_master(ZWSQL* zwsql);
/* get the value of the master read flag */  
my_bool		STDCALL zwsql_reads_from_master_enabled(ZWSQL* zwsql);

enum zwsql_rpl_type     STDCALL zwsql_rpl_query_type(const char* q, int len);  

/* discover the master and its slaves */  
my_bool		STDCALL zwsql_rpl_probe(ZWSQL* zwsql);

/* set the master, close/free the old one, if it is not a pivot */
int             STDCALL zwsql_set_master(ZWSQL* zwsql, const char* host,
					 unsigned int port,
					 const char* user,
					 const char* passwd);
int             STDCALL zwsql_add_slave(ZWSQL* zwsql, const char* host,
					unsigned int port,
					const char* user,
					const char* passwd);

int		STDCALL zwsql_shutdown(ZWSQL *zwsql,
                                       enum zwsql_enum_shutdown_level
                                       shutdown_level);
int		STDCALL zwsql_dump_debug_info(ZWSQL *zwsql);
int		STDCALL zwsql_refresh(ZWSQL *zwsql,
				     unsigned int refresh_options);
int		STDCALL zwsql_kill(ZWSQL *zwsql,unsigned long pid);
int		STDCALL zwsql_set_server_option(ZWSQL *zwsql,
						enum enum_zwsql_set_option
						option);
int		STDCALL zwsql_ping(ZWSQL *zwsql);
const char *	STDCALL zwsql_stat(ZWSQL *zwsql);
const char *	STDCALL zwsql_get_server_info(ZWSQL *zwsql);
const char *	STDCALL zwsql_get_client_info(void);
unsigned long	STDCALL zwsql_get_client_version(void);
const char *	STDCALL zwsql_get_host_info(ZWSQL *zwsql);
unsigned long	STDCALL zwsql_get_server_version(ZWSQL *zwsql);
unsigned int	STDCALL zwsql_get_proto_info(ZWSQL *zwsql);
ZWSQL_RES *	STDCALL zwsql_list_dbs(ZWSQL *zwsql,const char *wild);
ZWSQL_RES *	STDCALL zwsql_list_tables(ZWSQL *zwsql,const char *wild);
ZWSQL_RES *	STDCALL zwsql_list_processes(ZWSQL *zwsql);
int		STDCALL zwsql_options(ZWSQL *zwsql,enum zwsql_option option,
				      const char *arg);
void		STDCALL zwsql_free_result(ZWSQL_RES *result);
void		STDCALL zwsql_data_seek(ZWSQL_RES *result,
					my_ulonglong offset);
ZWSQL_ROW_OFFSET STDCALL zwsql_row_seek(ZWSQL_RES *result,
						ZWSQL_ROW_OFFSET offset);
ZWSQL_FIELD_OFFSET STDCALL zwsql_field_seek(ZWSQL_RES *result,
					   ZWSQL_FIELD_OFFSET offset);
ZWSQL_ROW	STDCALL zwsql_fetch_row(ZWSQL_RES *result);
unsigned long * STDCALL zwsql_fetch_lengths(ZWSQL_RES *result);
ZWSQL_FIELD *	STDCALL zwsql_fetch_field(ZWSQL_RES *result);
ZWSQL_RES *     STDCALL zwsql_list_fields(ZWSQL *zwsql, const char *table,
					  const char *wild);
unsigned long	STDCALL zwsql_escape_string(char *to,const char *from,
					    unsigned long from_length);
unsigned long	STDCALL zwsql_hex_string(char *to,const char *from,
                                         unsigned long from_length);
unsigned long STDCALL zwsql_real_escape_string(ZWSQL *zwsql,
					       char *to,const char *from,
					       unsigned long length);
void		STDCALL zwsql_debug(const char *debug);
char *		STDCALL zwsql_odbc_escape_string(ZWSQL *zwsql,
						 char *to,
						 unsigned long to_length,
						 const char *from,
						 unsigned long from_length,
						 void *param,
						 char *
						 (*extend_buffer)
						 (void *, char *to,
						  unsigned long *length));
void 		STDCALL myodbc_remove_escape(ZWSQL *zwsql,char *name);
unsigned int	STDCALL zwsql_thread_safe(void);
my_bool		STDCALL zwsql_embedded(void);
ZWSQL_MANAGER*  STDCALL zwsql_manager_init(ZWSQL_MANAGER* con);  
ZWSQL_MANAGER*  STDCALL zwsql_manager_connect(ZWSQL_MANAGER* con,
					      const char* host,
					      const char* user,
					      const char* passwd,
					      unsigned int port);
void            STDCALL zwsql_manager_close(ZWSQL_MANAGER* con);
int             STDCALL zwsql_manager_command(ZWSQL_MANAGER* con,
						const char* cmd, int cmd_len);
int             STDCALL zwsql_manager_fetch_line(ZWSQL_MANAGER* con,
						  char* res_buf,
						 int res_buf_size);
my_bool         STDCALL zwsql_read_query_result(ZWSQL *zwsql);


/*
  The following definitions are added for the enhanced 
  client-server protocol
*/

/* statement state */
enum enum_zwsql_stmt_state
{
  ZWSQL_STMT_INIT_DONE= 1, ZWSQL_STMT_PREPARE_DONE, ZWSQL_STMT_EXECUTE_DONE,
  ZWSQL_STMT_FETCH_DONE
};


/*
  This structure is used to define bind information, and
  internally by the client library.
  Public members with their descriptions are listed below
  (conventionally `On input' refers to the binds given to
  zwsql_stmt_bind_param, `On output' refers to the binds given
  to zwsql_stmt_bind_result):

  buffer_type    - One of the ZWSQL_* types, used to describe
                   the host language type of buffer.
                   On output: if column type is different from
                   buffer_type, column value is automatically converted
                   to buffer_type before it is stored in the buffer.
  buffer         - On input: points to the buffer with input data.
                   On output: points to the buffer capable to store
                   output data.
                   The type of memory pointed by buffer must correspond
                   to buffer_type. See the correspondence table in
                   the comment to zwsql_stmt_bind_param.

  The two above members are mandatory for any kind of bind.

  buffer_length  - the length of the buffer. You don't have to set
                   it for any fixed length buffer: float, double,
                   int, etc. It must be set however for variable-length
                   types, such as BLOBs or STRINGs.

  length         - On input: in case when lengths of input values
                   are different for each execute, you can set this to
                   point at a variable containining value length. This
                   way the value length can be different in each execute.
                   If length is not NULL, buffer_length is not used.
                   Note, length can even point at buffer_length if
                   you keep bind structures around while fetching:
                   this way you can change buffer_length before
                   each execution, everything will work ok.
                   On output: if length is set, zwsql_stmt_fetch will
                   write column length into it.

  is_null        - On input: points to a boolean variable that should
                   be set to TRUE for NULL values.
                   This member is useful only if your data may be
                   NULL in some but not all cases.
                   If your data is never NULL, is_null should be set to 0.
                   If your data is always NULL, set buffer_type
                   to ZWSQL_TYPE_NULL, and is_null will not be used.

  is_unsigned    - On input: used to signify that values provided for one
                   of numeric types are unsigned.
                   On output describes signedness of the output buffer.
                   If, taking into account is_unsigned flag, column data
                   is out of range of the output buffer, data for this column
                   is regarded truncated. Note that this has no correspondence
                   to the sign of result set column, if you need to find it out
                   use zwsql_stmt_result_metadata.
  error          - where to write a truncation error if it is present.
                   possible error value is:
                   0  no truncation
                   1  value is out of range or buffer is too small

  Please note that ZWSQL_BIND also has internals members.
*/

typedef struct st_zwsql_bind
{
  unsigned long	*length;          /* output length pointer */
  my_bool       *is_null;	  /* Pointer to null indicator */
  void		*buffer;	  /* buffer to get/put data */
  /* set this if you want to track data truncations happened during fetch */
  my_bool       *error;
  enum enum_field_types buffer_type;	/* buffer type */
  /* output buffer length, must be set when fetching str/binary */
  unsigned long buffer_length;
  unsigned char *row_ptr;         /* for the current data position */
  unsigned long offset;           /* offset position for char/binary fetch */
  unsigned long	length_value;     /* Used if length is 0 */
  unsigned int	param_number;	  /* For null count and error messages */
  unsigned int  pack_length;	  /* Internal length for packed data */
  my_bool       error_value;      /* used if error is 0 */
  my_bool       is_unsigned;      /* set if integer type is unsigned */
  my_bool	long_data_used;	  /* If used with zwsql_send_long_data */
  my_bool	is_null_value;    /* Used if is_null is 0 */
  void (*store_param_func)(NET *net, struct st_zwsql_bind *param);
  void (*fetch_result)(struct st_zwsql_bind *, ZWSQL_FIELD *,
                       unsigned char **row);
  void (*skip_result)(struct st_zwsql_bind *, ZWSQL_FIELD *,
		      unsigned char **row);
} ZWSQL_BIND;


/* statement handler */
typedef struct st_zwsql_stmt
{
  MEM_ROOT       mem_root;             /* root allocations */
  LIST           list;                 /* list to keep track of all stmts */
  ZWSQL          *zwsql;               /* connection handle */
  ZWSQL_BIND     *params;              /* input parameters */
  ZWSQL_BIND     *bind;                /* output parameters */
  ZWSQL_FIELD    *fields;              /* result set metadata */
  ZWSQL_DATA     result;               /* cached result set */
  ZWSQL_ROWS     *data_cursor;         /* current row in cached result */
  /* copy of zwsql->affected_rows after statement execution */
  my_ulonglong   affected_rows;
  my_ulonglong   insert_id;            /* copy of zwsql->insert_id */
  /*
    zwsql_stmt_fetch() calls this function to fetch one row (it's different
    for buffered, unbuffered and cursor fetch).
  */
  int            (*read_row_func)(struct st_zwsql_stmt *stmt, 
                                  unsigned char **row);
  unsigned long	 stmt_id;	       /* Id for prepared statement */
  unsigned long  flags;                /* i.e. type of cursor to open */
  unsigned long  prefetch_rows;        /* number of rows per one COM_FETCH */
  /*
    Copied from zwsql->server_status after execute/fetch to know
    server-side cursor status for this statement.
  */
  unsigned int   server_status;
  unsigned int	 last_errno;	       /* error code */
  unsigned int   param_count;          /* input parameter count */
  unsigned int   field_count;          /* number of columns in result set */
  enum enum_zwsql_stmt_state state;    /* statement state */
  char		 last_error[ZWSQL_ERRMSG_SIZE]; /* error message */
  char		 sqlstate[SQLSTATE_LENGTH+1];
  /* Types of input parameters should be sent to server */
  my_bool        send_types_to_server;
  my_bool        bind_param_done;      /* input buffers were supplied */
  unsigned char  bind_result_done;     /* output buffers were supplied */
  /* zwsql_stmt_close() had to cancel this result */
  my_bool       unbuffered_fetch_cancelled;  
  /*
    Is set to true if we need to calculate field->max_length for 
    metadata fields when doing zwsql_stmt_store_result.
  */
  my_bool       update_max_length;     
} ZWSQL_STMT;

enum enum_stmt_attr_type
{
  /*
    When doing zwsql_stmt_store_result calculate max_length attribute
    of statement metadata. This is to be consistent with the old API, 
    where this was done automatically.
    In the new API we do that only by request because it slows down
    zwsql_stmt_store_result sufficiently.
  */
  STMT_ATTR_UPDATE_MAX_LENGTH,
  /*
    unsigned long with combination of cursor flags (read only, for update,
    etc)
  */
  STMT_ATTR_CURSOR_TYPE,
  /*
    Amount of rows to retrieve from server per one fetch if using cursors.
    Accepts unsigned long attribute in the range 1 - ulong_max
  */
  STMT_ATTR_PREFETCH_ROWS
};


typedef struct st_zwsql_methods
{
  my_bool (*read_query_result)(ZWSQL *zwsql);
  my_bool (*advanced_command)(ZWSQL *zwsql,
			      enum enum_server_command command,
			      const char *header,
			      unsigned long header_length,
			      const char *arg,
			      unsigned long arg_length,
			      my_bool skip_check);
  ZWSQL_DATA *(*read_rows)(ZWSQL *zwsql,ZWSQL_FIELD *zwsql_fields,
			   unsigned int fields);
  ZWSQL_RES * (*use_result)(ZWSQL *zwsql);
  void (*fetch_lengths)(unsigned long *to, 
			ZWSQL_ROW column, unsigned int field_count);
  void (*flush_use_result)(ZWSQL *zwsql);
#if !defined(ZWSQL_SERVER) || defined(EMBEDDED_LIBRARY)
  ZWSQL_FIELD * (*list_fields)(ZWSQL *zwsql);
  my_bool (*read_prepare_result)(ZWSQL *zwsql, ZWSQL_STMT *stmt);
  int (*stmt_execute)(ZWSQL_STMT *stmt);
  int (*read_binary_rows)(ZWSQL_STMT *stmt);
  int (*unbuffered_fetch)(ZWSQL *zwsql, char **row);
  void (*free_embedded_thd)(ZWSQL *zwsql);
  const char *(*read_statistics)(ZWSQL *zwsql);
  my_bool (*next_result)(ZWSQL *zwsql);
  int (*read_change_user_result)(ZWSQL *zwsql, char *buff, const char *passwd);
#endif
} ZWSQL_METHODS;


ZWSQL_STMT * STDCALL zwsql_stmt_init(ZWSQL *zwsql);
int STDCALL zwsql_stmt_prepare(ZWSQL_STMT *stmt, const char *query,
                               unsigned long length);
int STDCALL zwsql_stmt_execute(ZWSQL_STMT *stmt);
int STDCALL zwsql_stmt_fetch(ZWSQL_STMT *stmt);
int STDCALL zwsql_stmt_fetch_column(ZWSQL_STMT *stmt, ZWSQL_BIND *bind, 
                                    unsigned int column,
                                    unsigned long offset);
int STDCALL zwsql_stmt_store_result(ZWSQL_STMT *stmt);
unsigned long STDCALL zwsql_stmt_param_count(ZWSQL_STMT * stmt);
my_bool STDCALL zwsql_stmt_attr_set(ZWSQL_STMT *stmt,
                                    enum enum_stmt_attr_type attr_type,
                                    const void *attr);
my_bool STDCALL zwsql_stmt_attr_get(ZWSQL_STMT *stmt,
                                    enum enum_stmt_attr_type attr_type,
                                    void *attr);
my_bool STDCALL zwsql_stmt_bind_param(ZWSQL_STMT * stmt, ZWSQL_BIND * bnd);
my_bool STDCALL zwsql_stmt_bind_result(ZWSQL_STMT * stmt, ZWSQL_BIND * bnd);
my_bool STDCALL zwsql_stmt_close(ZWSQL_STMT * stmt);
my_bool STDCALL zwsql_stmt_reset(ZWSQL_STMT * stmt);
my_bool STDCALL zwsql_stmt_free_result(ZWSQL_STMT *stmt);
my_bool STDCALL zwsql_stmt_send_long_data(ZWSQL_STMT *stmt, 
                                          unsigned int param_number,
                                          const char *data, 
                                          unsigned long length);
ZWSQL_RES *STDCALL zwsql_stmt_result_metadata(ZWSQL_STMT *stmt);
ZWSQL_RES *STDCALL zwsql_stmt_param_metadata(ZWSQL_STMT *stmt);
unsigned int STDCALL zwsql_stmt_errno(ZWSQL_STMT * stmt);
const char *STDCALL zwsql_stmt_error(ZWSQL_STMT * stmt);
const char *STDCALL zwsql_stmt_sqlstate(ZWSQL_STMT * stmt);
ZWSQL_ROW_OFFSET STDCALL zwsql_stmt_row_seek(ZWSQL_STMT *stmt, 
                                             ZWSQL_ROW_OFFSET offset);
ZWSQL_ROW_OFFSET STDCALL zwsql_stmt_row_tell(ZWSQL_STMT *stmt);
void STDCALL zwsql_stmt_data_seek(ZWSQL_STMT *stmt, my_ulonglong offset);
my_ulonglong STDCALL zwsql_stmt_num_rows(ZWSQL_STMT *stmt);
my_ulonglong STDCALL zwsql_stmt_affected_rows(ZWSQL_STMT *stmt);
my_ulonglong STDCALL zwsql_stmt_insert_id(ZWSQL_STMT *stmt);
unsigned int STDCALL zwsql_stmt_field_count(ZWSQL_STMT *stmt);

my_bool STDCALL zwsql_commit(ZWSQL * zwsql);
my_bool STDCALL zwsql_rollback(ZWSQL * zwsql);
my_bool STDCALL zwsql_autocommit(ZWSQL * zwsql, my_bool auto_mode);
my_bool STDCALL zwsql_more_results(ZWSQL *zwsql);
int STDCALL zwsql_next_result(ZWSQL *zwsql);
void STDCALL zwsql_close(ZWSQL *sock);


/* status return codes */
#define ZWSQL_NO_DATA        100
#define ZWSQL_DATA_TRUNCATED 101

#define zwsql_reload(zwsql) zwsql_refresh((zwsql),REFRESH_GRANT)

#ifdef USE_OLD_FUNCTIONS
ZWSQL *		STDCALL zwsql_connect(ZWSQL *zwsql, const char *host,
				      const char *user, const char *passwd);
int		STDCALL zwsql_create_db(ZWSQL *zwsql, const char *DB);
int		STDCALL zwsql_drop_db(ZWSQL *zwsql, const char *DB);
#define	 zwsql_reload(zwsql) zwsql_refresh((zwsql),REFRESH_GRANT)
#endif
#define HAVE_ZWSQL_REAL_CONNECT

/*
  The following functions are mainly exported because of zwsqlbinlog;
  They are not for general usage
*/

#define simple_command(zwsql, command, arg, length, skip_check) \
  (*(zwsql)->methods->advanced_command)(zwsql, command,         \
					NullS, 0, arg, length, skip_check)
unsigned long net_safe_read(ZWSQL* zwsql);

#ifdef __NETWARE__
#pragma pack(pop)		/* restore alignment */
#endif

#ifdef	__cplusplus
}
#endif

#endif /* _zwsql_h */
