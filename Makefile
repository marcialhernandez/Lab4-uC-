executable:=exp
library:=libtest

tmp:=./tmp
src:=./src
test:=./test

objects:=$(tmp)/reconocedora.o $(tmp)/entradaconsola.o $(tmp)/buffercomun.o $(tmp)/productora.o $(tmp)/escritora.o

sources:=$(src)/reconocedora.cc $(src)/entradaconsola.cc $(src)/buffercomun.cc $(src)/productora.cc $(src)/escritora.cc

#-std=c++0x -O3
#cxxflags:= -g -Wall
cxxflags:= -g -Wall
cxx:=u++ -multi

includes:=-I./ -I./include -I../api/include
libs:=-L./ -L./lib

main: $(objects)
	$(cxx) $(includes) $(libs) $(objects) -o $(executable) $(executable).cc $(cxxflags)

$(tmp)/%.o: $(src)/%.cc 
	test -d $(tmp) || mkdir $(tmp)
	$(cxx) $(includes) -c -o $(tmp)/$(*F).o $(src)/$*.cc $(cxxflags)

testing:  $(objects)
	$(cxx) $(includes) $(libs) $(objects) -o $(executable) $(executable).cc $(cxxflags)  -DTESTING

testing-lib:  $(objects)
	$(cxx) $(includes) $(libs) $(objects) -o $(library).so $(cxxflags) -shared

clean:
	rm -rf $(tmp);
	rm -f $(executable);