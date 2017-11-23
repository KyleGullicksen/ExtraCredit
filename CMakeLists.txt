cmake_minimum_required(VERSION 3.8)
project(ExtraCredit)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES re-nfaeEC.cpp)
add_executable(ExtraCredit ${SOURCE_FILES})


#Remove e-moves in NFA compiling
#set(SOURCE_FILES Transition.h Utilities.h Utilities.cpp Transition.cpp TransitionTable.h TransitionTable.cpp nfaeRemoveEC.cpp)

#NFA -> DFA
#set(SOURCE_FILES Transition.h Utilities.h Utilities.cpp Transition.cpp TransitionTable.h TransitionTable.cpp nfaeRemoveEC.cpp)