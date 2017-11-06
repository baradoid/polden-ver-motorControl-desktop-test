#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QUdpSocket>
#include <QString>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    k(0)
{
    ui->setupUi(this);
    connect(&timer, SIGNAL(timeout()), this, SLOT(handleTimer()));
    timer.setInterval(100);
    on_pushButtonReset_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleTimer()
{    
    int maxPcnt = ui->lineEditMaxPcnt->text().toInt();
    int posSin =(int)(maxPcnt * (qCos((k++/100.)*M_PI)-1)/-2);
    qDebug() << posSin;
    //qDebug() << QTime::currentTime().toString("mm:ss:zzz");
    QString msg;
    QString ss;
    ss.sprintf("S0p%03d", posSin);
//    msg.sprintf("S0p%03dS1p%03dS2p%03dS2p%03dS2p%03dS2p%03dS2p%03dS2p%03d\r\n",
//                posSin,
//                posSin,
//                posSin,
//                posSin,
//                posSin,
//                posSin,
//                posSin);
    for(int i=0;i<10;i++){
        msg.append(ss);
    }
    msg.append("\r\n");
    QUdpSocket s;
    if(s.writeDatagram(msg.toLatin1(), QHostAddress::LocalHost, 8051) == -1)
        qDebug() << "sendErr";

    for(int i=0; i<MOTOR_COUNT; i++){
        if(md[i] == MOVE_UP){
            pos[i] += 10;
            if(pos[i]>=900)
                md[i] = MOVE_DOWN;
        }
        else if(md[i] == MOVE_DOWN){
            pos[i] -= 10;
            if(pos[i] <100)
                md[i] = MOVE_UP;
        }
    }

    ui->lineCurrenHeight->setText(QString::number(pos[0]));
}

void MainWindow::on_pushButtonStart_clicked()
{
    if(ui->pushButtonStart->text().compare("start") == 0){
        timer.start();
        ui->pushButtonStart->setText("stop");
    }
    else if(ui->pushButtonStart->text().compare("stop") == 0){
        timer.stop();
        ui->pushButtonStart->setText("start");
    }
}

void MainWindow::on_pushButtonReset_clicked()
{
    k = 0;
    for(int i=0; i<MOTOR_COUNT; i++){
        pos[i] = 0;
        md[i] = MOVE_UP;
    }
    ui->lineCurrenHeight->setText(QString::number(pos[0]));
}
