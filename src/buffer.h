/*
   The SNL (Simple Network Layer) provides a neat C API for network programming.
   Copyright (C) 2001, 2002, 2013 Clemens Kirchgatterer <clemens@1541.org>

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _SNL_BUFFER_H_
#define _SNL_BUFFER_H_

typedef struct snl_buffer {
	void *data;
	unsigned int used;
	unsigned int size;
} snl_buffer;

snl_buffer *snl_buffer_new(void);
void snl_buffer_delete(snl_buffer *buf);
void snl_buffer_clear(snl_buffer *buf);
int snl_buffer_append(snl_buffer *buf, const void *data, unsigned int len);

#endif // _SNL_BUFFER_H_
