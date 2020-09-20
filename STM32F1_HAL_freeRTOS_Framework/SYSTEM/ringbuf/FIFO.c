/*	Generic FIFO buffer Implementation
	Copyright (C) 2014 Jesus Ruben Santa Anna Zamudio.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Author website: http://geekfactory.mx
	Author e-mail: ruben at geekfactory dot mx
 */

#include "FIFO.h"

/*-------------------------------------------------------------*
 *		Private function prototypes			*
 *-------------------------------------------------------------*/
static void fifo_copy_from(fifo_t, void *);
static void fifo_copy_to(fifo_t, const void *);

/*-------------------------------------------------------------*
 *		Public API implementation			*
 *-------------------------------------------------------------*/
fifo_t fifo_create(uint16_t count, size_t size)
{
	fifo_t newfifo;
	if (count > 0) {
		newfifo = (struct fifo_descriptor *) malloc(sizeof(struct fifo_descriptor));
		if (newfifo != NULL) {
			// Calculate the size in bytes of the buffer
			size_t bsize = count * size;
			// Try to allocate space for the buffer data
			newfifo->itemspace = malloc(bsize);
			if (newfifo->itemspace != NULL) {
				// Initialize structure members
				newfifo->itemsize = size;
				newfifo->allocatedbytes = bsize;
				newfifo->readoffset = 0;
				newfifo->writeoffset = 0;
				newfifo->storedbytes = 0;
				// Return the pointer to fifo_descriptor structure
				return newfifo;
			} else {
				// Cannot allocate space for items, free struct resources
				free(newfifo);
			}
		}
	}
	// Return NULL if something fails
	return NULL;
}

fifo_t fifo_create_static(fifo_t fifo, void * buf, uint16_t count, size_t size)
{
	// Sanity check for memory and element sizes
	if (buf != NULL && fifo != NULL && count != 0) {
		fifo->itemspace = buf;
		fifo->itemsize = size;
		fifo->allocatedbytes = count * size;
		fifo->readoffset = 0;
		fifo->writeoffset = 0;
		fifo->storedbytes = 0;
		return fifo;
	}
	return NULL;
}

bool fifo_add(fifo_t fifo, const void * item)
{
	if (!fifo_is_full(fifo)) {
		fifo_copy_to(fifo, item);
		fifo->storedbytes += fifo->itemsize;
		return true;
	} else {
		return false;
	}
}

bool fifo_get(fifo_t fifo, void * item)
{
	if (!fifo_is_empty(fifo)) {
		fifo_copy_from(fifo, item);
		fifo->storedbytes -= fifo->itemsize;
		return true;
	} else {
		return false;
	}
}

bool fifo_is_full(fifo_t fifo)
{
	if (fifo->storedbytes >= fifo->allocatedbytes)
		return true;
	else
		return false;
}

bool fifo_is_empty(fifo_t fifo)
{
	if (fifo->storedbytes == 0)
		return true;
	else
		return false;
}

bool fifo_discard(fifo_t fifo, uint16_t count, enum fifo_side side)
{
	uint16_t t;
	t = fifo->itemsize * count; // Compute byte size of elements to be deleted
	if (t <= fifo->storedbytes) // Check if we can remove the requested ammount of data
	{
		if (side == E_FIFO_FRONT) {
			fifo->readoffset = (fifo->readoffset + t) % fifo->allocatedbytes; // Increase read pointer n elements
			fifo->storedbytes -= t; // Decrease stored bytes number
		} else if (side == E_FIFO_BACK) {
			fifo->writeoffset = (fifo->writeoffset - t) % fifo->allocatedbytes; // Decrease write pointer n elements
			fifo->storedbytes -= t; // Decrease stored bytes number
		}
		return true;
	}
	return false;
}

static void fifo_copy_from(fifo_t fifo, void * item)
{  
	mymemcpy(item, ((uint16_t*)fifo->itemspace) + fifo->readoffset, fifo->itemsize);
	fifo->readoffset += fifo->itemsize;
	if (fifo->readoffset >= fifo->allocatedbytes) {
		fifo->readoffset = 0;
	}
}

static void fifo_copy_to(fifo_t fifo, const void *item)
{
	mymemcpy(((uint16_t*)fifo->itemspace) + fifo->writeoffset, item, fifo->itemsize);
	fifo->writeoffset += fifo->itemsize;
	if (fifo->writeoffset >= fifo->allocatedbytes) {
		fifo->writeoffset = 0;
	}
}


