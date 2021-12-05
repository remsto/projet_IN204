#include "imagedisplayer.h"
#include <QDebug>
#include <QListWidget>
#include <iostream>

void ImageDisplayer::setFromList(QListWidgetItem *item) {
  setText(item->text());
}
