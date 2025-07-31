# DataCenter 模块的 CMake 配置

# 创建一个静态库或者共享库
add_library(Client-DataCenter STATIC
   LoginUserManager.h   LoginUserManager.cpp
   TokenManager.h      TokenManager.cpp   
)


   # 设置包含目录（确保所有模块能互相访问头文件）
target_include_directories(Client-DataCenter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Client-DataCenter PRIVATE  
    Client-NetWork Client-Util  
    Qt6::Core Qt6::Widgets Qt6::Network
)
