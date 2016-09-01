#ifndef DEVICEBTSELECT_H
#define DEVICEBTSELECT_H

#include <qbluetoothglobal.h>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>

#include <QListView>
#include <QDialog>
#include <QPushButton>
#include <QListWidget>

class DeviceBTSelect : public QDialog
{
    Q_OBJECT

public:
    DeviceBTSelect(QWidget *parent = 0);
    ~DeviceBTSelect();
    QBluetoothDeviceInfo &deviceInfo();
    void startScan();
private slots:
    void addDevice(const QBluetoothDeviceInfo &info);
    void itemActivated(QListWidgetItem *item);
private:
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    QListWidget *devicesList;

    QMap<QListWidgetItem *, QBluetoothDeviceInfo> m_discoveredDevices;
    QBluetoothDeviceInfo m_deviceInfo;

    void closeEvent(QCloseEvent *event);
};

#endif // DEVICEBTSELECT_H
