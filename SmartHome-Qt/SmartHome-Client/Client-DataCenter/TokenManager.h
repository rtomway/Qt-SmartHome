#ifndef TOKENMANAGER_H_
#define TOKENMANAGER_H_

#include <QString>

class TokenManager
{
public:
	static void saveToken(const QString& token);
	static QString getToken();
};

#endif // !TOKENMANAGER_H_
