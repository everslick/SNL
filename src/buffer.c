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

#include <stdlib.h>
#include <string.h>

#include "buffer.h"

#define INIT_SIZE 64

snl_buffer *
snl_buffer_new(void) {
   snl_buffer *buf = (snl_buffer *)malloc(sizeof (snl_buffer));

	if (buf == NULL) return (NULL);

   memset(buf, 0, sizeof (snl_buffer));

   if (buf->data == NULL) {
      buf->data = malloc(INIT_SIZE);
      buf->size = INIT_SIZE;
   }

	return (buf);
}

void
snl_buffer_delete(snl_buffer *buf) {
	if (buf == NULL) return;

	free(buf->data);
   free(buf);
}

int
snl_buffer_append(snl_buffer *buf, const void *data, unsigned int len) {
	if (buf == NULL) return (-1);

   if ((buf->used + len + 1) > buf->size) {
      buf->size = (buf->used + len) * 2;
      buf->data = realloc(buf->data, buf->size);

      if (buf->data == NULL) return (-1);
   }

   memcpy(buf->data + buf->used, data, len);
   buf->used += len;

	return (0);
}

void
snl_buffer_clear(snl_buffer *buf) {
	if (buf == NULL) return;

	buf->used = 0;
}
