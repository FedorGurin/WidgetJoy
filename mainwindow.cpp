#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QThread>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->rusButtonsTable->resizeColumnToContents(1);
    ui->rudButtonsTable->resizeColumnToContents(1);
    connect(ui->checkBoxKey,SIGNAL(clicked(bool)),this,SIGNAL(keyboard(bool)));

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateStatus(const QHash<ControllerType, QPair<bool, bool>>) {

}

//
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ui->checkBoxKey->checkState() == Qt::Checked)
    {


    if(ev->key() == Qt::Key_W)
    {
        emit keyPressId(0);
    }else if(ev->key() == Qt::Key_S)
    {
        emit keyPressId(1);
    }else if(ev->key() == Qt::Key_A)
    {
        emit keyPressId(2);
    }else if(ev->key() == Qt::Key_D)
    {
        emit keyPressId(3);
    }else if(ev->key() == Qt::Key_Z)
    {
        emit keyPressId(4);
    }
    else if(ev->key() == Qt::Key_BracketRight)
    {
        emit keyPressId(6);
    }
    else if(ev->key() == Qt::Key_BracketLeft)
    {
        emit keyPressId(5);
    }
    }
    QMainWindow::keyPressEvent(ev);
}
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    emit keyPressId(-1);
    QMainWindow::keyReleaseEvent(ev);
}


void MainWindow::updateParams(const TControllerParams &p) {
//    std::cout << "Main window: " << QThread::currentThreadId() << std::endl;
    // РУС
    TRUSParams rusParams = p.rusParams;
    ui->x_value->setText(QString("%1").arg(rusParams.x));
    ui->y_value->setText(QString("%1").arg(rusParams.y));
    if (rusParams.trigger_strong) ui->rusButtonsTable->item(0, 1)->setText(QString("Зажата сильно"));
    else if (rusParams.trigger) ui->rusButtonsTable->item(0, 1)->setText(QString("Зажата"));
    else ui->rusButtonsTable->item(0, 1)->setText(QString("Отжата"));
    ui->rusButtonsTable->item(1, 1)->setText(twoPosText(rusParams.wpn_rel));
    ui->rusButtonsTable->item(2, 1)->setText(twoPosText(rusParams.nws));
    ui->rusButtonsTable->item(3, 1)->setText(twoPosText(rusParams.bottom));
    ui->rusButtonsTable->item(4, 1)->setText(twoPosText(rusParams.bottom_trigger));
    ui->rusButtonsTable->item(5, 1)->setText(fourPosText(rusParams.trim));
    ui->rusButtonsTable->item(6, 1)->setText(fourPosText(rusParams.bottomleft));
    ui->rusButtonsTable->item(7, 1)->setText(fourPosText(rusParams.bottomright));
    ui->rusButtonsTable->item(8, 1)->setText(fourPosText(rusParams.thumb));
    // РУД
    TRUDParams rudParams = p.rudParams;
    ui->z_value->setText(QString("%1").arg(rudParams.z));
    ui->ant_elev_v->setText(QString("%1").arg(rudParams.ant_elev));
    ui->man_rng_v->setText(QString("%1").arg(rudParams.man_rng));
    ui->rudButtonsTable->item(0, 1)->setText(twoPosText(rudParams.enable));
    ui->rudButtonsTable->item(1, 1)->setText(twoPosText(rudParams.uncage));
    ui->rudButtonsTable->item(2, 1)->setText(threePosText(rudParams.dog_fight));
    ui->rudButtonsTable->item(3, 1)->setText(threePosText(rudParams.spd_brk));
    ui->rudButtonsTable->item(4, 1)->setText(fourPosText(rudParams.vuhf_iff));
}

QString MainWindow::twoPosText(int p) {
    return p ? QString("Зажата") : QString("Отжата");
}

QString MainWindow::threePosText(TThreePosButton &b) {
    QString text;
    switch(b.pos) {
    case UP:
        text = "UP";
        break;
    case DOWN:
        text = "DOWN";
        break;
    case CENTER:
        text = "CENTER";
        break;
    default:
        text = "N/A";
    }
    return text;
}

QString MainWindow::fourPosText(TFourPosButton &b) {
    QString text;
    switch (b.hpos) {
    case UP:
       text = "LEFT / ";
       break;
    case DOWN:
        text = "RIGHT / ";
        break;
    case CENTER:
        text = "CENTER / ";
        break;
    default:
        text = "N/A / ";
    }
    switch (b.vpos) {
    case UP:
        text += "UP";
        break;
    case DOWN:
        text += "DOWN";
        break;
    case CENTER:
        text += "CENTER";
        break;
    default:
        text += "N/A";
    }
    return text;
}

