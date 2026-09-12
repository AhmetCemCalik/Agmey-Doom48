#pragma once

#include <QStackedWidget>
#include <QMainWindow>
#include "gameboard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;
    GameBoard *gameBoard;
};
