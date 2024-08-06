import sys, os

def build():
    os.system("gcc -o bin/vm src/*.c -Wall -Werror -pedantic -std=c11")
    print("\033[32mVM built successfully\033[00m")

def run():
    print("Running VM...")
    os.system("./bin/vm && rm ./bin/vm")



if len(sys.argv) != 2:
    print("Usage python(3) build / run")
    sys.exit(1)

match(sys.argv[1]):
    case "build":
        build()
    case "run":
        build()
        run()
    case _:
        print("Use \"run\" or \"build\" along with make")
        print("   run   - will build the project then run it")
        print("   build - will build the project")


