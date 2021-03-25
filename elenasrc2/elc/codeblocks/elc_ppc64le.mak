#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I../../common -I../../engine -I.. -I../win32
CFLAGS = -mcpu=power8 -Wall -std=c++11 -m64 -Wno-conversion-null -Wno-switch -D_LINUX
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -m32 -static-libgcc -static-libstdc++ -ldl

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = ../temp
DEP_DEBUG = 
OUT_DEBUG = ../../../bin/elena-lc

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = ../temp
DEP_RELEASE = 
OUT_RELEASE = ../../../bin/elena-lc

OBJ_DEBUG = $(OBJDIR_DEBUG)/__/derivation.o $(OBJDIR_DEBUG)/__/compilerscope.o $(OBJDIR_DEBUG)/__/compiler.o $(OBJDIR_DEBUG)/__/bcwriter.o $(OBJDIR_DEBUG)/__/__/engine/x86jitcompiler.o $(OBJDIR_DEBUG)/__/__/engine/x86helper.o $(OBJDIR_DEBUG)/__/__/engine/syntaxtree.o $(OBJDIR_DEBUG)/__/__/engine/section.o $(OBJDIR_DEBUG)/__/image.o $(OBJDIR_DEBUG)/__/linux32/elc.o $(OBJDIR_DEBUG)/__/linux32/linker.o $(OBJDIR_DEBUG)/__/linux32/syslibloader.o $(OBJDIR_DEBUG)/__/parser.o $(OBJDIR_DEBUG)/__/project.o $(OBJDIR_DEBUG)/__/separser.o $(OBJDIR_DEBUG)/__/source.o $(OBJDIR_DEBUG)/__/__/engine/bytecode.o $(OBJDIR_DEBUG)/__/__/common/xmlreader.o $(OBJDIR_DEBUG)/__/__/common/files.o $(OBJDIR_DEBUG)/__/__/common/dump.o $(OBJDIR_DEBUG)/__/__/common/config.o $(OBJDIR_DEBUG)/__/__/common/altstrings.o $(OBJDIR_DEBUG)/__/__/engine/compilerlogic.o $(OBJDIR_DEBUG)/__/__/engine/jitcompiler.o $(OBJDIR_DEBUG)/__/__/engine/jitlinker.o $(OBJDIR_DEBUG)/__/__/engine/libman.o $(OBJDIR_DEBUG)/__/__/engine/loader.o $(OBJDIR_DEBUG)/__/__/engine/module.o $(OBJDIR_DEBUG)/__/__/engine/parsertable.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/__/derivation.o $(OBJDIR_RELEASE)/__/compilerscope.o $(OBJDIR_RELEASE)/__/compiler.o $(OBJDIR_RELEASE)/__/bcwriter.o $(OBJDIR_RELEASE)/__/__/engine/x86jitcompiler.o $(OBJDIR_RELEASE)/__/__/engine/x86helper.o $(OBJDIR_RELEASE)/__/__/engine/syntaxtree.o $(OBJDIR_RELEASE)/__/__/engine/section.o $(OBJDIR_RELEASE)/__/image.o $(OBJDIR_RELEASE)/__/linux32/elc.o $(OBJDIR_RELEASE)/__/linux32/linker.o $(OBJDIR_RELEASE)/__/linux32/syslibloader.o $(OBJDIR_RELEASE)/__/parser.o $(OBJDIR_RELEASE)/__/project.o $(OBJDIR_RELEASE)/__/separser.o $(OBJDIR_RELEASE)/__/source.o $(OBJDIR_RELEASE)/__/__/engine/bytecode.o $(OBJDIR_RELEASE)/__/__/common/xmlreader.o $(OBJDIR_RELEASE)/__/__/common/files.o $(OBJDIR_RELEASE)/__/__/common/dump.o $(OBJDIR_RELEASE)/__/__/common/config.o $(OBJDIR_RELEASE)/__/__/common/altstrings.o $(OBJDIR_RELEASE)/__/__/engine/compilerlogic.o $(OBJDIR_RELEASE)/__/__/engine/jitcompiler.o $(OBJDIR_RELEASE)/__/__/engine/jitlinker.o $(OBJDIR_RELEASE)/__/__/engine/libman.o $(OBJDIR_RELEASE)/__/__/engine/loader.o $(OBJDIR_RELEASE)/__/__/engine/module.o $(OBJDIR_RELEASE)/__/__/engine/parsertable.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d ../../../bin || mkdir -p ../../../bin
	test -d $(OBJDIR_DEBUG)/__ || mkdir -p $(OBJDIR_DEBUG)/__
	test -d $(OBJDIR_DEBUG)/__/__/engine || mkdir -p $(OBJDIR_DEBUG)/__/__/engine
	test -d $(OBJDIR_DEBUG)/__/linux32 || mkdir -p $(OBJDIR_DEBUG)/__/linux32
	test -d $(OBJDIR_DEBUG)/__/__/common || mkdir -p $(OBJDIR_DEBUG)/__/__/common

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/__/derivation.o: ../derivation.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../derivation.cpp -o $(OBJDIR_DEBUG)/__/derivation.o

