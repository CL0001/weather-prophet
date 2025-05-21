#include "leftpanel.h"
#include "citypanel.h"

#include <QAction>
#include <QIcon>
#include <QPixmap>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QSizePolicy>
#include <QCompleter>
#include <QMessageBox>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

const QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/saved_cities.txt";

LeftPanel::LeftPanel(QWidget *parent)
    : QWidget{parent}
{
    // City suggestion
    QStringList citySuggestions;
    QFile file(":/assets/files/city_names.txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream tstream(&file);
        while (!tstream.atEnd()) {
            QString cityName = tstream.readLine();
            citySuggestions.append(cityName);
        }
        file.close();
    } else {
        qWarning() << "Could not open city list file.";
    }

    QCompleter* completer = new QCompleter(citySuggestions, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setFilterMode(Qt::MatchContains);

    // Search bar initialization
    searchBar = new QLineEdit(this);

    searchBar->setCompleter(completer);

    QAction* searchAction = new QAction(QIcon(QPixmap(":/assets/icons/magnifying-glass.png")), "search", this);
    searchBar->addAction(searchAction, QLineEdit::TrailingPosition);

    searchBar->setStyleSheet(
        "QLineEdit {"
        "   background-color: rgb(34, 34, 34);"
        "   border: 2px solid white;"
        "   border-radius: 10px;"
        "   color: white;"
        "   padding: 10px;"
        "}"
    );

    connect(searchBar, &QLineEdit::returnPressed, this, [this, completer]() {
        if (completer->completionCount() > 0) {
            completer->setCurrentRow(0);
            searchBar->setText(completer->currentCompletion());
            onSearchTriggered();
        }
        else
        {
            QMessageBox::warning(this, "Invalid Input", "Please select a valid city from the suggestions.");
            searchBar->clear();
        }

    });

    // Scroll area initialization
    cityPanelList = new QScrollArea(this);

    auto scrollContainer = new QWidget(this);
    scrollLayout = new QVBoxLayout(scrollContainer);
    scrollContainer->setLayout(scrollLayout);

    scrollContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    cityPanelList->setWidget(scrollContainer);
    cityPanelList->setWidgetResizable(true);
    cityPanelList->setStyleSheet(
        "QScrollArea {"
        "   background: rgb(34, 34, 34);"
        "   border: 2px solid white;"
        "   border-radius: 10px;"
        "}"
    );

    scrollContainer->setStyleSheet("background: transparent;");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(searchBar);
    layout->addWidget(cityPanelList);
    setLayout(layout);

    // Loading previous city data
    loadCityList();

    // Linking signals and slots
    connect(searchAction, &QAction::triggered, this, &LeftPanel::onSearchTriggered);
}

LeftPanel::~LeftPanel()
{
    saveCityList();
}

void LeftPanel::onSearchTriggered()
{
    QString city = searchBar->text().trimmed();

    if (!city.isEmpty() && !cityWeatherData.contains(city))
    {
        fetchCityWeatherData(city);
    }
    else
    {
        qWarning() << "City already added or invalid.";
    }

    searchBar->clear();
}

void LeftPanel::removeCityPanel(const QString& city)
{
    for (int i = 0; i < scrollLayout->count(); i++)
    {
        QWidget* widget = scrollLayout->itemAt(i)->widget();
        CityPanel* cityPanel = qobject_cast<CityPanel*>(widget);

        if (cityPanel && cityPanel->getCityName() == city)
        {
            scrollLayout->removeWidget(cityPanel);
            cityPanel->deleteLater();
            cityWeatherData.remove(city);
            break;
        }
    }
}

void LeftPanel::selectCityPanel(const QString &city)
{
    for (int i = 0; i < scrollLayout->count(); i++)
    {
        QWidget* widget = scrollLayout->itemAt(i)->widget();
        CityPanel* cityPanel = qobject_cast<CityPanel*>(widget);

        if (cityPanel && cityPanel->getCityName() == city)
        {
            cityPanel->selectPanel();

            for (int j = 0; j < scrollLayout->count(); j++)
            {
                QWidget* otherWidget = scrollLayout->itemAt(j)->widget();
                CityPanel* otherCityPanel = qobject_cast<CityPanel*>(otherWidget);

                if (otherCityPanel && otherCityPanel != cityPanel)
                {
                    otherCityPanel->unselectPanel();
                }
            }
            emit citySelected(cityWeatherData[city]);
            break;
        }
    }
}

void LeftPanel::fetchCityWeatherData(const QString city)
{
    QUrl apiUrl(QString("http://localhost:8000/city/%1").arg(city));
    QNetworkRequest request(apiUrl);

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, [=](){
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

            if (jsonDoc.isObject())
            {
                QJsonObject jsonObj = jsonDoc.object();
                QMap<QString, QVariant> cityWeather;

                for (const QString& key : jsonObj.keys())
                {
                    cityWeather[key] = jsonObj.value(key).toVariant();
                }

                cityWeatherData[city] = cityWeather;

                QVariant temperature = cityWeather["temp"];
                addCityPanel(city, temperature);

                qDebug() << "Weather data for city" << city << "is" << temperature.toDouble() << "C";
            }
            else
            {
                qWarning() << "Invalid JSON response for city:" << city;
            }
        }
        else
        {
            qWarning() << "HTTP request error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void LeftPanel::fetchAllCityWeatherData()
{
    for (const QString& city : cityWeatherData.keys())
    {
        fetchCityWeatherData(city);
    }
}

void LeftPanel::addCityPanel(const QString &city, const QVariant &temperature)
{
    CityPanel* cityPanel = new CityPanel(city, temperature.toDouble(), this);

    connect(cityPanel, &CityPanel::removeRequest, this, &LeftPanel::removeCityPanel);
    connect(cityPanel, &CityPanel::selectRequest, this, &LeftPanel::selectCityPanel);

    scrollLayout->addWidget(cityPanel);
    scrollLayout->setAlignment(Qt::AlignTop);
}

void LeftPanel::saveCityList()
{
    QDir dir = QFileInfo(filePath).absoluteDir();
    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Could not open file for writing:" << file.errorString();
        return;
    }

    QTextStream stream(&file);

    for (const QString &city : cityWeatherData.keys())
    {
        stream << city << "\n";
    }

    file.close();
    qDebug() << "City list successfully saved to" << filePath;
}

void LeftPanel::loadCityList()
{
    QFile file(filePath);
    if (!file.exists())
    {
        qWarning() << "City list file does not exist, skipping load.";
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Could not open city list file:" << file.errorString();
        return;
    }

    QTextStream tstream(&file);
    while (!tstream.atEnd())
    {
        QString cityName = tstream.readLine().trimmed();
        if (!cityName.isEmpty())
        {
            fetchCityWeatherData(cityName);
        }
    }
    file.close();
    qDebug() << "City list successfully loaded from" << filePath;
}
