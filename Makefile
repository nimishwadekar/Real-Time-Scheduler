SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, obj/%.o, $(SRC))

FLAGS := -I src/include -Wall -std=c99

bin/scheduler: $(OBJ)
	gcc $^ -o $@

run: bin/scheduler
	./bin/scheduler

obj/%.o: src/%.c
	gcc $(FLAGS) -c $^ -o $@

clean:
	rm -f obj/* bin/*