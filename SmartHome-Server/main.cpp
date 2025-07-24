﻿#include <QCoreApplication>
#include <QRunnable>
#include <QThreadPool>
#include <iostream>
#include "Server-DataBase/SSqlConnectionPool.h"
#include "Server.h"

// Main function
int main(int argc, char* argv[])
{ 
    QCoreApplication a(argc, argv);

    //线程不超过连接数 保证线程绑定连接
    auto maxCount = SSqlConnectionPool::instance()->getMaxConnnectionCount();
    QThreadPool::globalInstance()->setMaxThreadCount(maxCount);

    SSqlConnectionPool::instance()->setPort(3306);
    SSqlConnectionPool::instance()->setHostName("localhost");
    SSqlConnectionPool::instance()->setDatabaseName("SmartHome");
    SSqlConnectionPool::instance()->setUserName("xu");
    SSqlConnectionPool::instance()->setPassword("200600xx");

    Server server;
    
    return a.exec();
}



