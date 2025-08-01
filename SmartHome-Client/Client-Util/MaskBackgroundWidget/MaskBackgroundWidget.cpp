#include "MaskBackgroundWidget.h"
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QPainterPath>

MaskBackgroundWidget::MaskBackgroundWidget(QWidget* parent)
    : QWidget(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground); //子窗口圆角父类背景需透明
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MaskBackgroundWidget::setBackground(const QPixmap& pixmap, QColor color)
{
    m_bgPixmap = pixmap;
    m_maskColor = color;
    update();
}

void MaskBackgroundWidget::setWidgetRadius(int borderRadius)
{
    m_borderRadius = borderRadius;
    update();
}

void MaskBackgroundWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform); // 抗锯齿，图片更清晰
    painter.setRenderHint(QPainter::Antialiasing); // 圆角边缘抗锯齿（关键）

    QRect expandRect = this->rect().adjusted(-2, -2, -2, -2);

    QPainterPath path; // 定义绘制路径
    // 绘制一个带圆角的矩形（x=0, y=0, 宽=窗口宽, 高=窗口高, 圆角半径）
    path.addRoundedRect(expandRect, m_borderRadius, m_borderRadius);
    painter.setClipPath(path); // 将绘制范围限制在圆角路径内（超出部分不显示）

    // 1. 绘制背景图（缩放适配窗口大小）
    if (!m_bgPixmap.isNull()) {
        QPixmap scaledBg = m_bgPixmap.scaled(
            expandRect.size(),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation // 平滑缩放
        );
        painter.drawPixmap(0, 0, scaledBg);
    }

    // 2. 绘制半透明蒙层（覆盖整个窗口）
    painter.fillRect(expandRect, m_maskColor);

    QWidget::paintEvent(event);
}

