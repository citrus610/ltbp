CXX = g++
ifeq ($(BUILD),debug)   
CXXFLAGS += -fdiagnostics-color=always -DUNICODE -std=c++17 -Wall -Og -g
else
CXXFLAGS += -DUNICODE -DNDEBUG -std=c++17 -O3 -flto -s -march=native
endif

STATIC_LIB = -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -lsetupapi -lhid -static

.PHONY: all ltbp clean makedir

all: ltbp

ltbp: makedir
	@echo Building Lemon Tea ltbp...
	@$(CXX) $(CXXFLAGS) "tetris\*.cpp" "ltbp\*.cpp" $(STATIC_LIB) -o "bin\ltbp\ltbp.exe"
	@echo Finished building Lemon Tea ltbp!

clean: makedir
	@echo Cleaning the bin directory
	@rmdir /s /q bin
	@make makedir

makedir:
	@IF NOT exist bin ( mkdir bin )
	@IF NOT exist bin\ltbp ( mkdir bin\ltbp )
	@IF NOT exist bin\ltbp\content ( mkdir bin\ltbp\content )
	@copy content bin\ltbp\content

.DEFAULT_GOAL := ltbp