#pragma once

#include <memory>

#include <QtWidgets/QMainWindow>
#include "ui_QtFirebase.h"
#include <firebase/app.h>


class QtFirebase : public QMainWindow
{
    Q_OBJECT

public:
    QtFirebase(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtFirebaseClass ui;
    std::unique_ptr<firebase::App> m_firebaseApp;
};
