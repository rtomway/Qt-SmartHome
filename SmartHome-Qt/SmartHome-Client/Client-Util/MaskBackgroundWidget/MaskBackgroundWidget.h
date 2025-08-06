#ifndef MASKBACKGROUNDWIDGET_H_
#define MASKBACKGROUNDWIDGET_H_

#include <QWidget>
#include <QPainter>
#include <QPixmap>

class MaskBackgroundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MaskBackgroundWidget(QWidget* parent = nullptr);
    void setBackground(const QPixmap& pixmap, QColor color);
    void setWidgetRadius(int borderRadius);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    QPixmap m_bgPixmap; // 背景图
    QColor m_maskColor; // 蒙层颜色（含透明度）
    int m_borderRadius = 10;   // 圆角半径（新增）
};


#endif // !MASKBACKGROUNDWIDGET_H_



