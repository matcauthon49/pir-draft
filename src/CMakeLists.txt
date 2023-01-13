cmake_minimum_required(VERSION 3.13)
project(PIR)

# include(${CMAKE_CURRENT_LIST_DIR})
#Installing library 
add_library(dpf STATIC
deps/cryptoTools/cryptoTools/Common/Defines.cpp
deps/cryptoTools/cryptoTools/Common/Log.cpp
deps/cryptoTools/cryptoTools/Crypto/AES.cpp
deps/cryptoTools/cryptoTools/Crypto/PRNG.cpp
dpf.cpp
keys.cpp
server_trusted.cpp 
server.cpp 
client.cpp
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(dpf PUBLIC OpenMP::OpenMP_CXX)
endif()

# #find_package(Threads REQUIRED)
target_include_directories(dpf
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:./>)

target_include_directories(dpf PRIVATE 
deps/cryptoTools
deps/EzPC)

target_compile_options(dpf PRIVATE -std=c++17 -Wall -Wno-parentheses-equality -Wno-ignored-attributes -march=native -maes -msse2 -msse4.1 -fopenmp)
install(
    TARGETS dpf 
    EXPORT DPFTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(FILES 
    dpf.h 
    keys.h 
    client.h 
    server.h 
    server_trusted.h
    DESTINATION include
)
install(DIRECTORY
    "deps/cryptoTools"
    "deps/EzPC"
    DESTINATION include
)

add_executable(test)
target_sources(test PRIVATE ${PROJECT_SOURCE_DIR}/tests/main.cpp)
target_include_directories(test PRIVATE
deps/cryptoTools
deps/EzPC
${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(test PRIVATE dpf )
if(OpenMP_CXX_FOUND)
    target_link_libraries(test PUBLIC OpenMP::OpenMP_CXX)
endif()
target_compile_options(test PRIVATE -std=c++17 -Wno-parentheses-equality -Wno-ignored-attributes -march=native -maes -msse2 -msse4.1 -fopenmp)


add_executable(p0)
target_sources(p0 PRIVATE ${PROJECT_SOURCE_DIR}/tests/protocol/p0.cpp)
target_include_directories(p0 PRIVATE
deps/cryptoTools
deps/EzPC
${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(p0 PRIVATE dpf )
if(OpenMP_CXX_FOUND)
    target_link_libraries(p0 PUBLIC OpenMP::OpenMP_CXX)
endif()
target_compile_options(p0 PRIVATE -std=c++17 -Wno-parentheses-equality -Wno-ignored-attributes -march=native -maes -msse2 -msse4.1 -fopenmp)

add_executable(p1)
target_sources(p1 PRIVATE ${PROJECT_SOURCE_DIR}/tests/protocol/p1.cpp)
target_include_directories(p1 PRIVATE
deps/cryptoTools
deps/EzPC
${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(p1 PRIVATE dpf )
if(OpenMP_CXX_FOUND)
    target_link_libraries(p1 PUBLIC OpenMP::OpenMP_CXX)
endif()
target_compile_options(p1 PRIVATE -std=c++17 -Wno-parentheses-equality -Wno-ignored-attributes -march=native -maes -msse2 -msse4.1 -fopenmp)

add_executable(p2)
target_sources(p2 PRIVATE ${PROJECT_SOURCE_DIR}/tests/protocol/p2.cpp)
target_include_directories(p2 PRIVATE
deps/cryptoTools
deps/EzPC
${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(p2 PRIVATE dpf )
if(OpenMP_CXX_FOUND)
    target_link_libraries(p2 PUBLIC OpenMP::OpenMP_CXX)
endif()
target_compile_options(p2 PRIVATE -std=c++17 -Wno-parentheses-equality -Wno-ignored-attributes -march=native -maes -msse2 -msse4.1 -fopenmp)

add_executable(c)
target_sources(c PRIVATE ${PROJECT_SOURCE_DIR}/tests/protocol/c.cpp)
target_include_directories(c PRIVATE
deps/cryptoTools
deps/EzPC
${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(c PRIVATE dpf )
if(OpenMP_CXX_FOUND)
    target_link_libraries(c PUBLIC OpenMP::OpenMP_CXX)
endif()
target_compile_options(c PRIVATE -std=c++17 -Wno-parentheses-equality -Wno-ignored-attributes -march=native -maes -msse2 -msse4.1 -fopenmp)