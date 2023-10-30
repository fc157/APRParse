#include "dataitem.h"
#include "ui_dataitem.h"
#include <QDebug>
DataItem::DataItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataItem)
{
    ui->setupUi(this);
    font = new QFont("Microsoft YaHei",14);
}

DataItem::~DataItem()
{
    delete font;
    delete ui;
}

QSize DataItem::SetData(const QString& can_data, const QString& parse_data)
{
    //设置label背景颜色 ， 字体颜色
    ui->can_data->setFont(*font);
    ui->parse_data->setFont(*font);

    ui->can_data->setText(can_data);
    ui->parse_data->setText(parse_data);

    ui->parse_data->adjustSize();
    //qDebug() << "ui->parse_data size :" << ui->parse_data->size();
    //qDebug() << "DataItem size :" << DataItem::size();

    int can_data_width = ui->can_data->size().width();
    int parse_data_width = ui->parse_data->size().width();
    int max = (parse_data_width > can_data_width ? parse_data_width:can_data_width) + 50;
    setFixedSize(max, 75);
    return QSize(max, 75);
}
