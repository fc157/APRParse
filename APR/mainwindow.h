#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fileoperation.h"
#include "dataitem.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showParseResult();

public slots:
    void openFile();
    void lastClicked();
    void nextClicked();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void addItem(const QString& can_data, const QString& parse_data);
    Ui::MainWindow *ui;
    FileOperation* mfile;
    int current_index = 0;
    int current_page_size = 0;
};
#endif // MAINWINDOW_H
