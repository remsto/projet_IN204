#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <filesystem>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void openFile();
  void closeFile();
  void loadImage(int index);

private:
  Ui::MainWindow *ui;
  std::vector<std::filesystem::path> *image_directory;
  std::vector<bool> *image_loaded;
  int image_max;
};
#endif // MAINWINDOW_H
