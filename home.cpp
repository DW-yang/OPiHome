#include "home.h"
#include "ui_home.h"

#include <QDateTime>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QFile>
#include <QTextStream>
#include <QDebug>

home::home(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::home)
{
    ui->setupUi(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(refresh()));
    timer->start(1000);
}

home::~home()
{
    delete ui;
}

void home::init(){
    QList<QHostAddress> addrs = QNetworkInterface::allAddresses();
    foreach(QHostAddress addr,addrs)
    {
        if(addr != QHostAddress::LocalHost && addr.protocol() == QAbstractSocket::IPv4Protocol && !addr.toString().startsWith("169"))
        {
            ui->ip_list->addItem(addr.toString());
        }
    }
}

void home::refresh()
{
    QDateTime *datetime = new QDateTime(QDateTime::currentDateTime());
    QString time = datetime->time().toString();
    QString date = datetime->date().toString();
    QString cpu_temp;
    QFile file;
    file.setFileName("/sys/class/thermal/thermal_zone0/temp");
    if(!file.open(QIODevice::ReadOnly))
    {
        cpu_temp = file.errorString();
    }else{
        QTextStream in(&file);
        cpu_temp = in.readLine().mid(0,2);
        cpu_temp += " ℃";
    }
    ui->time->setText(time);
    ui->date->setText(date);
    ui->cpu_temp->setText(cpu_temp);
    ui->hostname->setText(QHostInfo::localHostName());
    ui->ip_list->clear();
    init();
}


