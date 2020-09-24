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

/**
 * Generic FIFO buffer Implementation.
 * 
 * Source code for the implementation of a FIFO buffer. The fifo can contain any
 * type of data, including but not limited to: integers, floats, structures,
 * arrays, etc.
 * 
 * This data structure can be used on a variety of situations, for example:
 * It can be used with microcontroller peripherals for buffering, as a message
 * or event queue, can be used for task synchronization or comunications 
 * and more.
 */

#ifndef FIFO_H
#define FIFO_H
/*-------------------------------------------------------------*
 *		Includes and dependencies			*
 *-------------------------------------------------------------*/
#include "MyString.h"
#include <stdint.h>
#include <stdbool.h>

/*-------------------------------------------------------------*
 *		Macros and definitions				*
 *-------------------------------------------------------------*/

#define FIFO_VERSION_STRING			"1.0.0"

/*-------------------------------------------------------------*
 *		Typedefs enums & structs			*
 *-------------------------------------------------------------*/

/**
 * This structure holds the data that denfines a fifo buffer, and pointers
 * to access data on the buffer
 */
struct fifo_descriptor {
	/**
	 * Start address on memory for item space.
	 */
	void * itemspace;
	/** 
	 * Size in bytes for each element in the buffer.
	 */
	size_t itemsize;
	/**
	 * Size in bytes of the whole buffer.
	 */
	size_t allocatedbytes;
	/**
	 * Memory offset from which we will read data.
	 */
	volatile uint16_t readoffset;
	/**
	 * Memory offset where data will be written.
	 */
	volatile uint16_t writeoffset;
	/**
	¨* Number of bytes used currently by stored items.
	 */
	volatile size_t storedbytes;
};

/**
 * Defines the side of the buffer to perform some actions
 */
enum fifo_side {
	E_FIFO_FRONT,
	E_FIFO_BACK,
};

/**
 * Definition for the fifo_t type, which is a pointer to a fifo_data struct
 */
typedef struct fifo_descriptor * fifo_t;

/*-------------------------------------------------------------*
 *		Function prototypes				*
 *-------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
	/**
	 * @brief Creates a FIFO using dynamic memory
	 *
	 * This function is used to create a buffer, it allocates memory for a buffer of
	 * the requested size plus the size of the structure that contains the
	 * information requeried by other API functions to access that buffer.
	 *
	 * @param count The number of elements the buffer should be able to store
	 * @param size The size in bytes for each element to be stored in the buffer
	 *
	 * @return If a buffer is succesfully created, returns a pointer to the
	 * structure that contains the buffer information (fifo_t). NULL is returned if
	 * something fails.
	 */
	fifo_t fifo_create(uint16_t count, size_t size);

	/**
	 * @brief Creates a statically allocated FIFO buffer
	 *
	 * This function is similar to fifo_create() but it difers from the last because
	 * the memory allocation is static. A pointer to the buffer space and a pointer
	 * to the control structure are required, both of them shoud be allocated 
	 * statically by the programmer.
	 *
	 * @param fifo The pointer to the structure holding FIFO data
	 * @param buf Pointer to the memory used to store actual fifo items
	 * @param count The number of items to store on the FIFO
	 * @param size The size in bytes of each item on the buffer
	 *
	 * @return This function performs some basic validation on the parameters passed,
	 * if something is wrong with them,it will return NULL.
	 */
	fifo_t fifo_create_static(fifo_t fifo, void * buf, uint16_t count, size_t size);

	/**
	 * @brief Adds one item to the FIFO buffer
	 *
	 * This function writes an item to the fifo buffer back. This function affects
	 * the write pointer and the stored items counter.
	 *
	 * The number of bytes to be copied to the fifo buffer was defined when the
	 * fifo buffer was created with the function fifo_create() (size parameter).
	 *
	 * @param fifo Pointer to a fifo_descriptor structure.
	 * @param item Pointer to a location that holds data to be written to the fifo
	 * buffer.
	 * 
	 * @return  Returns true if there is space in the FIFO to add the item. If the
	 * buffer is full and no data can be copied it returns false.
	 */
	bool fifo_add(fifo_t fifo, const void * item);

	/**
	 * @brief Obtains one item from the FIFO buffer.
	 *
	 * This function reads an item from the fifo buffer front. This function affects
	 * the read pointer and the stored items counter.
	 * 
	 * The number of bytes to be copied to the provided buffer was defined when the
	 * fifo buffer was created with the function fifo_create() (size parameter).
	 * 
	 * @param fifo Pointer to a fifo_descriptor structure.
	 * @param item Pointer to a location to hold the data read from the fifo buffer,
	 * this buffer should be sized appropiately to accomodate the data.
	 * 
	 * @return  Returns true if there is data available on the fifo buffer to be
	 * copied, if the buffer is empty and no data can be read this returns false.
	 */
	bool fifo_get(fifo_t fifo, void * item);

	/**
	 * @brief Checks if the FIFO is full.
	 *
	 * Check if it can accept one item at least.
	 *
	 * @param fifo Pointer to a fifo_descriptor structure.
	 *
	 * @return This function returns true if the buffer is full, false otherwise.
	 */
	bool fifo_is_full(fifo_t fifo);

	/**
	 * @brief Checks if the FIFO is empty.
	 *
	 * Check if the buffer has no data stored in it.
	 *
	 * @param fifo Pointer to a fifo_descriptor structure.
	 *
	 * @return This function returns true if the buffer is empty, false otherwise.
	 */
	bool fifo_is_empty(fifo_t fifo);

	/**
	 * @brief Discard data from the buffer.
	 *
	 * This function discards data from the back or the front side of the buffer,
	 * the side and the ammount of discarded data depends on the parameters passed.
	 *
	 * @param fifo Pointer to a fifo_descriptor structure.
	 * @param count The number of elements to discard from the buffer.
	 * @param side Defines if data should be discarted from the front or back side
	 * of the buffer.
	 * 
	 * @return Returns true if the data was discarted, false if not.
	 */
	bool fifo_discard(fifo_t fifo, uint16_t count, enum fifo_side side);

#ifdef __cplusplus
}
#endif

#endif

/*参考程序，其中，可存储的数据格式可以为各种字符型、整形、浮点型、结构体型等等*/
/*

  char data[] = "Hola Mundo!!!!";

  fifo_t myfifo;

  myfifo = fifo_create(10, sizeof(char));

  // check if FIFO was created
  if (myfifo == NULL) {
    Serial.println(F("Cannot create FIFO... halting!"));
    for (;;);
  } else {
    Serial.println(F("FIFO created successfully"));
  }
  
  
  for (unsigned int i = 0; i < sizeof(data); i++)
  {
    Serial.print(F("Add item to FIFO: "));
    Serial.print(data[i]);
    if (fifo_add(myfifo, &data[i])) {
      Serial.println(F(" (OK)"));
    } else {
      Serial.println(F(" (FAIL)"));
    }
  }
  
  while (!fifo_is_empty(myfifo)) {
    char c;
    fifo_get(myfifo, &c);
    Serial.print(c);
  }
  
  
*/
  



// End of Header file
