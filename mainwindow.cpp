#include "mainwindow.h"
#include "extractor.h"
#include "imagedisplayer.h"

#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);
  ui->horizontalLayout->setStretch(0, 1);
  ui->horizontalLayout->setStretch(1, 99);

  image_max = 10;
  image_directory = new std::vector<std::filesystem::path>;
  image_directory->push_back(std::filesystem::path());

  // Connection des Signaux et des slots des divers objets
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
  QObject::connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this,
                   SLOT(loadImage(int)));
}

MainWindow::~MainWindow() {
  closeFile();
  delete ui;
}

void MainWindow::openFile() {
  closeFile();
  namespace fs = std::filesystem; // C++17

  // Ouvre un dialogue et récupère le chemin du fichier sélectionné
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "",
                                                  tr("Image Files (*.cbz)"));

  if (fileName != NULL) {
    try {
      fs::path extracted_fname = fileName.toStdString();
      extracted_fname = extracted_fname.stem();

      // Conversion de la QString en char *, selon la FAQ de QT
      QByteArray ba = fileName.toLocal8Bit();
      const char *fileNameChar = ba.data();

      // Extraction de l'archive dans le répertoire courant
      extract(fileNameChar);

      // Tri des fichiers par ordre alphabétique
      image_directory = new std::vector<std::filesystem::path>;

      std::copy(std::filesystem::directory_iterator(extracted_fname),
                std::filesystem::directory_iterator(),
                std::back_inserter(*(image_directory)));
      std::sort(image_directory->begin(), image_directory->end());
      image_loaded = new std::vector<bool>(image_directory->size(), false);
    } catch (std::exception) {
      QMessageBox msgBox;
      msgBox.setText("Le fichier n'est pas au bon format !");
      msgBox.exec();
    }

    // Met toutes les images en blanc à l'initialisation
    QPixmap *white_pixmap;
    for (int i = 0; i < image_directory->size(); i++) {
      if (i == 0) {
        QPixmap first_img(image_directory->at(0).c_str());
        ui->label->frame_size = first_img.size();
        ui->label->scale_factor = 1;
        white_pixmap = new QPixmap(first_img.size());
        white_pixmap->fill();
      }
      ui->listWidget->addItem(new QListWidgetItem(
          QIcon(*white_pixmap), QString::fromStdString(std::to_string(i))));
    }
    loadImage(0);
  }
}

void MainWindow::closeFile() {
  std::filesystem::path dir_path = image_directory->at(0).parent_path();
  if (!dir_path.empty()) {
    std::filesystem::remove_all(dir_path);
    ui->listWidget->clear();
  }
}

void MainWindow::loadImage(int index) {
  int begin = 0;
  int end = image_directory->size();
  if (index + image_max > end)
    begin = std::max(begin, end - image_max - (index + image_max - end));
  else if (index - image_max < begin)
    end = std::min(end, image_max * 2);
  else {
    begin = std::max(begin, index - image_max);
    end = std::min(end, index + image_max);
  }
  for (int i = begin; i < end; i++) {
    if (!image_loaded->at(i)) {
      ui->listWidget->item(i)->setIcon(
          QIcon(QString(image_directory->at(i).c_str())));
      if (i == index)
        emit ui->listWidget->currentItemChanged(ui->listWidget->item(index),
                                                nullptr);
      image_loaded->at(i) = true;
    }
  }
}
