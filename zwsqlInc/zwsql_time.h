/* Copyright (C) 2004 zwsql AB & zwsql Finland AB & TCX DataKonsult AB

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

#ifndef _zwsql_time_h_
#define _zwsql_time_h_

/*
  Time declarations shared between the server and client API:
  you should not add anything to this header unless it's used
  (and hence should be visible) in zwsql.h.
  If you're looking for a place to add new time-related declaration,
  it's most likely my_time.h. See also "C API Handling of Date
  and Time Values" chapter in documentation.
*/

enum enum_zwsql_timestamp_type
{
  ZWSQL_TIMESTAMP_NONE= -2, ZWSQL_TIMESTAMP_ERROR= -1,
  ZWSQL_TIMESTAMP_DATE= 0, ZWSQL_TIMESTAMP_DATETIME= 1, ZWSQL_TIMESTAMP_TIME= 2
};


/*
  Structure which is used to represent datetime values inside zwsql.

  We assume that values in this structure are normalized, i.e. year <= 9999,
  month <= 12, day <= 31, hour <= 23, hour <= 59, hour <= 59. Many functions
  in server such as my_system_gmt_sec() or make_time() family of functions
  rely on this (actually now usage of make_*() family relies on a bit weaker
  restriction). Also functions that produce ZWSQL_TIME as result ensure this.
  There is one exception to this rule though if this structure holds time
  value (time_type == ZWSQL_TIMESTAMP_TIME) days and hour member can hold
  bigger values.
*/
typedef struct st_zwsql_time
{
  unsigned int  year, month, day, hour, minute, second;
  unsigned long second_part;
  my_bool       neg;
  enum enum_zwsql_timestamp_type time_type;
} ZWSQL_TIME;

#endif /* _zwsql_time_h_ */
