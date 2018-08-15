#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unordered_map>
#include <QMainWindow>
#include "controller_types.h"
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void keyboard(bool);
    void keyPressId(int);
protected :
    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);

public slots:
    void updateStatus(const QHash<ControllerType, QPair<bool, bool>>);
    void updateParams(const TControllerParams&);
private:
    Ui::MainWindow *ui;

    QString twoPosText(int);
    QString threePosText(TThreePosButton&);
    QString fourPosText(TFourPosButton&);
};


#endif // MAINWINDOW_H
