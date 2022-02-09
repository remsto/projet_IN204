#include "mainwindow.h"
#include "extractor.h"
#include "imagedisplayer.h"

#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <filesystem>
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // ui->horizontalLayout->setAlignment(ui->listWidget, Qt::AlignLeft);
  // ui->horizontalLayout->setAlignment(ui->label, Qt::AlignLeft);

  ui->horizontalLayout->setStretch(0, 1);
  ui->horizontalLayout->setStretch(1, 99);

  QObject::connect(
      ui->listWidget,
      SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
      ui->label, SLOT(setFromList(QListWidgetItem *)));
  QObject::connect(ui->actionZoomer, SIGNAL(triggered(bool)), ui->label,
                   SLOT(zoomIn()));
  QObject::connect(ui->actionDezoomer, SIGNAL(triggered(bool)), ui->label,
                   SLOT(zoomOut()));
  QObject::connect(ui->actionOpen, SIGNAL(triggered(bool)), this,
                   SLOT(openFile()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::openFile() {
  namespace fs = std::filesystem; // C++17

  // Ouvre un dialogue et récupère le chemin du fichier sélectionné
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "",
                                                  tr("Image Files (*.cbz)"));
  std::string extracted_fname =
      fileName.toStdString().erase(fileName.toStdString().rfind('.'));
  /*fs::path temp_path = fs::temp_directory_path();
  temp_path += extracted_fname;

  std::cout << "Current path is " << temp_path << std::endl;*/

  // Conversion de la QString en char *, selon la FAQ de QT
  QByteArray ba = fileName.toLocal8Bit();
  const char *fileNameChar = ba.data();

  std::cout << extracted_fname << std::endl;
  // fs::create_directory(extracted_fname);

  // Extraction de l'archive dans le répertoire courant
  extract(fileNameChar);
  fs::current_path(extracted_fname);
  // Tri des fichiers par ordre alphabétique
  std::vector<std::filesystem::path> files_in_directory;
  std::copy(std::filesystem::directory_iterator(extracted_fname),
            std::filesystem::directory_iterator(),
            std::back_inserter(files_in_directory));
  std::sort(files_in_directory.begin(), files_in_directory.end());

  int i = 0;
  for (const std::string &filename : files_in_directory) {
    if (i == 0) {
      QPixmap first_img(filename.c_str());
      ui->label->frame_size = first_img.size();
      ui->label->scale_factor = 1;
    }
    ui->listWidget->addItem(
        new QListWidgetItem(QIcon(QString(filename.c_str())),
                            QString::fromStdString(std::to_string(i))));
    i++;
  }
}

/*for (const auto &entry : fs::directory_iterator("iliad_homer")) {
  ui->listWidget->addItem(
      new QListWidgetItem(QIcon(entry.path().c_str()), "test"));
}*/
