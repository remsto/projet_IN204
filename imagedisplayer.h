#ifndef IMAGEDISPLAYER_H
#define IMAGEDISPLAYER_H

#include <QLabel>
#include <QListWidget>

class ImageDisplayer : public QLabel {
  Q_OBJECT
public:
  using QLabel::QLabel;
public slots:
  void setFromList(QListWidgetItem *item);
};

#endif // IMAGEDISPLAYER_H
