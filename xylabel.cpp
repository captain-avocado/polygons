#include "xylabel.h"

xyLabel::xyLabel( QWidget * parent ):QLabel(parent) {}

void xyLabel::mousePressEvent ( QMouseEvent * event )
{
    emit xyLabel_clicked(this, event);
}
