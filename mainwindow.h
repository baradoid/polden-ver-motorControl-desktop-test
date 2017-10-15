#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}
#define MOTOR_COUNT 3
typedef enum {
    MOVE_UP, MOVE_DOWN
} TMoveDir;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    int pos[MOTOR_COUNT];
    TMoveDir md[MOTOR_COUNT];
private slots:
    void handleTimer();
    void on_pushButtonStart_clicked();
    void on_pushButtonReset_clicked();
};

#endif // MAINWINDOW_H
