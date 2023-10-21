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

char	BUFF[300];
int		INDEX = 0;

// void    print(sll_t *node)
// {
//     printf("%s\n", node->payload);
// }

void	fill_test_buffer(sll_t *node)
{
	strcpy(BUFF + INDEX, node->payload);
	INDEX += strlen(node->payload);
}

/*

    tests for the VSD driver

*/
# define TEST_STRING "fd3ucUlSXmCYhU53RgqS5nN6weqAx5CUwLM88rXyakPcdb0Y11tyDxkcFdrzCaXMj3FLZS4xzpUZSn6x4mVCVt6OzOqSsu6M28Fl4BweLMe4b83WXnpCM4cLcQ43xMNcR7zXPZ3n4LIsp4kG6ewOynN2EET5cXzkaLWjTX9Xz5WOos89KXgA8aKKkoAunOJ4AhGBy5NVNhrDNBfpVJsQze57OzbOBfihXoUJjaJR6TJmHioxKxf6rfhZFMoSvjok2OF9BKbmOmAvX0FyzrzViuT8px66m5HhV5r12SMbxLH56LCHuLaEerZhfnqlFzgoamEETS9ZhzrLHazebEmrYQIDwcT6idQkjiqmzzuZGIqOvJTN9Nm5wgYRK6Qpi5u0wGgXFSzzR96d1srtAfkeuIQtcNXAED470bviPE0PGob7Gq1XBVLs35AjuBoM10dQKCRmnk9pfEMaJoKDxOgid5YBY1aRQznSblXPmG1vvYhYOata2hds4ZrsmDPPGm7MviM1yzAtIkqW8OfY3Ff1VNK91E6k488icwAzDdodaFOMoPyXqKwlR7j9eiZ2oJ26m1dIKYyM6Ds3jzWqnvLGZLMw21BONc2voMm44fdSxzMLbMjjBLk0JJxzM7zXiniH11mDOeVAmRLe0RiMpANIkDPKG02g89AhjNjQwUVnbIhMpkWOgKcqELARrNeb93YhzteXwbTYHrHTZrHLR1oN2LdnxApetJmaxxL7mjzLJ6jYuXdH4O1mOZaX83XKew3XS0vcNV1fd4Iu2SFR5ygR08pQu7mI39Gjqjeyu7Qzf5fgHKx8CeiwOvL8fhux7Uquod4ycHIqCC9PkyfujTNruFmb3eHhlKe1FftFNF142YJFuPlG3Dyez4zzeSAp49opjKJ8V4tFXNvHwU5F8xFVmOgezSYfkCA2IbrPlzhFdNNkCr9LiBVpz2CEUWa6JBySGrnDNXCmZNUmF8cgcurOoQhZOn8ZmJL5qNK28eDSBM301pz0Bty2pSJ5PUfwHX"

# define VSIZE "DRIVER_OPEN_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_CLOSE_SUCCESS0"

# define READ_0 "DRIVER_OPEN_SUCCESS0DRIVER_READ_VALID_BLOCK_INDEX_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_OPEN_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_CLOSE_SUCCESS0DRIVER_READ_WITHIN_VSD_SUCCESS0DRIVER_READ_SUCCESS0DRIVER_CLOSE_SUCCESS0"
# define READ_1 "DRIVER_OPEN_SUCCESS0DRIVER_READ_NEGATIVE_BLOCK_INDEX_ERROR-1DRIVER_CLOSE_SUCCESS0"
# define READ_2 "DRIVER_OPEN_SUCCESS0DRIVER_READ_VALID_BLOCK_INDEX_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_OPEN_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_CLOSE_SUCCESS0DRIVER_READ_EXCEEDS_VSD_ERROR-1DRIVER_CLOSE_SUCCESS0"

