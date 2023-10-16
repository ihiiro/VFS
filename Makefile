SRCS = vsd-driver/vsd-driver.c tests/vsd-driver.c
TESTS = tests/vsd-driver
CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(TESTS)

$(TESTS): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) singly-linked-list/sll.a -Ivsd-driver -Isingly-linked-list -o $(TESTS)