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


/*

    tests for the VSD driver

*/

int main(void)
{
    /* testing base I/O calls */
    read_block_to_buffer(0);
    assert(strcmp(return_driver_status().status, READ_SUCCESS) == 0 && "read_block_to_buffer()");
    assert(return_driver_status().errrno == 0 && "read_block_to_buffer()");
    return_vsd_size();
    assert(strcmp(return_driver_status().status, READ_SUCCESS) == 0 && "return_vsd_size()");
    assert(return_driver_status().errrno == 0 && "return_vsd_size()");
    write_to_block(0, 2, "ll ", 2);
    assert(strcmp(return_driver_status().status, WRITE_SUCCESS) == 0 && "write_to_block()");

    /* testing emulation controls */
    read_block_to_buffer(VSD_BLOCK_SIZE * VSD_BLOCK_SIZE);
    assert(strcmp(return_driver_status().status, VREXCEED_ERROR) == 0 && "read_block_to_buffer()");
    assert(return_driver_status().errrno == -1 && "read_block_to_buffer()");
    write_to_block(VSD_BLOCK_SIZE * VSD_BLOCK_SIZE, 0, "ll", 2);
    assert(strcmp(return_driver_status().status, VWEXCEED_ERROR) == 0 && "write_to_block()");
    assert(return_driver_status().errrno == -1 && "write_to_block()");
    write_to_block(-1, 2, "ll", 2);
    assert(strcmp(return_driver_status().status, BINDEX_ERROR) == 0 && "write_to_block()");
    assert(return_driver_status().errrno == -1 && "write_to_block()");
    write_to_block(0, -1, "ll", 2);
    assert(strcmp(return_driver_status().status, OFFSET_ERROR) == 0 && "write_to_block()");
    assert(return_driver_status().errrno == -1 && "write_to_block()");
    write_to_block(0, VSD_BLOCK_SIZE, "ll", 2);
    assert(strcmp(return_driver_status().status, EXCEED_ERROR) == 0 && "write_to_block()");
    assert(return_driver_status().errrno == -1 && "write_to_block()");

    return (0);
}