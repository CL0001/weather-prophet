#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMap>
#include <QVariant>
#include <QFrame>
#include <QPixmap>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

class RightPanel : public QWidget
{
    Q_OBJECT
public:
    explicit RightPanel(QWidget *parent = nullptr);
    void displayDefaultView();
    void displayCityView();

public slots:
    void loadSelectedCityData(const QMap<QString, QVariant> &selectedCityData);

private:
    void clearLayout();
    void addTableSection(QTableWidget* table, int row, const QVector<QString>& iconPaths, const QVector<QString>& texts);

    QVBoxLayout* mainLayout;
    QMap<QString, QVariant> currentCityData;
};

#endif // RIGHTPANEL_H
