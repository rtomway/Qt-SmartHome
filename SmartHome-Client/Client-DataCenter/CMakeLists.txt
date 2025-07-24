# DataCenter 模块的 CMake 配置

# 创建一个静态库或者共享库
add_library(Client-DataCenter STATIC
    LoginUserDataMoudle/LoginUserManager.h    LoginUserDataMoudle/LoginUserManager.cpp

    TokenManager/TokenManager.h         TokenManager/TokenManager.cpp   
)


   # 设置包含目录（确保所有模块能互相访问头文件）
target_include_directories(Client-DataCenter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/LoginUserDataMoudle
    ${CMAKE_CURRENT_SOURCE_DIR}/TokenManager
)

target_link_libraries(Client-DataCenter PRIVATE  
    Client-NetWork Client-Util  Client-Core
    Qt6::Core Qt6::Widgets Qt6::WebSockets Qt6::Network
)
