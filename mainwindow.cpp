#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  for (int i = 1; i < 200; i++)
    ui->listWidget->addItem(
        new QListWidgetItem(QString::fromStdString(std::to_string(i))));
  QIcon icon_test(
      "/home/remsto/IN204/projet/archive_test/minimax_alphabeta.png");
  ui->toolButton->setIcon(icon_test);
  ui->listWidget->addItem(new QListWidgetItem(
      QIcon("/home/remsto/IN204/projet/archive_test/minimax_alphabeta.png"),
      QString("test")));
  QObject::connect(
      ui->listWidget,
      SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
      ui->label, SLOT(setFromList(QListWidgetItem *)));
}

MainWindow::~MainWindow() { delete ui; }
