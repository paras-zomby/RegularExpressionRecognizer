cmake_minimum_required(VERSION 3.24)
project(RegularExpressionRecognizer)

set(CMAKE_CXX_STANDARD 17)

add_executable(RegularExpressionRecognizer main.cpp Regular.cpp Regular.h NFAutomata.cpp NFAutomata.h DFA.cpp DFA.h)
