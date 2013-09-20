


## on windows we can share the dll main implementation
file (GLOB slDll_Src "stateless/shared/platform/win32/*.cpp")
file (GLOB slDll_Hdrs "stateless/shared/platform/win32/*.h")

## on windows we can share the dll main implementation
file (GLOB slCocos2d_Src "stateless/cocos2d/*.cpp")
file (GLOB slCocos2d_Hdrs "stateless/cocos2d/*.h")

## add shared lib directories

add_subdirectory(stateless/application)
add_subdirectory(stateless/cocosutil)
add_subdirectory(stateless/framework)
add_subdirectory(stateless/network)


