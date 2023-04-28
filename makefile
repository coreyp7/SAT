FILENAME = main

#OBJS specifies which files to compile as part of the project
OBJS = src\main.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
#INCLUDE_PATHS = -IC:\MinGW\include\SDL2 -I. # -I. is in the current directory (include folder)
#INCLUDE_PATHS = -I include -IC:\MinGW\include\SDL2
INCLUDE_PATHS = -IC:\MinGW\include\SDL2 -I. # -I. is in the current directory (include folder)


#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\MinGW\lib
#LIBRARY_PATHS = -L lib -LC:\MinGW\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
# ONLY DO THIS WHEN YOU DON'T WANT ERRORS IN CONSOLE.
#COMPILER_FLAGS = -w -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = sat.exe

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
