#include "mainwindow.h"

#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    splitter = new QSplitter(this);
    splitter->setStyleSheet("background: rgb(69, 69, 69);");

    leftPanel = new LeftPanel(splitter);
    rightPanel = new RightPanel(splitter);

    connect(leftPanel, &LeftPanel::citySelected, rightPanel, &RightPanel::loadSelectedCityData);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    setCentralWidget(splitter);

    resize(2000, 900);
    setMinimumSize(1600, 900);
}

