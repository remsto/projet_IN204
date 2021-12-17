#ifndef IMAGEDISPLAYER_H
#define IMAGEDISPLAYER_H

#include <QLabel>
#include <QListWidget>

class ImageDisplayer : public QLabel {
  Q_OBJECT
public:
  using QLabel::QLabel;
  QSize frame_size;
  double scale_factor;

public slots:
  void setFromList(QListWidgetItem *item);
  void zoomIn();
  void zoomOut();
};

#endif // IMAGEDISPLAYER_H
