#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I../../common -I../../engine -I..
CFLAGS = -march=pentium2 -m32 -D_LINUX
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -m32 -static-libgcc -static-libstdc++

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -Wall -g -DBUILD_DLL
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = ../temp
DEP_DEBUG = 
OUT_DEBUG = ../../../bin/elenasm.so

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O3 -Wall -DBUILD_DLL
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = ../temp
DEP_RELEASE = 
OUT_RELEASE = ../../../bin/elenasm.so

OBJ_DEBUG = $(OBJDIR_DEBUG)/__/treeparser.o $(OBJDIR_DEBUG)/__/transformer.o $(OBJDIR_DEBUG)/__/session.o $(OBJDIR_DEBUG)/__/linux32/main.o $(OBJDIR_DEBUG)/__/inlineparser.o $(OBJDIR_DEBUG)/__/cfparser.o $(OBJDIR_DEBUG)/__/__/engine/textsource.o $(OBJDIR_DEBUG)/__/__/common/altstrings.o $(OBJDIR_DEBUG)/__/__/engine/syntaxtree.o $(OBJDIR_DEBUG)/__/__/engine/parsertable.o $(OBJDIR_DEBUG)/__/__/engine/bytecode.o $(OBJDIR_DEBUG)/__/__/common/files.o $(OBJDIR_DEBUG)/__/__/common/dump.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/__/treeparser.o $(OBJDIR_RELEASE)/__/transformer.o $(OBJDIR_RELEASE)/__/session.o $(OBJDIR_RELEASE)/__/linux32/main.o $(OBJDIR_RELEASE)/__/inlineparser.o $(OBJDIR_RELEASE)/__/cfparser.o $(OBJDIR_RELEASE)/__/__/engine/textsource.o $(OBJDIR_RELEASE)/__/__/common/altstrings.o $(OBJDIR_RELEASE)/__/__/engine/syntaxtree.o $(OBJDIR_RELEASE)/__/__/engine/parsertable.o $(OBJDIR_RELEASE)/__/__/engine/bytecode.o $(OBJDIR_RELEASE)/__/__/common/files.o $(OBJDIR_RELEASE)/__/__/common/dump.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d ../../../bin || mkdir -p ../../../bin
	test -d $(OBJDIR_DEBUG)/__ || mkdir -p $(OBJDIR_DEBUG)/__
	test -d $(OBJDIR_DEBUG)/__/linux32 || mkdir -p $(OBJDIR_DEBUG)/__/linux32
	test -d $(OBJDIR_DEBUG)/__/__/engine || mkdir -p $(OBJDIR_DEBUG)/__/__/engine
	test -d $(OBJDIR_DEBUG)/__/__/common || mkdir -p $(OBJDIR_DEBUG)/__/__/common

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) -shared $(LIBDIR_DEBUG) $(OBJ_DEBUG)  -o $(OUT_DEBUG) $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/__/treeparser.o: ../treeparser.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../treeparser.cpp -o $(OBJDIR_DEBUG)/__/treeparser.o

$(OBJDIR_DEBUG)/__/transformer.o: ../transformer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../transformer.cpp -o $(OBJDIR_DEBUG)/__/transformer.o

$(OBJDIR_DEBUG)/__/session.o: ../session.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../session.cpp -o $(OBJDIR_DEBUG)/__/session.o

$(OBJDIR_DEBUG)/__/linux32/main.o: ../linux32/main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../linux32/main.cpp -o $(OBJDIR_DEBUG)/__/linux32/main.o

$(OBJDIR_DEBUG)/__/inlineparser.o: ../inlineparser.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../inlineparser.cpp -o $(OBJDIR_DEBUG)/__/inlineparser.o

$(OBJDIR_DEBUG)/__/cfparser.o: ../cfparser.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../cfparser.cpp -o $(OBJDIR_DEBUG)/__/cfparser.o

$(OBJDIR_DEBUG)/__/__/engine/textsource.o: ../../engine/textsource.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/textsource.cpp -o $(OBJDIR_DEBUG)/__/__/engine/textsource.o

$(OBJDIR_DEBUG)/__/__/common/altstrings.o: ../../common/altstrings.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../common/altstrings.cpp -o $(OBJDIR_DEBUG)/__/__/common/altstrings.o

$(OBJDIR_DEBUG)/__/__/engine/syntaxtree.o: ../../engine/syntaxtree.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/syntaxtree.cpp -o $(OBJDIR_DEBUG)/__/__/engine/syntaxtree.o

