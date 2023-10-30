#include "fileoperation.h"
#include <QDebug>
#include <QApplication>
FileOperation::FileOperation()
{
    data_list = new QVector<DataStruct>();
    parse_list = new QVector<ParseStruct>();


}
FileOperation::~FileOperation()
{
    delete parse_list;
    delete data_list;
}

bool FileOperation::parseInit()
{
    QFile in_file(QApplication::applicationDirPath() + "/APR.csv");
    qDebug() << QApplication::applicationDirPath();

    if (in_file.open(QIODevice::ReadOnly))
    {
        QTextStream stream_text(&in_file);
        QStringList split;
        ParseStruct ps;
        while (!stream_text.atEnd())
        {
            split = stream_text.readLine().split(",");//列数据
            ps.signal = split[0];
            ps.offset = split[1].toInt();
            ps.precision = split[2].toFloat();
            ps.start_position = split[3].toInt();
            ps.length = split[4].toInt();
            parse_list->append(ps);
            //qDebug() << split.size();
        }
        in_file.close();
        return true;
    }
    in_file.close();
    return false;
}

bool FileOperation::openFile(QString file_path)
{
    qDebug() << "FileOperation::openFile";
    current_df_index = 0;
    current_precision_index;
    is_parse = false;
    QFile in_file(file_path);
    QStringList lines_data;
    if (in_file.open(QIODevice::ReadOnly))
    {
        QTextStream stream_text(&in_file);
        while (!stream_text.atEnd())
        {
            lines_data.push_back(stream_text.readLine());
        }
        parse(lines_data);
    }
    else
    {
        qDebug() << "open fail";
        return false;
    }
    qDebug() << "open success: ";
    in_file.close();
    return true;
}

void FileOperation::parse(QStringList& lines_data)
{
    qDebug() << "FileOperation::parse";
    data_list->clear();
    current_df_index = 0;
    if(lines_data.size() <=0)
        return;
    QString line = lines_data.at(0);
    QStringList split = line.split(",");//列数据
    if(split.size() < 21 || split[13] != "MAKE_CAN_ID(HEX)")
        return;
    for (int var = 1; var < lines_data.size(); var++)
    {
        QString line = lines_data.at(var);
        QStringList split = line.split(",");//列数据

        if(split[13] == "0xdf")
        {
            //qDebug() << "split[20] : " << split[20];
            parseDF(split[20]);
            current_df_index++;
            if(current_df_index == MAX_PAGE_SIZE)
                emit parseSuccess();
        }
    }
    qDebug() << "FileOperation::parse end";
}

void FileOperation::parseDF(QString& data)
{
    if(data.contains("f5 01",Qt::CaseInsensitive))
    {
        is_parse = true;
        current_precision_index = 1;
    }
    //&& current_df_index == 1
    QString parse_data = "";
    if( is_parse )
    {
        if(data.contains("f3 01",Qt::CaseInsensitive)== true)
        {
            parse_data = "PrivateCan data start";
        }
        else if(data.contains("f3 02",Qt::CaseInsensitive)== true)
        {
            parse_data = "PrivateCan data end";
        }
        else if(data.contains("f1 01",Qt::CaseInsensitive)== true)
        {
            parse_data = "radar can data start";
        }
        else if(data.contains("f1 02",Qt::CaseInsensitive)== true)
        {
            parse_data = "radar can data end";
        }
        else {
            QStringList split = data.split(" ");
            //qDebug() <<  "split.size():" <<split.size();
            quint64 hexnum =0;
            QString hexMessage = "";
            ParseStruct ps;
            for (int var = 0; var < split.size() -1; ++var) {
                quint64 temp = split[var].toInt(nullptr,16);
                temp = temp << (8*var);
                hexnum = temp + hexnum;
            }
            //qDebug() << "hexnum <<" << hexnum;
            hexMessage = hexMessage.setNum(hexnum,2);
            parseSignal(hexMessage, parse_data);
        }
    }
    data_list->append(DataStruct(data, parse_data));//这行一定要执行，不管什么条件
    if(data.contains("f5 02",Qt::CaseInsensitive))
    {
        is_parse = false;
    }
}

void FileOperation::parseSignal(QString& hexMessage ,QString& parse_data)
{
    ParseStruct ps;
    int last_signal_position = 0;
    quint64 hexnum =0;
    QString data;
    for(; current_precision_index < parse_list->size(); current_precision_index++)
    {
        ps = (*parse_list)[current_precision_index];
        if(ps.start_position > last_signal_position)
        {
            //can报文长度-开始位置 - 信号长度- (64 - size)
            //减去64 - size是因为左边零会省略，所以总体长度可能不到64
            //int start_position = CAN_LENGTH - ps.position - parse_list[current_df_index].length - (CAN_LENGTH - hexMessage.size());
            int start_position = hexMessage.size() - ps.start_position - ps.length;
            data = hexMessage.mid( start_position, ps.length);
            //qDebug() << "hexMessage mid :::" << hexMessage;

            hexnum = data.toInt(nullptr,2);
            //qDebug() << ps.signal << ":" << data << "::" << hex << hexnum;

            float phsical = hexnum*ps.precision + ps.offset;
            last_signal_position = ps.start_position;
            parse_data += ps.signal;
            parse_data += "=";
            parse_data += QString::number(phsical);
            parse_data += "; ";
        }
        else {
            break;
        }
    }
}

void FileOperation::getData(QVector<DataStruct>& data_list)
{
    data_list = *(this->data_list);
}
