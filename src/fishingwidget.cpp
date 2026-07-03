#include "fishingwidget.h"
#include"mainwindow.h"
#include <QPainter>
#include <QPen>

FishingWidget::FishingWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
}
void FishingWidget::setLine(const QPoint &from, const QPoint &to)
{
    base = from;
    tip  = to;
    update(); // ✅ 立刻重画
}
void FishingWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    //if(!hasLine)return;

    p.setPen(QPen(Qt::yellow, 4));
    p.drawLine(base, tip);

}