# define WRITE_0 "DRIVER_OPEN_SUCCESS0DRIVER_WRITE_VALID_BLOCK_INDEX_SUCCESS0DRIVER_WRITE_VALID_OFFSET_SUCCESS0DRIVER_WRITE_WITHIN_BLOCK_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_OPEN_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_CLOSE_SUCCESS0DRIVER_WRITE_WITHIN_VSD_SUCCESS0DRIVER_WRITE_SUCCESS0DRIVER_CLOSE_SUCCESS0"
# define WRITE_1 "DRIVER_OPEN_SUCCESS0DRIVER_WRITE_NEGATIVE_BLOCK_INDEX_ERROR-1DRIVER_CLOSE_SUCCESS0"
# define WRITE_2 "DRIVER_OPEN_SUCCESS0DRIVER_WRITE_VALID_BLOCK_INDEX_SUCCESS0DRIVER_WRITE_NEGATIVE_OFFSET_ERROR-1DRIVER_CLOSE_SUCCESS0"
# define WRITE_3 "DRIVER_OPEN_SUCCESS0DRIVER_WRITE_VALID_BLOCK_INDEX_SUCCESS0DRIVER_WRITE_VALID_OFFSET_SUCCESS0DRIVER_WRITE_WITHIN_BLOCK_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_OPEN_SUCCESS0DRIVER_LSEEK_SUCCESS0DRIVER_CLOSE_SUCCESS0DRIVER_WRITE_EXCEEDS_VSD_ERROR-1DRIVER_CLOSE_SUCCESS0"
# define WRITE_4 "DRIVER_OPEN_SUCCESS0DRIVER_WRITE_VALID_BLOCK_INDEX_SUCCESS0DRIVER_WRITE_VALID_OFFSET_SUCCESS0DRIVER_WRITE_EXEEDED_BLOCK_ERROR-1DRIVER_CLOSE_SUCCESS0"

int main(void)
{
	/* return_vsd_size() */
	initialize_driver_status();
	return_vsd_size();
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, VSIZE) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* read_block_to_buffer() */

	/* READ_0 test MIN */
    initialize_driver_status();
    read_block_to_buffer(0);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, READ_0) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* READ_1 test */
	initialize_driver_status();
    read_block_to_buffer(-1);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, READ_1) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* READ_0 test MAX */
	initialize_driver_status();
    read_block_to_buffer(1048574); /* this is the max block index that can be read/written */
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, READ_0) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* READ_2 */
	initialize_driver_status();
    read_block_to_buffer(1048575);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, READ_2) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* write_to_block() */

	/* WRITE_0 test */
	initialize_driver_status();
	write_to_block(0, 0, "0", 1);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, WRITE_0) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* WRITE_1 test */
	initialize_driver_status();
	write_to_block(-1, 0, "1", 1);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, WRITE_1) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* WRITE_2 test */
	initialize_driver_status();
	write_to_block(0, -1, "2", 1);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, WRITE_2) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* WRITE_0 test */
	initialize_driver_status();
	write_to_block(1048574, 0, "3", 1);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, WRITE_0) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* WRITE_3 test */
	initialize_driver_status();
	write_to_block(1048575, 0, "4", 1);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, WRITE_3) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* WRITE_0 test */
	initialize_driver_status();
	write_to_block(1048574, 2, TEST_STRING, 1022);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, WRITE_0) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* WRITE_4 test */
	initialize_driver_status();
	write_to_block(1048574, 3, TEST_STRING, 1022);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, WRITE_4) == 0);

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* WRITE_4 test */
	initialize_driver_status();
	write_to_block(1048574, 0, TEST_STRING, 1024 * 2);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, WRITE_4) == 0);

	/* write 1 byte to last offset of final writable block in vsd */
	initialize_driver_status();
	write_to_block(1048574, 1023, "w", 1);
	read_block_to_buffer(1048574);
	free_driver_status();
	assert(return_buffer()[1023] == 'w');

	memset(BUFF, 0, 300);
	INDEX = 0;

	/* WRITE_4 test */
	initialize_driver_status();
	write_to_block(1048574, 1024, "l", 1);
	traverse(return_driver_status_log(), fill_test_buffer);
	free_driver_status();
	assert(strcmp(BUFF, WRITE_4) == 0);

    return (0);
}