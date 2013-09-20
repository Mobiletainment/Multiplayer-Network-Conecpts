

## simply add directories
message(STATUS "- adding subdirectory poco-develop")
add_subdirectory(poco-develop)

message(STATUS "- adding subdirectory RakNet_PC-4.08")
add_subdirectory(RakNet_PC-4.08)

message(STATUS "- adding subdirectory cocos2d-x-2.1.4")
add_subdirectory(cocos2d-x-2.1.4)




message(STATUS "- adding subdirectory stateless-develop")
include(${NL_MACRO_DIRECTORY}/nlMacros.cmake)

add_subdirectory(stateless-develop)
