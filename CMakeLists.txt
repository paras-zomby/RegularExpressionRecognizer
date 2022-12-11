cmake_minimum_required(VERSION 3.24)
project(RegularExpressionRecognizer)

set(CMAKE_CXX_STANDARD 17)

include_directories(DFAutomata NFAutomata Regular)

add_executable(RegularExpressionRecognizer
        demo.cpp
        Regular/Regular.cpp
        Regular/Regular.h
        NFAutomata/NFAutomata.cpp
        NFAutomata/NFAutomata.h
        DFAutomata/DFAutomata.cpp
        DFAutomata/DFAutomata.h
        )
