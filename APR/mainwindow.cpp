#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mfile = new FileOperation();
    if(!mfile->parseInit())
    {
        ui->file_path->setText("APR parse file open failed");
    }

    connect(ui->open_file, &QPushButton::clicked, this , &MainWindow::openFile);
    connect(mfile, &FileOperation::parseSuccess, this , &MainWindow::showParseResult);
    connect(ui->last_btn, &QPushButton::clicked, this , &MainWindow::lastClicked);
    connect(ui->next_btn, &QPushButton::clicked, this , &MainWindow::nextClicked);
}

MainWindow::~MainWindow()
{
    delete mfile;
    delete ui;
}

void MainWindow::openFile()
{
    current_index = 0;
    current_page_size = 0;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    bool res;
    if(!fileNames.empty())
    {
        qDebug() << "file path:" <<fileNames[0];
        //解析
        res = mfile->openFile(fileNames[0]);
        if(res)
        {
            ui->file_path->setText(fileNames[0]);
        }
        else
        {
            QMessageBox::about(this, "fail", "open file failed");
        }
    }
}

void MainWindow::showParseResult()
{
    QVector<DataStruct> data_list;
    mfile->getData(data_list);
    if( current_index >= data_list.size())
    {
        return;
    }
    ui->listWidget->clear();
    current_page_size = 0;
    for(; current_index < data_list.size() && current_page_size < MAX_PAGE_SIZE; current_index++)
    {
        addItem(data_list[current_index].data, data_list[current_index].parse_data);
        current_page_size++;
    }
}

void MainWindow::addItem(const QString& can_data, const QString& parse_data)
{
    DataItem* pItemWidget = new DataItem(this);
    QSize item_size = pItemWidget->SetData(can_data, parse_data);
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(item_size);
    ui->listWidget->addItem(pItem);
    ui->listWidget->setItemWidget(pItem, pItemWidget);
}

void MainWindow::lastClicked()
{
    qDebug() << "lastClicked: current_index = " << current_index;
    if(current_index > MAX_PAGE_SIZE)
    {
        current_index -= (current_page_size+MAX_PAGE_SIZE);
        showParseResult();
    }
}
void MainWindow::nextClicked()
{
    //qDebug() << "nextClicked";
    showParseResult();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << this->width() << this->height();
    int width = this->width();
    int height = this->height();
    ui->last_btn->move(width/2 - 150, height - 80);
    ui->next_btn->move(width/2 + 150, height - 80);
    ui->listWidget->setFixedSize(width,  height - 160);
}



