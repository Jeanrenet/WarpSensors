#ifndef CSENSORS_H
#define CSENSORS_H

#include <QSharedMemory>
#include <QTimer>

#define TEMP         "/sys/bus/iio/devices/iio:device1/in_temp_input"
#define ACCEL_X      "/sys/bus/iio/devices/iio:device1/in_accel_x_raw"
#define ACCEL_Y      "/sys/bus/iio/devices/iio:device1/in_accel_y_raw"
#define ACCEL_Z      "/sys/bus/iio/devices/iio:device1/in_accel_z_raw"
#define ACCEL_SCALE  "/sys/bus/iio/devices/iio:device1/in_accel_scale"
#define MAGNET_X     "/sys/bus/iio/devices/iio:device1/in_magn_x_raw"
#define MAGNET_Y     "/sys/bus/iio/devices/iio:device1/in_magn_y_raw"
#define MAGNET_Z     "/sys/bus/iio/devices/iio:device1/in_magn_z_raw"
#define MAGNET_SCALE "/sys/bus/iio/devices/iio:device1/in_magn_scale"

class CSensors: public QSharedMemory
{
    Q_OBJECT
public:
    CSensors();
    ~CSensors();

protected slots:
    void readData();

private:
    quint32         m_temperature;
    double          m_accelerometerX;
    double          m_accelerometerY;
    double          m_accelerometerZ;
    double          m_accelerometerScale;
    double          m_magnetometerX;
    double          m_magnetometerY;
    double          m_magnetometerZ;
    double          m_magnetometerScale;

    QTimer m_timer;
};

#endif // CSENSORS_H
