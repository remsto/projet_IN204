#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);
  QIcon icon_test(
      "/home/remsto/IN204/projet/archive_test/minimax_alphabeta.png");
  for (int i = 1; i < 10; i++)
    ui->listWidget->addItem(new QListWidgetItem(
        icon_test, QString::fromStdString(std::to_string(i))));
  ui->listWidget->addItem(new QListWidgetItem(icon_test, "yahey"));
  QObject::connect(
      ui->listWidget,
      SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
      ui->label, SLOT(setFromList(QListWidgetItem *)));
}

MainWindow::~MainWindow() { delete ui; }
