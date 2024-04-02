
# option de compil pour gdb
GDB = 1
# option de compil pour C++
CPP = 0

ifeq ($(CPP),1)
	CC  = g++
	STD = -std=c++11
	CPP = ++
else
	CC  = clang
	STD = -std=c17
	CPP =
endif

ifeq ($(GDB),1)
  CFLAGS  = -g -Wpointer-arith -Wall
  EXT     = $(CPP).gdb
else
  CFLAGS  = -O2
  EXT     = $(CPP)
endif

LFLAGS = $(libG2X)$(EXT)
PFLAGS = $(incG2X) -I./include


# répertoire pour les sources .c
src = src/

# all : g2x_00_squelette g2x_01_control g2x_02_cercle g2x_03_drawfunction\
#       g2x_04_hsvflower g2x_05_hsvcolor g2x_06_image g2x_REFERENCE

all : flag2D

# règle générique de création de xxx.o à partir de src/xxx.c
%.o : $(src)%.c.*
	@echo "module $@"
	@$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@
	@echo "------------------------"


# règle générique de création de xxx.o à partir de src/xxx.c
%.o : $(src)%.cpp
	@echo "module $@"
	@$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@
	@echo "------------------------"

# règle générique de création de l'executable xxx à partir de src/xxx.c (1 seul module)
% : %.o
	@echo "assemblage [$^]->$@"
	@$(CC) $^ $(LFLAGS) -o $@
	@echo "------------------------"

.PHONY : clean cleanall ?

# informations sur les paramètres de compilation
? :
	@echo "---------informations de compilation----------"
	@echo "  processeur     : $(PROCBIT)"
	@echo "  compilateur    : $(CC)"
	@echo "  CFLAGS         : $(CFLAGS)"
	@echo "  LFLAGS         : $(LFLAGS)"
	@echo "  PFLAGS         : $(PFLAGS)"

clean :
	@rm -f *.o

cleanall :
	@rm -f *.o g2x_*

