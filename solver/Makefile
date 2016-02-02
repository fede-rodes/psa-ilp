
ifneq (, $(wildcard Makefile.local))
include Makefile.local
endif

SRC=src
INCLUDE=src
BUILD=build

INCS=$(wildcard $(INCLUDE)/*.h)
SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(SRCS:$(SRC)/%=$(BUILD)/%.o)

EXENAME=psailp.exe
EXE=$(BUILD)/$(EXENAME)

CFLAGS += -W -Wall -Wextra -O3 -Wno-unused-parameter 
#valgrind:
#CFLAGS += -W -Wall -Wextra -g -Wno-unused-parameter

.PHONY: all test clean show

all: $(EXE)

test: $(EXE)
	$(EXE) -out $(BUILD)/Gen50-2-1.psa-ilp.log -msol on ../instances/Gen50-2-1.dat
	$(EXE) -out $(BUILD)/Gen50-2-2.psa-ilp.log -msol on ../instances/Gen50-2-2.dat
	$(EXE) -out $(BUILD)/Gen50-2-3.psa-ilp.log -msol on ../instances/Gen50-2-3.dat

ifeq ($(OS),Windows_NT)
$(EXE): $(OBJS)
		$(CXX) -lstdc++ -o $@ $^ -lpsapi -Llib_w32 -lglpk_4_52
		cp lib_w32/glpk_4_52.dll build
else
UNAME := $(shell uname)
    
ifeq ($(UNAME),Linux)
$(EXE): $(OBJS)
		$(CXX) -lstdc++ -o $@ $^ -lrt -Llib_linux -lglpk
		cp lib_linux/libglpk.so build
endif
endif	
	
#$(EXE): $(OBJS)
#	$(CXX) -lstdc++ -o $@ $^ -lpsapi -Llib -lqsopt -Llib_w32 -lglpk_4_52
#	cp lib/qsopt.dll build
#	cp lib_w32/glpk_4_52.dll build

$(BUILD)/%.cpp.o: $(SRC)/%.cpp $(INCS) $(HPPS) | $(BUILD)
#	$(CXX) -c -o $@ $<
	$(CXX) -std=c++11 -c -o $@ $<

$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)

show:
	@echo INCS: $(INCS)
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)
	@echo LIBNAME: $(LIBNAME)
	@echo LIB: $(LIB)
	@echo CC: $(CC)
	@echo LINK.c: $(LINK.c)
