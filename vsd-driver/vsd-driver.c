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
#include "singly-linked-list.h"
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char					BUF[VSD_BLOCK_SIZE]; /* central read buffer */
struct driver_status	DRIVER_STATUS; /* latest driver call status */
sll_headnode_t			DRIVER_STATUS_LOG; /* singly linked list that holds DRIVER_STATUS logs in FIFO order */

sll_headnode_t	*return_driver_status_log()
{
	return (&DRIVER_STATUS_LOG);
}

void	initialize_driver_status()
{
	DRIVER_STATUS_LOG = new_list();
	DRIVER_STATUS.status = (char*)malloc(sizeof(char) * 50);
	if (!DRIVER_STATUS.status)
	{
		DRIVER_STATUS.errrno = -1;
		return;
	}
	DRIVER_STATUS.errrno = 0;
}

void	free_driver_status()
{
	free_sll(&DRIVER_STATUS_LOG);
	free(DRIVER_STATUS.status);
}

/*

	update DRIVER_STATUS as well as return 0 or 1 on fail/success
	here BASE just means messages that describe errors from POSIX functions such as read, write etc...
	while CONTROL means messages that are made up for controlling what would be legal write or legal read
	to VSD

*/
int	handle(int fd, int val, char *err_msg, char *suc_msg, int type)
{
	char	buf[3];

	if ((val < 0 && type == BASE) || (val && type == CONTROL)) /* if it's a BASE error or if it's a CONTROL error */
	{
		strcpy(DRIVER_STATUS.status, err_msg);
		DRIVER_STATUS.errrno = (type == BASE) ? errno : -1; /* the second condition is assumed to be type == CONTROL */
		append(&DRIVER_STATUS_LOG, DRIVER_STATUS.status);
		sprintf(buf, "%d", DRIVER_STATUS.errrno);
		append(&DRIVER_STATUS_LOG, buf);
		if (close(fd) < 0)
		{
			strcpy(DRIVER_STATUS.status, HCLOSE_ERROR);
			DRIVER_STATUS.errrno = errno;
			append(&DRIVER_STATUS_LOG, DRIVER_STATUS.status);
			sprintf(buf, "%d", DRIVER_STATUS.errrno);
			append(&DRIVER_STATUS_LOG, buf);
		}
		else
		{
			strcpy(DRIVER_STATUS.status, CLOSE_SUCCESS);
			DRIVER_STATUS.errrno = 0;
			append(&DRIVER_STATUS_LOG, DRIVER_STATUS.status);
			sprintf(buf, "%d", DRIVER_STATUS.errrno);
			append(&DRIVER_STATUS_LOG, buf);
		}
		return (0);
	}
	strcpy(DRIVER_STATUS.status, suc_msg);
	DRIVER_STATUS.errrno = 0;
	append(&DRIVER_STATUS_LOG, DRIVER_STATUS.status);
	sprintf(buf, "%d", DRIVER_STATUS.errrno);
	append(&DRIVER_STATUS_LOG, buf);
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
	else if (!handle(fd, block_index < 0, WBINDEX_ERROR, WBINDEX_SUCCESS, CONTROL))
		return;
	else if (!handle(fd, offset < 0, OFFSET_ERROR, OFFSET_SUCCESS, CONTROL))
		return;
	else if (!handle(fd, offset >= VSD_BLOCK_SIZE
		|| size > VSD_BLOCK_SIZE || size + offset > VSD_BLOCK_SIZE, BEXCEED_ERROR, NEXCEED_SUCCESS, CONTROL))
		return;
	pos = lseek(fd, block_index * VSD_BLOCK_SIZE + offset, SEEK_SET);
	if (!handle(fd, pos, LSEEK_ERROR, LSEEK_SUCCESS, BASE))
		return;
	else if (!handle(fd, pos > (return_vsd_size() - VSD_BLOCK_SIZE - 1), VWEXCEED_ERROR, VWNEXCEED_SUCCESS, CONTROL))
		return;
	else if (!handle(fd, write(fd, buf, size), WRITE_ERROR, WRITE_SUCCESS, BASE))
		return;
	else if (!handle(fd, close(fd), CLOSE_ERROR, CLOSE_SUCCESS, BASE))
		return;
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
	size = lseek(fd, 0, SEEK_END);
	if (!handle(fd, size, LSEEK_ERROR, LSEEK_SUCCESS, BASE))
		return (-1);
	else if (!handle(fd, close(fd), CLOSE_ERROR, CLOSE_SUCCESS, BASE))
		return (-1);
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
	else if (!handle(fd, block_index < 0, RBINDEX_ERROR, RBINDEX_SUCCESS, CONTROL))
		return;
	pos = lseek(fd, block_index * VSD_BLOCK_SIZE, SEEK_SET);
	if (!handle(fd, pos, LSEEK_ERROR, LSEEK_SUCCESS, BASE))
		return;
	else if (!handle(fd, pos > (return_vsd_size() - VSD_BLOCK_SIZE - 1), VREXCEED_ERROR, VRNEXCEED_SUCCESS, CONTROL))
		return;
	else if (!handle(fd, read(fd, BUF, VSD_BLOCK_SIZE), READ_ERROR, READ_SUCCESS, BASE))
		return;
	else if (!handle(fd, close(fd), CLOSE_ERROR, CLOSE_SUCCESS, BASE))
		return;
}