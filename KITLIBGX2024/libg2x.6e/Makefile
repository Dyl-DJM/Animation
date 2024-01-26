#==================================================
# version 6e (2023/2024) -
#==================================================
ifeq ($(CPP),1)
	CC  = g++
	STD = -std=c++11
	CPP = ++
else
	CC  = clang
	STD = -std=c17
	CPP =
endif


# FLAGS : options de compil. (mode debug ou optimise)
ifeq ($(GDB),1)
  CFLAGS = -g -Wpointer-arith -Wall
	EXT    = $(CPP).gdb
else
  CFLAGS = -O2
	EXT    = $(CPP)
endif

LFLAGS = -lm -L/usr/lib/x86_64-linux-gnu/ -lGL -lGLU -lglut
PFLAGS = -I./include

SRC = src
BIN = bin

# liste des modules
MODULES = g2x_window.o g2x_geom.o g2x_colors.o g2x_tools.o g2x_capture.o g2x_draw.o g2x_transfo.o g2x_pixmap.o g2x_geoalgo.o g2x_polygon.o g2x_particle.o g2x_board.o

all : g2x

# compilation generique d'un module %.o a partir de src/%.c
%.o : $(SRC)/%.c
	@echo "module $@"
	$(CC)  $(STD) -fPIC $(CFLAGS) $(PFLAGS) -c $^ -o $@
	@echo "------------------------"

# compilation generique d'un executable - edition de liens
% : $(MODULES) %.o
	@echo "assemblage [$^]->$@"
	$(CC) $^ $(LFLAGS) -o $@$(EXT)
	@echo "------------------------"

# compilation de libg2x
g2x : $(MODULES)
	@echo "assemblage bibliotheque dynamique $(BIN)/lib$@$(EXT).so"
	@$(CC) -fpic -shared $^ $(LFLAGS) -o $(BIN)/lib$@$(EXT).so
	@echo "assemblage bibliotheque d'archive $(BIN)/lib$@$(EXT).a"
	@ar r $(BIN)/lib$@$(EXT).a $^
	@echo "------------------------                                "

.PHONY : clean cleanall ?

clean :
	rm -f *.o
cleanall :
	rm -f *.o $(BIN)/libg2x*

# informations sur les paramètres de compilation
? :
	@echo "---------compilation informations----------"
	@echo "  compilateur : $(CC)"
	@echo "  CFLAGS      : $(CFLAGS)"
	@echo "  LFLAGS      : $(LFLAGS)"
	@echo "  PFLAGS      : $(PFLAGS)"

