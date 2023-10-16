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
 
#include "vsd-driver.h"
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

char					BUF[VSD_BLOCK_SIZE]; /* central read buffer */
struct driver_status	DRIVER_STATUS; /* latest driver call status */

void	initialize_driver_status()
{
	DRIVER_STATUS.status = (char*)malloc(sizeof(char) * 40);
	DRIVER_STATUS.errrno = 0;
}

int	handle(int fd, int val, char *err_msg, char *suc_msg, int type)
{
	if ((val < 0 && type == BASE) || (val && type == CONTROL)) /* if it's a BASE error or if it's a CONTROL error */
	{
		strcpy(DRIVER_STATUS.status, err_msg);
		DRIVER_STATUS.errrno = (type == BASE) ? errno : -1; /* the second condition is assumed to be type == CONTROL */
		if (close(fd) < 0)
		{
			DRIVER_STATUS.status = HCLOSE_ERROR;
			DRIVER_STATUS.errrno = errno;
		}
		return (0);
	}
	strcpy(DRIVER_STATUS.status, suc_msg);
	DRIVER_STATUS.errrno = 0;
	return (1);
}

/*

	writes buffer within a block at an offset (both 0 indexed)

*/
void	write_to_block(int block_index, int offset, char *buf, int size)
{
	int	fd;
	int	pos;

	fd = open(VSD, O_WRONLY);
	if (!handle(fd, fd, OPEN_ERROR, OPEN_SUCCESS, BASE))
		return;
	// if (fd == -1)
    // {
    //     DRIVER_STATUS.status = OPEN_ERROR;
    //     DRIVER_STATUS.errrno = errno;
	// 	return;
    // }
	else if (!handle(fd, block_index < 0, BINDEX_ERROR, BINDEX_SUCCESS, CONTROL))
		return;
	// if (block_index < 0)
	// {
	// 	DRIVER_STATUS.status = BINDEX_ERROR;
	// 	DRIVER_STATUS.errrno = -1;
	// 	return;
	// }
	else if (!handle(fd, offset < 0, OFFSET_ERROR, OFFSET_SUCCESS, CONTROL))
		return;
	// else if (offset < 0)
	// {
	// 	DRIVER_STATUS.status = OFFSET_ERROR;
	// 	DRIVER_STATUS.errrno = -1;
	// 	return;
	// }
	else if (!handle(fd, offset >= VSD_BLOCK_SIZE
		|| size > VSD_BLOCK_SIZE || size + offset > VSD_BLOCK_SIZE, EXCEED_ERROR, NEXCEED_SUCCESS, CONTROL))
		return;
	// else if (offset >= VSD_BLOCK_SIZE
	// 	|| size > VSD_BLOCK_SIZE || size + offset > VSD_BLOCK_SIZE)
	// {
	// 	DRIVER_STATUS.status = EXCEED_ERROR;
	// 	DRIVER_STATUS.errrno = -1;
	// 	return;
	// }
	pos = lseek(fd, block_index * VSD_BLOCK_SIZE + offset, SEEK_SET);
	if (!handle(fd, pos, LSEEK_ERROR, LSEEK_SUCCESS, BASE))
		return;
	// if (pos == -1)
	// {
	// 	if (close(fd) == -1)
	// 	{
	// 		DRIVER_STATUS.status = CLOSE_ERROR;
	// 		DRIVER_STATUS.errrno = errno;
	// 		return;
	// 	}
	// 	DRIVER_STATUS.status = LSEEK_ERROR;
	// 	DRIVER_STATUS.errrno = errno;
	// 	return;
	// }
	else if (!handle(fd, pos >= return_vsd_size(), VWEXCEED_ERROR, VWNEXCEED_SUCCESS, CONTROL))
		return;
	// else if (pos >= return_vsd_size())
	// {
	// 	if (close(fd) == -1)
	// 	{
	// 		DRIVER_STATUS.status = CLOSE_ERROR;
	// 		DRIVER_STATUS.errrno = errno;
	// 		return;
	// 	}
	// 	DRIVER_STATUS.status = VWEXCEED_ERROR;
	// 	DRIVER_STATUS.errrno = -1;
	// 	return;
	// }
	else if (!handle(fd, write(fd, buf, size), WRITE_ERROR, WRITE_SUCCESS, BASE))
		return;
	// if (write(fd, buf, size) == -1)
	// {
	// 	if (close(fd) == -1)
	// 	{
	// 		DRIVER_STATUS.status = CLOSE_ERROR;
	// 		DRIVER_STATUS.errrno = errno;
	// 		return;
	// 	}
	// 	DRIVER_STATUS.status = WRITE_ERROR;
	// 	DRIVER_STATUS.errrno = errno;
	// 	return;
	// }
	else if (!handle(fd, close(fd), CLOSE_ERROR, CLOSE_SUCCESS, BASE))
		return;
	// if (close(fd) == -1)
	// {
	// 	DRIVER_STATUS.status = CLOSE_ERROR;
	// 	DRIVER_STATUS.errrno = errno;
	// 	return;
	// }
	// DRIVER_STATUS.status = WRITE_SUCCESS;
	// DRIVER_STATUS.errrno = 0;
}

