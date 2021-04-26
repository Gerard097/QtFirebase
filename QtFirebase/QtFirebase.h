#pragma once

#include <memory>

#include <QtWidgets/QMainWindow>
#include "ui_QtFirebase.h"
#include <firebase/app.h>
#include <qserialport.h>
#include <qserialportinfo.h>

class QtFirebase : public QMainWindow
{
    Q_OBJECT

public:
    QtFirebase(QWidget *parent = Q_NULLPTR);
    //~QtFirebase();
 public slots:
    void connectToSerialPort();
private:
    Ui::QtFirebaseClass ui;
    std::unique_ptr<firebase::App> m_firebaseApp;
    QSerialPort* m_serialPort;
};