$(OBJDIR_DEBUG)/__/compilerscope.o: ../compilerscope.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../compilerscope.cpp -o $(OBJDIR_DEBUG)/__/compilerscope.o

$(OBJDIR_DEBUG)/__/compiler.o: ../compiler.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../compiler.cpp -o $(OBJDIR_DEBUG)/__/compiler.o

$(OBJDIR_DEBUG)/__/bcwriter.o: ../bcwriter.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../bcwriter.cpp -o $(OBJDIR_DEBUG)/__/bcwriter.o

$(OBJDIR_DEBUG)/__/__/engine/x86jitcompiler.o: ../../engine/x86jitcompiler.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/x86jitcompiler.cpp -o $(OBJDIR_DEBUG)/__/__/engine/x86jitcompiler.o

$(OBJDIR_DEBUG)/__/__/engine/x86helper.o: ../../engine/x86helper.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/x86helper.cpp -o $(OBJDIR_DEBUG)/__/__/engine/x86helper.o

$(OBJDIR_DEBUG)/__/__/engine/syntaxtree.o: ../../engine/syntaxtree.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/syntaxtree.cpp -o $(OBJDIR_DEBUG)/__/__/engine/syntaxtree.o

$(OBJDIR_DEBUG)/__/__/engine/section.o: ../../engine/section.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/section.cpp -o $(OBJDIR_DEBUG)/__/__/engine/section.o

$(OBJDIR_DEBUG)/__/image.o: ../image.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../image.cpp -o $(OBJDIR_DEBUG)/__/image.o

$(OBJDIR_DEBUG)/__/linux32/elc.o: ../linux32/elc.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../linux32/elc.cpp -o $(OBJDIR_DEBUG)/__/linux32/elc.o

$(OBJDIR_DEBUG)/__/linux32/linker.o: ../linux32/linker.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../linux32/linker.cpp -o $(OBJDIR_DEBUG)/__/linux32/linker.o

$(OBJDIR_DEBUG)/__/linux32/syslibloader.o: ../linux32/syslibloader.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../linux32/syslibloader.cpp -o $(OBJDIR_DEBUG)/__/linux32/syslibloader.o

$(OBJDIR_DEBUG)/__/parser.o: ../parser.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../parser.cpp -o $(OBJDIR_DEBUG)/__/parser.o

$(OBJDIR_DEBUG)/__/project.o: ../project.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../project.cpp -o $(OBJDIR_DEBUG)/__/project.o

$(OBJDIR_DEBUG)/__/separser.o: ../separser.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../separser.cpp -o $(OBJDIR_DEBUG)/__/separser.o

$(OBJDIR_DEBUG)/__/source.o: ../source.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../source.cpp -o $(OBJDIR_DEBUG)/__/source.o

$(OBJDIR_DEBUG)/__/__/engine/bytecode.o: ../../engine/bytecode.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/bytecode.cpp -o $(OBJDIR_DEBUG)/__/__/engine/bytecode.o

$(OBJDIR_DEBUG)/__/__/common/xmlreader.o: ../../common/xmlreader.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../common/xmlreader.cpp -o $(OBJDIR_DEBUG)/__/__/common/xmlreader.o

$(OBJDIR_DEBUG)/__/__/common/files.o: ../../common/files.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../common/files.cpp -o $(OBJDIR_DEBUG)/__/__/common/files.o

$(OBJDIR_DEBUG)/__/__/common/dump.o: ../../common/dump.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../common/dump.cpp -o $(OBJDIR_DEBUG)/__/__/common/dump.o

