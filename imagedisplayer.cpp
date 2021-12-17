#include "imagedisplayer.h"
#include <QDebug>
#include <QListWidget>
#include <QSize>
#include <iostream>

void ImageDisplayer::setFromList(QListWidgetItem *item) {
  QPixmap p = (item->icon()).pixmap(50 * frame_size);
  setPixmap(p);
}

void ImageDisplayer::zoomIn() {
  scale_factor *= 1.25;
  resize(scale_factor * frame_size);
}

void ImageDisplayer::zoomOut() {
  scale_factor *= 0.75;
  resize(scale_factor * frame_size);
}
