#include "vsd-driver.h"
#include <stdio.h>

int main(void)
{
    printf("%ld\n", return_vsd_size());
    printf("%s\n%d", return_driver_status().status, return_driver_status().errrno);

    return (0);
}