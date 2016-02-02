
ifneq (, $(wildcard Makefile.local))
include Makefile.local
endif

SRC=src
INCLUDE=src
BUILD=build

INCS=$(wildcard $(INCLUDE)/*.h)
SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(SRCS:$(SRC)/%=$(BUILD)/%.o)

EXENAME=dat2lp.exe
EXE=$(BUILD)/$(EXENAME)

CFLAGS += -W -Wall -Wextra -O3 -Wno-unused-parameter

.PHONY: all test clean show

all: $(EXE)

test: $(EXE)
	$(EXE) ../instances/Gen20-2-1.dat
	$(EXE) ../instances/Gen20-2-2.dat
	$(EXE) ../instances/Gen20-2-3.dat

$(EXE): $(OBJS)
	$(CXX) -lstdc++ -o $@ $^ 

$(BUILD)/%.cpp.o: $(SRC)/%.cpp $(INCS) $(HPPS) | $(BUILD)
	$(CXX)  -c -o $@ $<

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