$(OBJDIR_DEBUG)/__/__/common/config.o: ../../common/config.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../common/config.cpp -o $(OBJDIR_DEBUG)/__/__/common/config.o

$(OBJDIR_DEBUG)/__/__/common/altstrings.o: ../../common/altstrings.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../common/altstrings.cpp -o $(OBJDIR_DEBUG)/__/__/common/altstrings.o

$(OBJDIR_DEBUG)/__/__/engine/compilerlogic.o: ../../engine/compilerlogic.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/compilerlogic.cpp -o $(OBJDIR_DEBUG)/__/__/engine/compilerlogic.o

$(OBJDIR_DEBUG)/__/__/engine/jitcompiler.o: ../../engine/jitcompiler.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/jitcompiler.cpp -o $(OBJDIR_DEBUG)/__/__/engine/jitcompiler.o

$(OBJDIR_DEBUG)/__/__/engine/jitlinker.o: ../../engine/jitlinker.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/jitlinker.cpp -o $(OBJDIR_DEBUG)/__/__/engine/jitlinker.o

$(OBJDIR_DEBUG)/__/__/engine/libman.o: ../../engine/libman.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/libman.cpp -o $(OBJDIR_DEBUG)/__/__/engine/libman.o

$(OBJDIR_DEBUG)/__/__/engine/loader.o: ../../engine/loader.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/loader.cpp -o $(OBJDIR_DEBUG)/__/__/engine/loader.o

$(OBJDIR_DEBUG)/__/__/engine/module.o: ../../engine/module.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/module.cpp -o $(OBJDIR_DEBUG)/__/__/engine/module.o

$(OBJDIR_DEBUG)/__/__/engine/parsertable.o: ../../engine/parsertable.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ../../engine/parsertable.cpp -o $(OBJDIR_DEBUG)/__/__/engine/parsertable.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf ../../../bin
	rm -rf $(OBJDIR_DEBUG)/__
	rm -rf $(OBJDIR_DEBUG)/__/__/engine
	rm -rf $(OBJDIR_DEBUG)/__/linux32
	rm -rf $(OBJDIR_DEBUG)/__/__/common

before_release: 
	test -d ../../../bin || mkdir -p ../../../bin
	test -d $(OBJDIR_RELEASE)/__ || mkdir -p $(OBJDIR_RELEASE)/__
	test -d $(OBJDIR_RELEASE)/__/__/engine || mkdir -p $(OBJDIR_RELEASE)/__/__/engine
	test -d $(OBJDIR_RELEASE)/__/linux32 || mkdir -p $(OBJDIR_RELEASE)/__/linux32
	test -d $(OBJDIR_RELEASE)/__/__/common || mkdir -p $(OBJDIR_RELEASE)/__/__/common

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/__/derivation.o: ../derivation.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../derivation.cpp -o $(OBJDIR_RELEASE)/__/derivation.o

$(OBJDIR_RELEASE)/__/compilerscope.o: ../compilerscope.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../compilerscope.cpp -o $(OBJDIR_RELEASE)/__/compilerscope.o

$(OBJDIR_RELEASE)/__/compiler.o: ../compiler.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../compiler.cpp -o $(OBJDIR_RELEASE)/__/compiler.o

$(OBJDIR_RELEASE)/__/bcwriter.o: ../bcwriter.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../bcwriter.cpp -o $(OBJDIR_RELEASE)/__/bcwriter.o

$(OBJDIR_RELEASE)/__/__/engine/x86jitcompiler.o: ../../engine/x86jitcompiler.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/x86jitcompiler.cpp -o $(OBJDIR_RELEASE)/__/__/engine/x86jitcompiler.o

$(OBJDIR_RELEASE)/__/__/engine/x86helper.o: ../../engine/x86helper.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/x86helper.cpp -o $(OBJDIR_RELEASE)/__/__/engine/x86helper.o

$(OBJDIR_RELEASE)/__/__/engine/syntaxtree.o: ../../engine/syntaxtree.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/syntaxtree.cpp -o $(OBJDIR_RELEASE)/__/__/engine/syntaxtree.o

$(OBJDIR_RELEASE)/__/__/engine/section.o: ../../engine/section.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/section.cpp -o $(OBJDIR_RELEASE)/__/__/engine/section.o

