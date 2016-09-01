#include <QDebug>
#include <QGridLayout>
#include <QMessageBox>
#include <QLabel>

#include "controlwindow.h"

ControlWindow::ControlWindow(QWidget *parent)
:   QMainWindow(parent), keyInput(defaultSendCommand)
{
    nameLineEdit = new QLineEdit();
    connectStatusLineEdit = new QLineEdit();
    nameLineEdit->setReadOnly(true);
    connectStatusLineEdit->setReadOnly(true);

    QLabel *bt_label = new QLabel();
    bt_label->setText("Bluetooth output:");

    BTOutputTextEdit = new QTextEdit();
    BTOutputTextEdit->setReadOnly(true);

    connectBtn = new QPushButton();
    connectBtn->setText("Connect");

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLineEdit, 0, 0);
    mainLayout->addWidget(connectStatusLineEdit, 0, 1);
    mainLayout->addWidget(bt_label, 1, 0, 1, 2);
    mainLayout->addWidget(BTOutputTextEdit, 2, 0, 1, 2);
    mainLayout->addWidget(connectBtn, 3, 0, 1, 2);

    QWidget *window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);
    setWindowTitle(tr("Bluetooth Control"));

    connect(connectBtn, &QPushButton::clicked,
            this, &ControlWindow::on_connectBtnClicked);
}

ControlWindow::~ControlWindow()
{
    qDebug() << "ControlWindow destructor";
    delete btSocket;
}

void ControlWindow::on_connectBtnClicked(bool)
{    
    if (QBluetoothLocalDevice().hostMode() == QBluetoothLocalDevice::HostPoweredOff) {
        QMessageBox::information(this, "Bluetooth power", "Check Bluetooth is powered on");
        return;
    }
    connectStatusLineEdit->clear();
    connectBtn->setEnabled(false);
//    QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
    DeviceBTSelect deviceSelect;
    deviceSelect.startScan();
    if ( deviceSelect.exec() == QDialog::Accepted ) {
        QBluetoothDeviceInfo deviceInfo = deviceSelect.deviceInfo();
        nameLineEdit->setText(deviceInfo.name());

        btSocket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
        qDebug() << "Create socket";
        btSocket->connectToService(deviceInfo.address(),
                                   deviceInfo.deviceUuid());
                                   //QBluetoothUuid(serviceUuid));
        qDebug() << "ConnectToService done";
        connectStatusLineEdit->setText("Wait for connect");

        connect(btSocket, &QBluetoothSocket::connected,
                this, &ControlWindow::btSocketConnected);

        connect(btSocket, &QBluetoothSocket::disconnected,
                this, &ControlWindow::btSocketDisconnected);

        connect(btSocket, static_cast<void(QBluetoothSocket::*)(QBluetoothSocket::SocketError)>(&QBluetoothSocket::error),
                this, &ControlWindow::btSocketError);

        connect(btSocket, &QBluetoothSocket::readyRead,
                this, &ControlWindow::btSocketReadyRead);
    }

    connectBtn->setEnabled(true);
}

void ControlWindow::btSocketDisconnected()
{
    connectStatusLineEdit->setText("Disconnected from" + btSocket->peerName());
    delete senderTimer;
}

void ControlWindow::btSocketConnected()
{
    connectStatusLineEdit->setText("Connected to " + btSocket->peerName());

//    senderTimer = new QTimer();
//    senderTimer->setInterval(100);
//    connect(senderTimer, &QTimer::timeout,
//            this, &ControlWindow::btSendCommand);
//    senderTimer->start(100);
}

void ControlWindow::btSocketError(QBluetoothSocket::SocketError error)
{
    connectStatusLineEdit->setText("Error: " + QString::number(error));
    qDebug() << connectStatusLineEdit->text();
    delete btSocket;
}

void ControlWindow::btSocketReadyRead()
{
    outputBuffer.append(btSocket->readAll());
    if ( outputBuffer.endsWith("\n") ) {
        BTOutputTextEdit->append(outputBuffer.left(outputBuffer.length()));
        outputBuffer.clear();
    }
}

void ControlWindow::btSendCommand()
{
    if ( !btSocket || !btSocket->isOpen() )
        return;

    qDebug() << "Send: " << keyInput;
    btSocket->write(&keyInput, 1);
}

void ControlWindow::keyPressEvent(QKeyEvent *event)
{
    keyInput = event->key();
    qDebug() << "Write: " << keyInput;
    btSendCommand();
}
