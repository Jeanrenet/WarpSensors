#include "CSensors.h"

#include <QFile>
#include <QString>
#include <QDataStream>
#include <QDebug>

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
    this->create(88, QSharedMemory::ReadWrite);

    //récupération des échelles
    m_accelerometerScale = stringToValue(ACCEL_SCALE);
    m_magnetometerScale = stringToValue(MAGNET_SCALE);
    m_barometerScale = stringToValue(PRESSURE_SCALE);
    m_temperatureScale = stringToValue(TEMP_SCALE);
    m_gyroscopeScale = stringToValue(GYRO_SCALE);

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
    //lecture des données et application de l'échelle
    m_accelerometerX = stringToValue(ACCEL_X) * m_accelerometerScale;
    m_accelerometerY = stringToValue(ACCEL_Y) * m_accelerometerScale;
    m_accelerometerZ = stringToValue(ACCEL_Z) * m_accelerometerScale;
    m_magnetometerX = stringToValue(MAGNET_X) * m_magnetometerScale;
    m_magnetometerY = stringToValue(MAGNET_Y) * m_magnetometerScale;
    m_magnetometerZ = stringToValue(MAGNET_Z) * m_magnetometerScale;
    m_pressure = stringToValue(PRESSURE_RAW) * m_barometerScale;
    m_temperature = stringToValue(TEMP_RAW) * m_temperatureScale;
    m_gyroscopeX = stringToValue(GYRO_X) * m_gyroscopeScale;
    m_gyroscopeY = stringToValue(GYRO_Y) * m_gyroscopeScale;
    m_gyroscopeZ = stringToValue(GYRO_Z) * m_gyroscopeScale;

    if (this->isAttached()) //vérifier si le segment est attaché
    {
        //écriture sur la mémoire partagée
        QByteArray sharedData;
        QDataStream stream (&sharedData, QIODevice::WriteOnly);
        stream << m_temperature;
        stream << m_pressure;
        stream << m_accelerometerX;
        stream << m_accelerometerY;
        stream << m_accelerometerZ;
        stream << m_magnetometerX;
        stream << m_magnetometerY;
        stream << m_magnetometerZ;
        stream << m_gyroscopeX;
        stream << m_gyroscopeY;
        stream << m_gyroscopeZ;

        this->lock();
        char *to = (char*)this->data();
        //copie de la donnée sur le segment de mémoire
        memcpy(to, sharedData.data(), qMin(this->size(), (qint32) sharedData.size()));
        this->unlock();
    }
    else
        this->attach(); //attacher le segment
}
