/*

MIT License

Copyright (c) 2023 Hiro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
 
#ifndef VSD_DRIVER_H
# define VSD_DRIVER_H

# include <sys/types.h>

// structs
struct driver_status
{
	char	*status;
	int		errrno;
};

// common values
# define VSD_BLOCK_SIZE 1024 /* in bytes */
# define VSD "/home/hiro/projects/vfs/vsd-driver/vsd" /* vsd path/filename */

// i/o driver messages
# define OPEN_ERROR "DRIVER_OPEN_ERRNO"
# define READ_ERROR "DRIVER_READ_ERRNO"
# define WRITE_ERROR "DRIVER_WRITE_ERRNO"
# define LSEEK_ERROR "DRIVER_LSEEK_ERRNO"
# define CLOSE_ERROR "DRIVER_CLOSE_ERRNO"
# define OPEN_SUCCESS "DRIVER_OPEN_SUCCESS"
# define READ_SUCCESS "DRIVER_READ_SUCCESS"
# define WRITE_SUCCESS "DRIVER_WRITE_SUCCESS"
# define LSEEK_SUCCESS "DRIVER_LSEEK_SUCCESS"

// other driver messages
# define EXCEED_ERROR "DRIVER_WRITE_EXEEDED_BLOCK"
# define BINDEX_ERROR "DRIVER_READ_NEGATIVE_BLOCK"
# define OFFSET_ERROR "DRIVER_WRITE_NEGATIVE_OFFSET"
# define VREXCEED_ERROR "DRIVER_READ_EXCEEDS_VSD"
# define VWEXCEED_ERROR "DRIVER_WRITE_EXCEEDS_VSD"

// functions
void					read_block_to_buffer(int block_index);
char					*return_buffer();
off_t					return_vsd_size();
struct driver_status	return_driver_status();
void					write_to_block(int block_index, int offset, char *buf, int size);

#endif