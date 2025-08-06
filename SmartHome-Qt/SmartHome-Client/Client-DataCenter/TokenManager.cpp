#include "TokenManager.h"
#include "SConfigFile.h"


//保存token
void TokenManager::saveToken(const QString& token)
{
	SConfigFile config("config.ini");
	config.setValue("token", token);
}

//获取token
QString TokenManager::getToken()
{
	SConfigFile config("config.ini");
	return config.value("token").toString();
}
