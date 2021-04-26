#include "QtFirebase.h"

#include <firebase/auth.h>
#include <firebase/auth/user.h>
#include <firebase/firestore.h>

#include <qserialport.h>

#include <qfile.h>


QtFirebase::QtFirebase(QWidget *parent)
    : QMainWindow(parent),
      m_serialPort(new QSerialPort(this))
{
  ui.setupUi(this);


  using namespace firebase; 

  QFile file(":/QtFirebase/google-services.json");
  
  
  if (file.open(QIODevice::ReadOnly)) {

    auto data = file.readAll();

    //ui.label->setText(data);

    AppOptions Options;

    AppOptions::LoadFromJsonConfig(data.toStdString().c_str(), &Options);

    m_firebaseApp = std::unique_ptr<App>(App::Create(Options));

    if (m_firebaseApp == nullptr) {
      ui.label->setText("Error while initializing firebase app object");
    }
    
    using namespace firebase::auth;
      
    InitResult result;

    //Hardcoded login
    auto auth = std::unique_ptr<Auth>(Auth::GetAuth(m_firebaseApp.get(), &result));
    
    if (result == kInitResultSuccess) {
      
      auto res = auth->SignInWithEmailAndPassword("developer.4m2q@gmail.com", "#$Bl45.2!ert");


      while (res.status() == FutureStatus::kFutureStatusPending) {}

      if (res.error() == kAuthErrorNone) {
        if (res.result() && *res.result()) {
          auto user = *res.result();
          
        }
        else {
          ui.label->setText("Invalid result");
        }
      }
      else {
        ui.label->setText(QString("Error while trying to login") + res.error_message());
      }
    }
    else {
      ui.label->setText("Failed to get Auth module");
    }
  }
  else {
    ui.label->setText("Failed to open file");
  }

  connect(ui.sendButton, &QPushButton::clicked, [this]() {
    ui.consoleLabel->setText("Botton down");
    
    QByteArray data = ui.serialData->toPlainText().toLocal8Bit();

    m_serialPort->write(data);
  });

  connect(m_serialPort, &QSerialPort::errorOccurred, [this](QSerialPort::SerialPortError err) {
    
    if (QSerialPort::NoError == err) {
      return;
    }

    ui.consoleLabel->setText("Error detected: " + QString("%1").arg(err));
    ui.reconnect->setVisible(true);

    if (QSerialPort::ResourceError == err) {
      m_serialPort->close();
    }
  });

  connect(ui.reconnect, &QPushButton::clicked, this, &QtFirebase::connectToSerialPort);

  m_serialPort->setPortName("COM3");
  m_serialPort->setBaudRate(9600);
  m_serialPort->setDataBits(QSerialPort::Data8);
  m_serialPort->setParity(QSerialPort::NoParity);
  m_serialPort->setStopBits(QSerialPort::OneStop);
  m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

  ui.reconnect->setVisible(false);

  connectToSerialPort();
}

void QtFirebase::connectToSerialPort()
{
  if (m_serialPort->open(QIODevice::ReadWrite)) {
    ui.consoleLabel->setText("Serial port opened");
    ui.reconnect->setVisible(false);
  }
  else {
    ui.consoleLabel->setText("Failed to open serial port");
    ui.reconnect->setVisible(true);
  }
}
