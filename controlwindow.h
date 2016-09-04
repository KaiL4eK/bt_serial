#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QKeyEvent>
#include <QBluetoothSocket>
#include <QTextEdit>
#include <QTimer>

#include "devicebtselect.h"

namespace Ui {
class ControlWindow;
}

class ControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControlWindow(QWidget *parent = 0);
    ~ControlWindow();
private slots:

    void btSocketConnected();
    void btSocketError(QBluetoothSocket::SocketError);
    void btSocketDisconnected();
    void btSocketReadyRead();
    void btSendCommand();
private:
    QLineEdit           *nameLineEdit;
    QLineEdit           *connectStatusLineEdit;
    QTextEdit           *BTOutputTextEdit;
    QBluetoothSocket    *btSocket;
    QString             outputBuffer;

    char                keyInput;
    int                 defaultSendCommand = 1;
    bool                connected;

    bool chooseConnectionDevice();
    void keyPressEvent(QKeyEvent *event);
};

#endif // CONTROLWINDOW_H
