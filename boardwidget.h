#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <QListWidget>
#include <QString>
#include <QLabel>
#include <QApplication>
#include <QFrame>

#include "Board.h"
#include "boardpositionwidget.h"

class BoardWidget : public QWidget, public Board
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);
    void togglePhase2At(){this->phase2At = 3 - phase2At;};
private:
    BoardPositionWidget positionsW[7][7];
    QListWidget *statusList;
    QListWidget *rules;
    void updateStatusList(QString inc);
    //interim variable for the "phase 2 states": 1 is selection, 2 is moving.
    int phase2At = 1;

    //style sheets
    void hoverStyle();
    void selectionStyle();
    void removeStyle();

    //variables for phase 2 GUI
    char rowSelected;
    int columnSelected;

    char rowGoal;
    int  columnGoal;

    //disable buttons on victory
    void gameOverButtonDisable();

signals:

public slots:
    void posClicked();

};

#endif // BOARDWIDGET_H
