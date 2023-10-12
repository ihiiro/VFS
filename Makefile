SRCS = vsd-driver/vsd-driver.c tests/vsd-driver.c
HEADERS = vsd-driver
TESTS = tests/vsd-driver
CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(TESTS)

$(TESTS): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -I$(HEADERS) -o $(TESTS)