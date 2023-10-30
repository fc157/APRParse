#ifndef DATAITEM_H
#define DATAITEM_H

#include <QWidget>
#include <QSize>

namespace Ui {
class DataItem;
}

class DataItem : public QWidget
{
    Q_OBJECT

public:
    explicit DataItem(QWidget *parent = nullptr);
    ~DataItem();
    //设置数据
    QSize SetData(const QString& can_data, const QString& parse_data);

private:
    Ui::DataItem *ui;
    QFont* font;
};

#endif // DATAITEM_H
