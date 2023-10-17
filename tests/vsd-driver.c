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
#include <assert.h>
#include <string.h>
#include <stdio.h>

char	BUFF[1000];
int		INDEX = 0;

void    print(sll_t *node)
{
    printf("%s\n", node->payload);
}

void	fill_test_buffer(sll_t *node)
{
	strcpy(BUFF + INDEX, node->payload);
	INDEX += strlen(node->payload);
}

/*

    tests for the VSD driver

*/

# define READ_0 "DRIVER_OPEN_SUCCESS0DRIVER_READ_VALID_BLOCK_INDEX_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_OPEN_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_CLOSE_SUCCESS0DRIVER_READ_WITHIN_VSD_SUCCESS0DRIVER_READ_SUCCESS0DRIVER_CLOSE_SUCCESS0"
# define READ_1 "DRIVER_OPEN_SUCCESS0DRIVER_READ_NEGATIVE_BLOCK_INDEX_ERROR-1DRIVER_CLOSE_SUCCESS0"

int main(void)
{
	/* READ_0 test */
    initialize_driver_status();
    read_block_to_buffer(0);
	traverse(return_driver_status_log(), fill_test_buffer);
	assert(strcmp(BUFF, READ_0) == 0);
    free_driver_status();

	memset(BUFF, 0, 1000);
	INDEX = 0;

	/* READ_1 test */
	initialize_driver_status();
    read_block_to_buffer(-1);
	traverse(return_driver_status_log(), fill_test_buffer);
	assert(strcmp(BUFF, READ_1) == 0);
    free_driver_status();

	/* READ_2 test */
	initialize_driver_status();
    read_block_to_buffer(1048574);
	traverse(return_driver_status_log(), print);
	// assert(strcmp(BUFF, READ_2) == 0);
    free_driver_status();

    return (0);
}