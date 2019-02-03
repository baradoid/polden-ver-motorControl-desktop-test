#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QUdpSocket>
#include <QString>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

    //qDebug() << posSin;
    //qDebug() << QTime::currentTime().toString("mm:ss:zzz");
    QString msg;
//    msg.sprintf("S0p%03dS1p%03dS2p%03dS2p%03dS2p%03dS2p%03dS2p%03dS2p%03d\r\n",
//                posSin,
//                posSin,
//                posSin,
//                posSin,
//                posSin,
//                posSin,
//                posSin);
    for(int i=0;i<MOTOR_COUNT;i++){
        int posSin =(int)(maxPcnt * (qCos((motPhase[i]/60.)*M_PI)-1)/-2);
        QString ss;
        ss.sprintf("S0p%03d", posSin);
        msg.append(ss);

        if(skipPhase[i] > 0)
            skipPhase[i]--;
        else
            motPhase[i]++;
    }
    qDebug() << qPrintable(msg);
    msg.append("\r\n");
    QUdpSocket s;
    if(s.writeDatagram(msg.toLatin1(), /*QHostAddress("192.168.3.152")*/QHostAddress::LocalHost, 8161) == -1)
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
   // k = 0;
    for(int i=0; i<MOTOR_COUNT; i++){
        pos[i] = 0;
        md[i] = MOVE_UP;
        motPhase[i] = 0;
        skipPhase[i] = i*8;
    }
    ui->lineCurrenHeight->setText(QString::number(pos[0]));
}
