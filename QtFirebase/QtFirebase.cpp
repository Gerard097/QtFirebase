#include "QtFirebase.h"

#include <firebase/auth.h>
#include <firebase/auth/user.h>

#include <qfile.h>


QtFirebase::QtFirebase(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);


  using namespace firebase; 

  QFile file(":/QtFirebase/google-services.json");
  
  
  if (file.open(QIODevice::ReadOnly)) {

    auto data = file.readAll();

    ui.label->setText(data);


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
        if (res.result()) {
          auto user = *res.result();
          ui.label->setText(QString::fromStdString(user->uid()));
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
  
}
