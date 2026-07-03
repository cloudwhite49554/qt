#ifndef FISHINGWIDGET_H
#define FISHINGWIDGET_H

#include <QWidget>

class FishingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FishingWidget(QWidget *parent = nullptr);
    void setLine(const QPoint &from, const QPoint &to);
    bool hasLine=true;

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QPoint base;
    QPoint tip;

};

#endif // FISHINGWIDGET_H