#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QImage>
#include <QSize>
#include <QGuiApplication> 
#include <QThread>
#include <QPixmap>


class ImageUtils {
public:
	static QPixmap roundedPixmap(const QPixmap& image, QSize size, int radius);
	static QPixmap roundedPixmap(const QPixmap& image, QSize size);
	static void getUserAvatar(const QString& user_id, std::function<void(QPixmap)>callBack);

	static void setLoginUser(const QString&user_id);
	static QString getUserAvatarFolderPath();
	static void saveAvatarToLocal(const QString& avatarPath, const QString& id,  std::function<void()>callBack=nullptr);
	static void saveAvatarToLocal(const QImage& image, const QString& id, std::function<void()>callBack=nullptr);
	static void saveAvatarToLocal(const QByteArray& data, const QString& id, std::function<void()>callBack=nullptr);
	static void loadAvatarFromFile(const QString& avatarPath, std::function<void(QImage)>callBack);
private:
	static bool saveAvatarToLocalTask(const QString& avatarPath, const QString& id);
	static bool saveAvatarToLocalTask(const QImage& image, const QString& id);
	static bool saveAvatarToLocalTask(const QByteArray& data, const QString& id);
	static QImage loadAvatarFromFileTask(const QString& avatarPath);
	static QString m_currentUser;
};

#endif // IMAGEUTILS_H