$(OBJDIR_RELEASE)/__/image.o: ../image.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../image.cpp -o $(OBJDIR_RELEASE)/__/image.o

$(OBJDIR_RELEASE)/__/linux32/elc.o: ../linux32/elc.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../linux32/elc.cpp -o $(OBJDIR_RELEASE)/__/linux32/elc.o

$(OBJDIR_RELEASE)/__/linux32/linker.o: ../linux32/linker.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../linux32/linker.cpp -o $(OBJDIR_RELEASE)/__/linux32/linker.o

$(OBJDIR_RELEASE)/__/linux32/syslibloader.o: ../linux32/syslibloader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../linux32/syslibloader.cpp -o $(OBJDIR_RELEASE)/__/linux32/syslibloader.o

$(OBJDIR_RELEASE)/__/parser.o: ../parser.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../parser.cpp -o $(OBJDIR_RELEASE)/__/parser.o

$(OBJDIR_RELEASE)/__/project.o: ../project.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../project.cpp -o $(OBJDIR_RELEASE)/__/project.o

$(OBJDIR_RELEASE)/__/separser.o: ../separser.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../separser.cpp -o $(OBJDIR_RELEASE)/__/separser.o

$(OBJDIR_RELEASE)/__/source.o: ../source.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../source.cpp -o $(OBJDIR_RELEASE)/__/source.o

$(OBJDIR_RELEASE)/__/__/engine/bytecode.o: ../../engine/bytecode.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/bytecode.cpp -o $(OBJDIR_RELEASE)/__/__/engine/bytecode.o

$(OBJDIR_RELEASE)/__/__/common/xmlreader.o: ../../common/xmlreader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../common/xmlreader.cpp -o $(OBJDIR_RELEASE)/__/__/common/xmlreader.o

$(OBJDIR_RELEASE)/__/__/common/files.o: ../../common/files.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../common/files.cpp -o $(OBJDIR_RELEASE)/__/__/common/files.o

$(OBJDIR_RELEASE)/__/__/common/dump.o: ../../common/dump.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../common/dump.cpp -o $(OBJDIR_RELEASE)/__/__/common/dump.o

$(OBJDIR_RELEASE)/__/__/common/config.o: ../../common/config.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../common/config.cpp -o $(OBJDIR_RELEASE)/__/__/common/config.o

$(OBJDIR_RELEASE)/__/__/common/altstrings.o: ../../common/altstrings.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../common/altstrings.cpp -o $(OBJDIR_RELEASE)/__/__/common/altstrings.o

$(OBJDIR_RELEASE)/__/__/engine/compilerlogic.o: ../../engine/compilerlogic.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/compilerlogic.cpp -o $(OBJDIR_RELEASE)/__/__/engine/compilerlogic.o

$(OBJDIR_RELEASE)/__/__/engine/jitcompiler.o: ../../engine/jitcompiler.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/jitcompiler.cpp -o $(OBJDIR_RELEASE)/__/__/engine/jitcompiler.o

$(OBJDIR_RELEASE)/__/__/engine/jitlinker.o: ../../engine/jitlinker.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/jitlinker.cpp -o $(OBJDIR_RELEASE)/__/__/engine/jitlinker.o

$(OBJDIR_RELEASE)/__/__/engine/libman.o: ../../engine/libman.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/libman.cpp -o $(OBJDIR_RELEASE)/__/__/engine/libman.o

$(OBJDIR_RELEASE)/__/__/engine/loader.o: ../../engine/loader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/loader.cpp -o $(OBJDIR_RELEASE)/__/__/engine/loader.o

$(OBJDIR_RELEASE)/__/__/engine/module.o: ../../engine/module.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/module.cpp -o $(OBJDIR_RELEASE)/__/__/engine/module.o

$(OBJDIR_RELEASE)/__/__/engine/parsertable.o: ../../engine/parsertable.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ../../engine/parsertable.cpp -o $(OBJDIR_RELEASE)/__/__/engine/parsertable.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf ../../../bin
	rm -rf $(OBJDIR_RELEASE)/__
	rm -rf $(OBJDIR_RELEASE)/__/__/engine
	rm -rf $(OBJDIR_RELEASE)/__/linux32
	rm -rf $(OBJDIR_RELEASE)/__/__/common

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release
