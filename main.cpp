#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
  QApplication::setStyle(QStyleFactory::create("Windows"));
  QApplication app(argc, argv);

  MainWindow mainWindow;

  mainWindow.show();
  return app.exec();
}
