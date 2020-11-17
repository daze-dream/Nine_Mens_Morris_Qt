#ifndef BOARDPOSITIONWIDGET_H
#define BOARDPOSITIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QSignalMapper>

#include "BoardPosition.h"

class BoardPositionWidget: public QPushButton, public BoardPosition
{
    Q_OBJECT
public:
    BoardPositionWidget(QPushButton *parent = nullptr);

};

#endif // BOARDPOSITIONWIDGET_H
