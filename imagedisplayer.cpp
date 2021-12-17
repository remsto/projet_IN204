#include "imagedisplayer.h"
#include <QDebug>
#include <QListWidget>
#include <QSize>
#include <iostream>

void ImageDisplayer::setFromList(QListWidgetItem *item) {
  QPixmap p = (item->icon())
                  .pixmap((item->icon().actualSize(QSize(974 / 2, 1452 / 2))));
  setPixmap(p);
}

void ImageDisplayer::zoomIn() {
  resize(1.25 * pixmap(Qt::ReturnByValue).size());
}
