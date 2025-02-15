CC = clang
CFLAGS = -Wall -Wextra

PART1_EXE = 1.out
PART2_EXE = 2.out

PART1_OBJ = 1.o
PART2_OBJ = 2.o

UTIL_OBJS = util/str.o util/io.o util/dbg.o

part1: $(PART1_EXE)

part2: $(PART2_EXE)

$(PART1_EXE): $(PART1_OBJ) $(UTIL_OBJS)
	$(CC) $(PART1_OBJ) $(UTIL_OBJS) -o $(PART1_EXE)

$(PART2_EXE): $(PART2_OBJ) $(UTIL_OBJS)
	$(CC) $(PART2_OBJ) $(UTIL_OBJS) -o $(PART2_EXE)

$(PART1_OBJ): 1.c
	$(CC) $(CFLAGS) -c 1.c -o $(PART1_OBJ)

$(PART2_OBJ): 2.c
	$(CC) $(CFLAGS) -c 2.c -o $(PART2_OBJ)

util/str.o: util/str.c util/str.h
	$(CC) $(CFLAGS) -c util/str.c -o util/str.o

util/io.o: util/io.c util/io.h
	$(CC) $(CFLAGS) -c util/io.c -o util/io.o

util/dbg.o: util/dbg.c util/dbg.h
	$(CC) $(CFLAGS) -c util/dbg.c -o util/dbg.o

clean:
	rm -f $(PART1_EXE) $(PART2_EXE) *.o util/*.o
