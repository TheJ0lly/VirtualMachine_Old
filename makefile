help:
	@echo "Use \"run\" or \"build\" along with make"
	@echo "   run   - will build the project then run it"
	@echo "   build - will build the project"

build:
	gcc -o bin/vm src/*.c -Wall -Werror -pedantic -std=c11

run: build
	./bin/vm
