#include <QGridLayout>

#include "devicebtselect.h"

DeviceBTSelect::DeviceBTSelect(QWidget *parent)
:   QDialog(parent), localDevice(new QBluetoothLocalDevice)
{
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

    devicesList = new QListWidget();

    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &DeviceBTSelect::addDevice);

    connect(devicesList, &QListWidget::itemActivated,
            this, &DeviceBTSelect::itemActivated);

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(devicesList, 0, 0);

    setLayout(mainLayout);

}

DeviceBTSelect::~DeviceBTSelect()
{
    delete m_discoveryAgent;
}

QBluetoothDeviceInfo &DeviceBTSelect::deviceInfo()
{
    return( m_deviceInfo );
}

void DeviceBTSelect::startScan()
{
    if ( m_discoveryAgent->isActive() )
        m_discoveryAgent->stop();

    m_discoveryAgent->start();
}

void DeviceBTSelect::addDevice(const QBluetoothDeviceInfo &info)
{
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
    QList<QListWidgetItem *> items = devicesList->findItems(label, Qt::MatchExactly);
    if (items.empty()) {
        QListWidgetItem *item = new QListWidgetItem(label);
        QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
        if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
            item->setTextColor(QColor(Qt::green));
        else
            item->setTextColor(QColor(Qt::black));

        devicesList->addItem(item);
        m_discoveredDevices.insert(item, info);
    }
}

void DeviceBTSelect::itemActivated(QListWidgetItem *item)
{
    m_deviceInfo = m_discoveredDevices.value(item);
    if ( m_discoveryAgent->isActive() )
        m_discoveryAgent->stop();

    accept();
}

void DeviceBTSelect::closeEvent(QCloseEvent *event)
{
    reject();
    QWidget::closeEvent(event);
}
