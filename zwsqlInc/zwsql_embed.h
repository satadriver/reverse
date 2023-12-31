/* Copyright (C) 2000 zwsql AB

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

/* Defines that are unique to the embedded version of zwsql */

#ifdef EMBEDDED_LIBRARY

/* Things we don't need in the embedded version of zwsql */
/* TODO HF add #undef HAVE_VIO if we don't want client in embedded library */

#undef HAVE_PSTACK				/* No stacktrace */
#undef HAVE_DLOPEN				/* No udf functions */
#undef HAVE_OPENSSL
#undef HAVE_SMEM				/* No shared memory */
#undef HAVE_NDBCLUSTER_DB /* No NDB cluster */

#define DONT_USE_RAID

#endif /* EMBEDDED_LIBRARY */