$(OBJDIR_DEBUG)/__/__/engine/parsertable.o: ../../engine/parsertable.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/parsertable.cpp -o $(OBJDIR_DEBUG)/__/__/engine/parsertable.o

$(OBJDIR_DEBUG)/__/__/engine/bytecode.o: ../../engine/bytecode.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/bytecode.cpp -o $(OBJDIR_DEBUG)/__/__/engine/bytecode.o

$(OBJDIR_DEBUG)/__/__/common/files.o: ../../common/files.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../common/files.cpp -o $(OBJDIR_DEBUG)/__/__/common/files.o

$(OBJDIR_DEBUG)/__/__/common/dump.o: ../../common/dump.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../common/dump.cpp -o $(OBJDIR_DEBUG)/__/__/common/dump.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf ../../../bin
	rm -rf $(OBJDIR_DEBUG)/__
	rm -rf $(OBJDIR_DEBUG)/__/linux32
	rm -rf $(OBJDIR_DEBUG)/__/__/engine
	rm -rf $(OBJDIR_DEBUG)/__/__/common

before_release: 
	test -d ../../../bin || mkdir -p ../../../bin
	test -d $(OBJDIR_RELEASE)/__ || mkdir -p $(OBJDIR_RELEASE)/__
	test -d $(OBJDIR_RELEASE)/__/linux32 || mkdir -p $(OBJDIR_RELEASE)/__/linux32
	test -d $(OBJDIR_RELEASE)/__/__/engine || mkdir -p $(OBJDIR_RELEASE)/__/__/engine
	test -d $(OBJDIR_RELEASE)/__/__/common || mkdir -p $(OBJDIR_RELEASE)/__/__/common

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) -shared $(LIBDIR_RELEASE) $(OBJ_RELEASE)  -o $(OUT_RELEASE) $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/__/treeparser.o: ../treeparser.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../treeparser.cpp -o $(OBJDIR_RELEASE)/__/treeparser.o

$(OBJDIR_RELEASE)/__/transformer.o: ../transformer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../transformer.cpp -o $(OBJDIR_RELEASE)/__/transformer.o

$(OBJDIR_RELEASE)/__/session.o: ../session.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../session.cpp -o $(OBJDIR_RELEASE)/__/session.o

$(OBJDIR_RELEASE)/__/linux32/main.o: ../linux32/main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../linux32/main.cpp -o $(OBJDIR_RELEASE)/__/linux32/main.o

$(OBJDIR_RELEASE)/__/inlineparser.o: ../inlineparser.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../inlineparser.cpp -o $(OBJDIR_RELEASE)/__/inlineparser.o

$(OBJDIR_RELEASE)/__/cfparser.o: ../cfparser.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../cfparser.cpp -o $(OBJDIR_RELEASE)/__/cfparser.o

$(OBJDIR_RELEASE)/__/__/engine/textsource.o: ../../engine/textsource.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/textsource.cpp -o $(OBJDIR_RELEASE)/__/__/engine/textsource.o

$(OBJDIR_RELEASE)/__/__/common/altstrings.o: ../../common/altstrings.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../common/altstrings.cpp -o $(OBJDIR_RELEASE)/__/__/common/altstrings.o

$(OBJDIR_RELEASE)/__/__/engine/syntaxtree.o: ../../engine/syntaxtree.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/syntaxtree.cpp -o $(OBJDIR_RELEASE)/__/__/engine/syntaxtree.o

$(OBJDIR_RELEASE)/__/__/engine/parsertable.o: ../../engine/parsertable.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/parsertable.cpp -o $(OBJDIR_RELEASE)/__/__/engine/parsertable.o

$(OBJDIR_RELEASE)/__/__/engine/bytecode.o: ../../engine/bytecode.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/bytecode.cpp -o $(OBJDIR_RELEASE)/__/__/engine/bytecode.o

$(OBJDIR_RELEASE)/__/__/common/files.o: ../../common/files.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../common/files.cpp -o $(OBJDIR_RELEASE)/__/__/common/files.o

$(OBJDIR_RELEASE)/__/__/common/dump.o: ../../common/dump.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../common/dump.cpp -o $(OBJDIR_RELEASE)/__/__/common/dump.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf ../../../bin
	rm -rf $(OBJDIR_RELEASE)/__
	rm -rf $(OBJDIR_RELEASE)/__/linux32
	rm -rf $(OBJDIR_RELEASE)/__/__/engine
	rm -rf $(OBJDIR_RELEASE)/__/__/common

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release
