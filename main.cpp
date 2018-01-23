#include <QCoreApplication>

#include "CSensors.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    new CSensors;

    return a.exec();
}
