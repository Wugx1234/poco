# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h" )
POCO_HEADERS_AUTO( TEST_SRCS ${HDRS_G})

POCO_SOURCES_AUTO_PLAT( TEST_SRCS OFF
    src/WinDriver.cpp
)

POCO_SOURCES_AUTO_PLAT( TEST_SRCS WINCE
    src/WinCEDriver.cpp
)

add_executable(JWT-testrunner ${TEST_SRCS} )
if(ANDROID)
    add_test(NAME JWT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/JWT-testrunner -DTEST_PARAMETER=-all -P ${CMAKE_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake)
else()
    add_test(NAME JWT WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND JWT-testrunner -all)
endif()
target_link_libraries(JWT-testrunner PUBLIC Poco::JWT Poco::Crypto CppUnit)
if(UNIX AND NOT ANDROID)
    target_link_libraries(JWT-testrunner PUBLIC pthread)
endif(UNIX AND NOT ANDROID)