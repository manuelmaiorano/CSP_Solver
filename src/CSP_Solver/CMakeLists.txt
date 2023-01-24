add_library(SolverLib solver.cpp)
add_library(ProblemLib problem.cpp)
add_library(AlgorithmsLib algorithms.cpp)

target_include_directories(SolverLib
                           INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_include_directories(ProblemLib
                            INTERFACE
                             $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_include_directories(AlgorithmsLib
                             INTERFACE
                              $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)