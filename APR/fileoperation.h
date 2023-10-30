#ifndef FILEOPERATION_H
#define FILEOPERATION_H
#include <QString>
#include <QPair>
#include <QFile>
#include <QVector>
#include <QObject>
#include <QStringList>

#define MAX_PAGE_SIZE 20
#define CAN_LENGTH 64

typedef struct DataStruct{
    DataStruct()
    {
        data = "";
        parse_data = "";
    }
    DataStruct(QString data, QString parse_data)
    {
        this->data = data;
        this->parse_data = parse_data;
    }
    QString data;
    QString parse_data;
};

typedef struct ParseStruct{
    ParseStruct()
    {
        signal = "";
        offset = 0;
        precision = 0;
        start_position = 0;
        length = 0;
    }
    ParseStruct(QString signal, int offset, float precision, int start_position, int length)
    {
        this->signal = signal;
        this->offset = offset;
        this->precision = precision;
        this->start_position = start_position;
        this->length = length;
    }
    QString signal;
    int offset;
    float precision;
    int start_position;
    int length;
};

class FileOperation : public QObject
{
    Q_OBJECT    //必须使用宏Q_OBJECT
public:
    FileOperation();
    ~FileOperation();
    bool openFile(QString file_path);
    void parse(QStringList& lines_data);
    void getData(QVector<DataStruct>& data_list);
    bool parseInit();
signals:
    void parseSuccess();
private:
    void parseSignal(QString& hexMessage ,QString& parse_data);
    void parseDF(QString& data);
    QVector<DataStruct>* data_list;
    QVector<ParseStruct>* parse_list;
    int current_df_index = 0;
    int current_precision_index;
    bool is_parse = false;
    //QStringList lines_data;
};

#endif // FILEOPERATION_H
