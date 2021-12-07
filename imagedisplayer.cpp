#include "imagedisplayer.h"
#include <QDebug>
#include <QListWidget>
#include <iostream>

void ImageDisplayer::setFromList(QListWidgetItem *item) {
  setPixmap((item->icon()).pixmap(500, 500));
}
