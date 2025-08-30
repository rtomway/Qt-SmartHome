# NetWork 模块的 CMake 配置


find_package(Qt6 REQUIRED COMPONENTS Mqtt)

# 创建一个静态库或者共享库
add_library(Client-NetWork STATIC
         
     HttpClient.h   HttpClient.cpp
     NetWorkService.h   NetWorkService.cpp
     HttpClientPort.h   
     ExternalHttpClient.h ExternalHttpClient.cpp
     MqttClient.h      MqttClient.cpp
     MessageHandle.h      MessageHandle.cpp
)


# 设置包含目录（确保主程序能访问头文件）
target_include_directories(Client-NetWork PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#链接库
target_link_libraries(Client-NetWork PUBLIC   
    Qt6::Core Qt6::Gui  Qt6::Network Qt6::Mqtt
    Client-Util Client-DataCenter Client-Core
)