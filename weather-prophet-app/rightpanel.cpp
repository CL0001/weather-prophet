#include "rightpanel.h"

RightPanel::RightPanel(QWidget *parent)
    : QWidget(parent), mainLayout(new QVBoxLayout(this))
{
    setStyleSheet(
        "QWidget {"
        "   background: rgb(34, 34, 34);"
        "   border: 2px solid white;"
        "   border-radius: 10px;"
        "}"
    );

    setLayout(mainLayout);
    displayDefaultView();
}

void RightPanel::clearLayout()
{
    while (QLayoutItem* item = mainLayout->takeAt(0))
    {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void RightPanel::displayDefaultView()
{
    clearLayout();

    QLabel* titleLabel = new QLabel("Weather Prophet", this);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 24px;"
        "   color: white;"
        "   qproperty-alignment: AlignCenter;"
        "}"
    );
    titleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);
}

void RightPanel::displayCityView()
{
    clearLayout();

    QTableWidget* table = new QTableWidget(this);

    table->setShowGrid(false);

    table->setColumnCount(8);
    table->setRowCount(9);

    table->horizontalHeader()->setVisible(false);
    table->verticalHeader()->setVisible(false);

    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Header
    QLabel* cityName = new QLabel(currentCityData["name"].toString(), this);
    cityName->setStyleSheet("font-size: 24px; font-weight: bold; color: white; border: none;");
    cityName->setAlignment(Qt::AlignCenter);
    table->setRowHeight(0, 60);

    QTableWidgetItem* headerItem = new QTableWidgetItem();
    table->setSpan(0, 0, 1, 8);
    table->setCellWidget(0, 0, cityName);

    // Location & Description
    addTableSection(table, 1, {":/assets/icons/earth.png", ":/assets/icons/sunrise.png", ":/assets/icons/sunset.png", ":/assets/icons/info.png"},
                    {
                        QString("%1°N, %2°W").arg(currentCityData["lon"].toString(), currentCityData["lat"].toString()),
                        currentCityData["sunrise"].toString(),
                        currentCityData["sunset"].toString(),
                        currentCityData["weather_description"].toString()
                    });

    // Main Information
    addTableSection(table, 2, {":/assets/icons/temperature.png", ":/assets/icons/temperature.png", ":/assets/icons/temperature.png", ":/assets/icons/temperature.png"},
                    {
                        QString("Temperature: %1°C").arg(currentCityData["temp"].toString()),
                        QString("Feels Like: %1°C").arg(currentCityData["feels_like"].toString()),
                        QString("Min Temp: %1°C").arg(currentCityData["temp_min"].toString()),
                        QString("Max Temp: %1°C").arg(currentCityData["temp_max"].toString())
                    });

    // Additional Information
    addTableSection(table, 3, {":/assets/icons/gauge.png", ":/assets/icons/drop.png", ":/assets/icons/wind.png", ":/assets/icons/height.png"},
                    {
                        QString("Pressure: %1 hPa").arg(currentCityData["pressure"].toString()),
                        QString("Humidity: %1 %").arg(currentCityData["humidity"].toString()),
                        QString("Wind Speed: %1 m/s").arg(currentCityData["wind_speed"].toString()),
                        QString("Sea Level: %1 m").arg(currentCityData["sea_level"].toString())
                    });

    // Predictions for five days ahead
    addTableSection(table, 4, {" ", ":/assets/icons/circle-1.png", " ", " "},
                    {
                        QString(" "),
                        QString("%1").arg(currentCityData["day1"].toString()),
                        QString("%1").arg(currentCityData["value1"].toString()),
                        QString(" ")
                    });

    addTableSection(table, 5, {" ", ":/assets/icons/circle-2.png", " ", " "},
                    {
                        QString(" "),
                        QString("%1").arg(currentCityData["day2"].toString()),
                        QString("%1").arg(currentCityData["value2"].toString()),
                        QString(" ")
                    });

    addTableSection(table, 6, {" ", ":/assets/icons/circle-3.png", " ", " "},
                    {
                        QString(" "),
                        QString("%1").arg(currentCityData["day3"].toString()),
                        QString("%1").arg(currentCityData["value3"].toString()),
                        QString(" ")
                    });

    addTableSection(table, 7, {" ", ":/assets/icons/circle-4.png", " ", " "},
                    {
                        QString(" "),
                        QString("%1").arg(currentCityData["day4"].toString()),
                        QString("%1").arg(currentCityData["value4"].toString()),
                        QString(" ")
                    });

    addTableSection(table, 8, {" ", ":/assets/icons/circle-5.png", " ", " "},
                    {
                        QString(" "),
                        QString("%1").arg(currentCityData["day5"].toString()),
                        QString("%1").arg(currentCityData["value5"].toString()),
                        QString(" ")
                    });

    mainLayout->addWidget(table);
}

void RightPanel::addTableSection(QTableWidget* table, int row, const QVector<QString>& iconPaths, const QVector<QString>& texts)
{
    table->setRowHeight(row, 60);

    for (int i = 0; i < 4; ++i)
    {
        QLabel* iconLabel = new QLabel(table);
        if (!iconPaths[i].isEmpty())
        {
            QPixmap iconPixmap(iconPaths[i]);
            QPixmap scaledIcon = iconPixmap.scaled(QSize(48, 48), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            iconLabel->setPixmap(scaledIcon);
            iconLabel->setStyleSheet("border: none;");
        }
        iconLabel->setAlignment(Qt::AlignCenter);
        table->setCellWidget(row, i * 2, iconLabel);

        QLabel* textLabel = new QLabel(texts[i], table);
        textLabel->setStyleSheet("color: white; font-weight: bold; font-size: 14px; border: none;");
        textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        table->setCellWidget(row, i * 2 + 1, textLabel);
    }
}

void RightPanel::loadSelectedCityData(const QMap<QString, QVariant> &selectedCityData)
{
    currentCityData = selectedCityData;
    qDebug() << "Swapping to city data view";
    displayCityView();
}
