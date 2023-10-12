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

char					BUF[VSD_BLOCK_SIZE]; /* stores VSD block reads */
struct driver_status	DRIVER_STATUS; /* latest driver call status */

struct driver_status	return_driver_status()
{
	return (DRIVER_STATUS);
}

off_t	return_vsd_size()
{
	int		fd;
	off_t	size;

	fd = open(VSD, O_RDONLY);
	if (fd == -1)
    {
        if (close(fd) == -1)
		{
			DRIVER_STATUS.status = CLOSE_ERROR;
			DRIVER_STATUS.errrno = errno;
			return (-1);
		}
        DRIVER_STATUS.status = OPEN_ERROR;
        DRIVER_STATUS.errrno = errno;
		return (-1);
    }
	size = lseek(fd, 0, SEEK_END);
	if (size == -1)
	{
		if (close(fd) == -1)
		{
			DRIVER_STATUS.status = CLOSE_ERROR;
			DRIVER_STATUS.errrno = errno;
			return (-1);
		}
		DRIVER_STATUS.status = LSEEK_ERROR;
		DRIVER_STATUS.errrno = errno;
		return (-1);
	}
	if (close(fd) == -1)
	{
		DRIVER_STATUS.status = CLOSE_ERROR;
		DRIVER_STATUS.errrno = errno;
		return (-1);
	}
	DRIVER_STATUS.status = READ_SUCCESS;
	DRIVER_STATUS.errrno = 0;
	return (size);
}

char	*return_buffer()
{
	return (BUF);
}

void	read_block(unsigned int block_index)
{
    int     fd;

    fd = open(VSD, O_RDONLY);
	if (fd == -1)
    {
        if (close(fd) == -1)
		{
			DRIVER_STATUS.status = CLOSE_ERROR;
			DRIVER_STATUS.errrno = errno;
			return;
		}
        DRIVER_STATUS.status = OPEN_ERROR;
        DRIVER_STATUS.errrno = errno;
		return;
    }
	if (lseek(fd, block_index * VSD_BLOCK_SIZE, SEEK_SET) == -1)
	{
		if (close(fd) == -1)
		{
			DRIVER_STATUS.status = CLOSE_ERROR;
			DRIVER_STATUS.errrno = errno;
			return;
		}
		DRIVER_STATUS.status = LSEEK_ERROR;
		DRIVER_STATUS.errrno = errno;
		return;
	}
    if (read(fd, BUF, VSD_BLOCK_SIZE) == -1)
    {
        if (close(fd) == -1)
		{
			DRIVER_STATUS.status = CLOSE_ERROR;
			DRIVER_STATUS.errrno = errno;
			return;
		}
        DRIVER_STATUS.status = READ_ERROR;
        DRIVER_STATUS.errrno = errno;
		return;
    }
	DRIVER_STATUS.status = READ_SUCCESS;
	DRIVER_STATUS.errrno = errno;
    if (close(fd) == -1)
	{
		DRIVER_STATUS.status = CLOSE_ERROR;
		DRIVER_STATUS.errrno = errno;
		return;
	}
	DRIVER_STATUS.status = READ_SUCCESS;
	DRIVER_STATUS.errrno = 0;
}