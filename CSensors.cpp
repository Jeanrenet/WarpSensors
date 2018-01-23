#include "CSensors.h"

#include <QFile>
#include <QString>
#include <QDataStream>

double stringToValue(QString path)
{
    double value = 0;

    QFile file(path);
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray array = file.readAll().trimmed();

            value = array.toDouble();
            file.close();
        }
    }
    return value;
}
CSensors::CSensors()
{
    //initialisation mémoire partagée
    this->setKey("DataExchangeMemory");
    this->create(52, QSharedMemory::ReadWrite);

    //récupération des échelles
    m_accelerometerScale = stringToValue(ACCEL_SCALE);
    m_magnetometerScale = stringToValue(MAGNET_SCALE);

    //connexion timer et démarrage
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(readData()));
    m_timer.start(10);
}

CSensors::~CSensors()
{
    this->detach();
}

void CSensors::readData()
{
    //lecture des données
    m_temperature = stringToValue(TEMP);
    m_accelerometerX = stringToValue(ACCEL_X) * m_accelerometerScale;
    m_accelerometerY = stringToValue(ACCEL_Y) * m_accelerometerScale;
    m_accelerometerZ = stringToValue(ACCEL_Z) * m_accelerometerScale;
    m_magnetometerX = stringToValue(MAGNET_X) * m_magnetometerScale;
    m_magnetometerY = stringToValue(MAGNET_Y) * m_magnetometerScale;
    m_magnetometerZ = stringToValue(MAGNET_Z) * m_magnetometerScale;

    if (this->isAttached())
    {
        QByteArray sharedData;
        QDataStream stream (&sharedData, QIODevice::WriteOnly);
        stream << m_temperature;
        stream << m_accelerometerX;
        stream << m_accelerometerY;
        stream << m_accelerometerZ;
        stream << m_magnetometerX;
        stream << m_magnetometerY;
        stream << m_magnetometerZ;

        this->lock();
        char *to = (char*)this->data();
        memcpy(to, sharedData.data(), qMin(this->size(), (qint32) sharedData.size()));
        this->unlock();
    }
    else
        this->attach();
}
