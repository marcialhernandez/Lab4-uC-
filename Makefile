executable:=exp
library:=libtest

#tmp:=./tmp
#src:=./src
#test:=./test

#objects:=$(tmp)/entrada.o

#sources:=$(src)/entrada.cc

#-std=c++0x -O3
#cxxflags:= -g -Wall
cxxflags:= -g -Wall
cxx:=u++
#thread:=-lpthread

includes:=-I./ -I./include -I../api/include
libs:=-L./ -L./lib

main: $(objects)
	$(cxx) $(includes) $(libs) -o $(executable) $(executable).cc $(cxxflags)


clean:
	rm -rf $(tmp);
	rm -f $(executable);