struct driver_status	return_driver_status()
{
	return (DRIVER_STATUS);
}

/*

	returns the size in bytes of the VSD

*/
off_t	return_vsd_size()
{
	int		fd;
	off_t	size;

	fd = open(VSD, O_RDONLY);
	if (!handle(fd, fd, OPEN_ERROR, OPEN_SUCCESS, BASE))
		return (-1);
	// if (fd == -1)
    // {
    //     DRIVER_STATUS.status = OPEN_ERROR;
    //     DRIVER_STATUS.errrno = errno;
	// 	return (-1);
    // }
	size = lseek(fd, 0, SEEK_END);
	if (!handle(fd, size, LSEEK_ERROR, LSEEK_SUCCESS, BASE))
		return (-1);
	// if (size == -1)
	// {
	// 	if (close(fd) == -1)
	// 	{
	// 		DRIVER_STATUS.status = CLOSE_ERROR;
	// 		DRIVER_STATUS.errrno = errno;
	// 		return (-1);
	// 	}
	// 	DRIVER_STATUS.status = LSEEK_ERROR;
	// 	DRIVER_STATUS.errrno = errno;
	// 	return (-1);
	// }
	else if (!handle(fd, close(fd), CLOSE_ERROR, CLOSE_SUCCESS, BASE))
		return (-1);
	// if (close(fd) == -1)
	// {
	// 	DRIVER_STATUS.status = CLOSE_ERROR;
	// 	DRIVER_STATUS.errrno = errno;
	// 	return (-1);
	// }
	// DRIVER_STATUS.status = READ_SUCCESS;
	// DRIVER_STATUS.errrno = 0;
	return (size);
}

/*

	returns the central buffer

*/
char	*return_buffer()
{
	return (BUF);
}

/*

	reads a block from VSD (zero indexed) into the central buffer

*/
void	read_block_to_buffer(int block_index)
{
    int     fd;
	int		pos;

	fd = open(VSD, O_RDONLY);
	if (!handle(fd, fd, OPEN_ERROR, OPEN_SUCCESS, BASE))
		return;
	// if (fd == -1)
    // {
    //     DRIVER_STATUS.status = OPEN_ERROR;
    //     DRIVER_STATUS.errrno = errno;
	// 	return;
    // }
	else if (!handle(fd, block_index < 0, BINDEX_ERROR, BINDEX_SUCCESS, CONTROL))
		return;
	// if (block_index < 0)
	// {
	// 	DRIVER_STATUS.status = BINDEX_ERROR;
	// 	DRIVER_STATUS.errrno = -1;
	// 	return;
	// }
	pos = lseek(fd, block_index * VSD_BLOCK_SIZE, SEEK_SET);
	if (!handle(fd, pos, LSEEK_ERROR, LSEEK_SUCCESS, BASE))
		return;
	// if (pos == -1)
	// {
	// 	if (close(fd) == -1)
	// 	{
	// 		DRIVER_STATUS.status = CLOSE_ERROR;
	// 		DRIVER_STATUS.errrno = errno;
	// 		return;
	// 	}
	// 	DRIVER_STATUS.status = LSEEK_ERROR;
	// 	DRIVER_STATUS.errrno = errno;
	// 	return;
	// }
	else if (!handle(fd, pos >= return_vsd_size(), VREXCEED_ERROR, VRNEXCEED_SUCCESS, CONTROL))
		return;
	// else if (pos >= return_vsd_size())
	// {
	// 	if (close(fd) == -1)
	// 	{
	// 		DRIVER_STATUS.status = CLOSE_ERROR;
	// 		DRIVER_STATUS.errrno = errno;
	// 		return;
	// 	}
	// 	DRIVER_STATUS.status = VREXCEED_ERROR;
	// 	DRIVER_STATUS.errrno = -1;
	// 	return;
	// }
	else if (!handle(fd, read(fd, BUF, VSD_BLOCK_SIZE), READ_ERROR, READ_SUCCESS, BASE))
		return;
    // if (read(fd, BUF, VSD_BLOCK_SIZE) == -1)
    // {
    //     if (close(fd) == -1)
	// 	{
	// 		DRIVER_STATUS.status = CLOSE_ERROR;
	// 		DRIVER_STATUS.errrno = errno;
	// 		return;
	// 	}
    //     DRIVER_STATUS.status = READ_ERROR;
    //     DRIVER_STATUS.errrno = errno;
	// 	return;
    // }
	// DRIVER_STATUS.status = READ_SUCCESS;
	// DRIVER_STATUS.errrno = 0;
	else if (!handle(fd, close(fd), CLOSE_ERROR, CLOSE_SUCCESS, BASE))
		return;
    // if (close(fd) == -1)
	// {
	// 	DRIVER_STATUS.status = CLOSE_ERROR;
	// 	DRIVER_STATUS.errrno = errno;
	// }
}