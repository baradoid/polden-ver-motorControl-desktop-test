#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QUdpSocket>
#include <QString>

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
    //qDebug() << QTime::currentTime().toString("mm:ss:zzz");
    QString msg;
    msg.sprintf("S0p%03dS1p%03dS2p%03d\r\n", pos[0], pos[1], pos[2]);
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
    for(int i=0; i<MOTOR_COUNT; i++){
        pos[i] = 0;
        md[i] = MOVE_UP;
    }
}
