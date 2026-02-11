# LangtonsAnt Makefile
# Objetivo: compilar todos los archivos del proyecto, además de ciertos extras de depuración, debug y limpieza de ficheros


# Configuración del proyecto
BIN := LangtonsAnt

# Ficheros
SRC := main.cpp tape.cpp ant.cpp simulator.cpp
OBJ := $(SRC:.cpp=.o)
DEP := $(OBJ:.o=.d)

# Compilador
CXX ?= g++
STD = -std=c++20
WARN = -Wall -Wextra
OPT ?= -O2
DBG ?= -g
CXXFLAGS ?= $(STD) $(WARN) $(OPT) $(DBG) -MMD -MP

  
# Reglas

all: $(BIN)

.PHONY: all clean debug help

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
-include $(DEP)

# Recompilación y debugging
debug: CXXFLAGS := $(STD) $(WARN) -O0 -g3 -MMD -MP
debug: clean all

run: $(BIN)
	./$(BIN) $(ARGS)

clean: 
	@echo "Limpiando ficheros..."
	@rm -f $(BIN) $(OBJ) $(DEP)

help:
	@echo "Uso del comando make"
	@echo " make  -> compila con configuración por defecto"
	@echo " make debug -> recompila en modo depuración (sin optimizar, com máxima depuración)"
	@echo " make clean -> elimina los ficheros creados (.o y .d)"