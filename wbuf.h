/*
 * Module of window buffer based on pointers.
 * It is made completely with macros, so it can be used with any data type.
 *
 * It is used to read elements from cycle array like from regular array by
 * element number. For example, to read cell with number 4, like from regular
 * static array, just write: wbuf_read(buf, 4, read_element);
 */

//------------------------------------------------------------------------------
// initialization and properties
//------------------------------------------------------------------------------

/*
 * Template to declare new wbuffer. It only declare.
 * To init you should use fifo_init macros.
 * wr - pointer on last putted element (write)
 * top - pointer on the last cell of buffer
 * bottom - pointer on the first cell of buffer
 */
#define WBUF_DECLARE(TYPE, SIZE, NAME)\
struct\
{\
	TYPE* wr;\
	TYPE* top;\
	TYPE* bottom;\
	TYPE array[SIZE];\
} NAME;

/*
 * initialization of wbuffer
 */
#define wbuf_init(BUF)\
do {\
	(BUF).bottom = (BUF).array;\
	(BUF).top = (BUF).bottom+(wbuf_size(BUF)-1);\
	(BUF).wr = (BUF).top;\
} while (0)

/*
 * get entire size of wbuffer
 */
#define wbuf_size(BUF)				(sizeof(BUF.array)/sizeof(BUF.array[0]))

/*
 * write cell into wbuffer without overflow check
 */
#define wbuf_put(BUF, CELL)\
do {\
	(BUF).wr++;\
	if ((BUF).wr > (BUF).top)\
		(BUF).wr = (BUF).bottom;\
	*(BUF).wr = (CELL);\
} while (0)

/*
 * read given cell from wbuffer
 * CELL_NUM - number of element just like in regular
 */
#define wbuf_read(BUF, CELL_NUM, CELL)\
do {\
	typeof(CELL) *tmp = ((BUF).wr + (CELL_NUM) + 1)\
	if (tmp > ((BUF).top)) {\
		(CELL) = *(tmp - (BUF).top + (BUF).bottom);\
	} else {\
		(CELL) = *tmp;\
	}\
} while (0)

/*
 * get pointer to the last written cell
 */
#define wbuf_getlast(BUF, CELLP)	(CELLP) = ((BUF).wr)

/*
 * get pointer to the next cell.
 * The pointer is incremented, so it have to be written
 */
#define wbuf_getnext(BUF, CELLP)\
do {\
	(BUF).wr++;\
	if ((BUF).wr > (BUF).top)\
		(BUF).wr = (BUF).bottom;\
	(CELLP) = (BUF).wr;\
} while (0)

