#ifndef LEFTPANEL_H
#define LEFTPANEL_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QNetworkAccessManager>
#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>

class LeftPanel : public QWidget
{
    Q_OBJECT
public:
    explicit LeftPanel(QWidget *parent = nullptr);
    ~LeftPanel();

signals:
    void citySelected(const QMap<QString, QVariant>);

public slots:
    void onSearchTriggered();
    void removeCityPanel(const QString& city);
    void selectCityPanel(const QString& city);

private:
    QMap<QString, QMap<QString, QVariant>> cityWeatherData;
    QNetworkAccessManager networkManager;
    QLineEdit* searchBar;
    QScrollArea* cityPanelList;
    QVBoxLayout* scrollLayout;

    void fetchCityWeatherData(const QString city);
    void fetchAllCityWeatherData();
    void addCityPanel(const QString& city, const QVariant &temperature);
    void saveCityList();
    void loadCityList();
};

#endif // LEFTPANEL_H
