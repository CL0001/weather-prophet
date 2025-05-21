#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>

#include "leftpanel.h"
#include "rightpanel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    QSplitter* splitter;
    LeftPanel* leftPanel;
    RightPanel* rightPanel;
};

#endif // MAINWINDOW_H
