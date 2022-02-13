#include "mainwindow.h"
#include "extractor.h"
#include "imagedisplayer.h"

#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <chrono>
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

  image_max = 10;
  image_directory = new std::vector<std::filesystem::path>;
  image_directory->push_back(std::filesystem::path());

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

// void MainWindow::loadImage() {}

void MainWindow::openFile() {
  closeFile();
  namespace fs = std::filesystem; // C++17

  // Ouvre un dialogue et récupère le chemin du fichier sélectionné
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "",
                                                  tr("Image Files (*.cbz)"));

  std::cout << fileName.toStdString() << std::endl;

  if (fileName != NULL) {

    // std::string extracted_fname =
    // fileName.toStdString().erase(fileName.toStdString().rfind('.'));

    fs::path extracted_fname = fileName.toStdString();
    extracted_fname = extracted_fname.stem();

    /*fs::path temp_path = fs::temp_directory_path();
    temp_path += extracted_fname;

    std::cout << "Current path is " << temp_path << std::endl;*/

    // Conversion de la QString en char *, selon la FAQ de QT
    QByteArray ba = fileName.toLocal8Bit();
    const char *fileNameChar = ba.data();

    std::cout << extracted_fname << std::endl;

    // Extraction de l'archive dans le répertoire courant
    auto start_extr = std::chrono::high_resolution_clock::now();
    extract(fileNameChar);
    auto duration = std::chrono::high_resolution_clock::now() - start_extr;
    std::cout << "extraction time : " << duration.count() << std::endl;

    // fs::current_path(extracted_fname);

    // Tri des fichiers par ordre alphabétique
    image_directory = new std::vector<std::filesystem::path>;
    std::copy(std::filesystem::directory_iterator(extracted_fname),
              std::filesystem::directory_iterator(),
              std::back_inserter(*(image_directory)));
    std::sort(image_directory->begin(), image_directory->end());
    image_loaded = new std::vector<bool>(image_directory->size(), false);
    loadImage(0);

    //    int i = 0;
    //    for (const std::string &filename : files_in_directory) {
    //      if (i == 0) {
    //        QPixmap first_img(filename.c_str());
    //        ui->label->frame_size = first_img.size();
    //        ui->label->scale_factor = 1;
    //      }
    //      ui->listWidget->addItem(
    //          new QListWidgetItem(QIcon(QString(filename.c_str())),
    //                              QString::fromStdString(std::to_string(i))));
    //      i++;
    //    }
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
      if (i == 0) {
        QPixmap first_img(image_directory->at(i).c_str());
        ui->label->frame_size = first_img.size();
        ui->label->scale_factor = 1;
      }
      ui->listWidget->addItem(
          new QListWidgetItem(QIcon(QString(image_directory->at(i).c_str())),
                              QString::fromStdString(std::to_string(i))));
      image_loaded->at(i) = true;
    }
  }
}
/*for (const auto &entry : fs::directory_iterator("iliad_homer")) {
  ui->listWidget->addItem(
      new QListWidgetItem(QIcon(entry.path().c_str()), "test"));
}*/
