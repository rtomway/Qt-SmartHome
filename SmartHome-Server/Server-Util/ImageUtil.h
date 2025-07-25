#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QImage>
#include <QSize>
#include "GlobalTypes.h"

class ImageUtils {
public:
	static QString getUserAvatarFolderPath();
	static QString getGroupAvatarFolderPath();
	static void saveAvatarToLocal(const QString& avatarPath, const QString& id,  std::function<void()>callBack = nullptr);
	static void saveAvatarToLocal(const QImage& image, const QString& id, std::function<void()>callBack = nullptr);
	static void saveAvatarToLocal(const QByteArray& data, const QString& id, std::function<void()>callBack = nullptr);
	static QByteArray loadImage(const QString& user_id);
private:
	static bool saveAvatarToLocalTask(const QString& avatarPath, const QString& id);
	static bool saveAvatarToLocalTask(const QImage& image, const QString& id);
	static bool saveAvatarToLocalTask(const QByteArray& data, const QString& id);
};

#endif // IMAGEUTILS_H