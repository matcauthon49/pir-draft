cmake_minimum_required(VERSION 3.13)
project(PIR)

# include(${CMAKE_CURRENT_LIST_DIR})

add_library(dpf STATIC
deps/cryptoTools/cryptoTools/Common/Defines.cpp
deps/cryptoTools/cryptoTools/Common/Log.cpp
deps/cryptoTools/cryptoTools/Crypto/AES.cpp
deps/cryptoTools/cryptoTools/Crypto/PRNG.cpp
dpf.cpp
)


# #find_package(Threads REQUIRED)
target_include_directories(dpf
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:./>)

target_include_directories(dpf PRIVATE 
deps/cryptoTools
deps/EzPC)

target_compile_options(dpf PRIVATE -std=c++17 -Wno-parentheses-equality -Wno-ignored-attributes -march=native -maes -msse2 -msse4.1)
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
    DESTINATION include
)
install(DIRECTORY
    "deps/cryptoTools"
    "deps/EzPC"
    DESTINATION include
)

add_executable(test main.cpp)
target_include_directories(test PRIVATE
deps/cryptoTools
deps/EzPC
${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(test PRIVATE dpf)
target_compile_options(test PRIVATE -std=c++17 -Wno-parentheses-equality -Wno-ignored-attributes -march=native -maes -msse2 -msse4.1)