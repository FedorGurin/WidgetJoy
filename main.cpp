#include <QApplication>
#include <iostream>
#include "mainwindow.h"
#include "dispatcher.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dispatcher disp;
    MainWindow wind;

    std::cout << "Controller params: " << sizeof(TControllerParams) << std::endl;
    std::cout << "Controller params bitfields: " << sizeof(TControllerParamsB) << std::endl;
    std::cout << "4-pos button" << sizeof(TFourPosButton) << std::endl;

    QObject::connect(&disp, SIGNAL(paramsUpdated(TControllerParams)),
                     &wind, SLOT(updateParams(TControllerParams)));
    QObject::connect(&disp, SIGNAL(statusUpdated(QHash<ControllerType,QPair<bool,bool> >)),
                     &wind, SLOT(updateStatus(QHash<ControllerType,QPair<bool,bool> >)));

    QObject::connect(&wind, SIGNAL(keyboard(bool)),
                     &disp, SLOT(setKeyboard(bool)));
    QObject::connect(&wind, SIGNAL(keyPressId(int)),
                     &disp, SLOT(slotKey(int)));
    wind.show();
    int returnCode = a.exec();
    return returnCode;
}
