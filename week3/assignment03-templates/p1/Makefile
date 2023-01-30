CC=     gcc
LD=     ld
CFLAGS= -g -W -Wall -Werror -std=c99
TARGET= exploit_program
SRC=    exploit_program.c run_program.c testlib.c main.c
OBJ=    $(SRC:.c=.o)

all:	vulnerable exploit_program

vulnerable: vulnerable.c
	gcc -g vulnerable.c -o vulnerable -fno-stack-protector

exploit_program: $(OBJ)
	$(CC) -o $@ $(OBJ)

.PHONY: $(TARGET)-solution.o
$(TARGET)-sol: $(subst $(TARGET),$(TARGET)-solution,$(OBJ))
	$(CC) -o $@ $^

$(TARGET)-solution.o:
	@test -f $(TARGET)-solution.c \
	|| { echo "Please download solution." >&2; false; }
	$(CC) $(CFLAGS) -c $(TARGET)-solution.c

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(TARGET) $(TARGET)-sol *.o
	rm -rf vulnerable
