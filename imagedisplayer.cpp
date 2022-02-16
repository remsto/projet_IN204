#include "imagedisplayer.h"
#include <QDebug>
#include <QListWidget>
#include <QSize>
#include <iostream>

void ImageDisplayer::setFromList(QListWidgetItem *item) {
  if (item) {
    QPixmap p = (item->icon()).pixmap(scale_factor * frame_size);
    setPixmap(p);
  }
}

void ImageDisplayer::zoomIn() {
  if (scale_factor < 1.5) {
    scale_factor *= 1.25;
    resize(scale_factor * frame_size);
  }
}

void ImageDisplayer::zoomOut() {
  if (scale_factor > 0.1) {
    scale_factor *= 0.75;
    resize(scale_factor * frame_size);
  }
}
