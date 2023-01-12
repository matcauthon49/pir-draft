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
prng.cpp
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
    prng.h 
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