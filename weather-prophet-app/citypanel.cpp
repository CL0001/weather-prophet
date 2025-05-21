#include "citypanel.h"
#include <QHBoxLayout>
#include <QMouseEvent>

CityPanel::CityPanel(const QString &cityName, double temperature, QWidget *parent)
    : QWidget(parent), cityName(cityName)
{
    // Initialization of the city label
    dataLabel = new QLabel(this);
    QString labelText = QString(
                            "<table width='100%' cellspacing='0' cellpadding='0'>"
                            "<tr>"
                            "<td align='left' style='padding-left: 10px;'><b>%1</b></td>"
                            "<td align='right' style='padding-right: 10px;'><b>%2°C</b></td>"
                            "</tr>"
                            "</table>"
                        ).arg(cityName).arg(temperature);

    dataLabel->setText(labelText);
    dataLabel->setFixedHeight(50);
    dataLabel->setStyleSheet(
        "background-color: lightgray;"
        "border: 1px solid gray;"
        "border-radius: 5px;"
        "padding: 5px;"
        "font-size: 16px;"
    );

    dataLabel->installEventFilter(this);

    // Initialization of the remove label
    removeLabel = new QLabel(this);
    removeLabel->setAlignment(Qt::AlignCenter);

    QPixmap closeIcon(":/assets/icons/close.png");
    removeLabel->setPixmap(closeIcon.scaled(removeLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    removeLabel->setStyleSheet(
        "QLabel {"
        "   background-color: red;"
        "border-radius: 5px;"
        "   border: none;"
        "}"
        "QLabel:hover {"
        "   background-color: darkred;"
        "}"
    );

    removeLabel->installEventFilter(this);

    // Initialization of the layout
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(dataLabel, 3);
    layout->addWidget(removeLabel, 1);

    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(10);
    setLayout(layout);
}

QString CityPanel::getCityName()
{
    return cityName;
}

void CityPanel::selectPanel()
{
    dataLabel->setStyleSheet(
        "background-color: lightgray;"
        "border: 5px solid green;"
        "border-radius: 5px;"
        "padding: 5px;"
        "font-size: 16px;"
    );

    removeLabel->setStyleSheet(
        "QLabel {"
        "   background-color: red;"
        "   border: 5px solid green;"
        "   border-radius: 5px;"
        "}"
        "QLabel:hover {"
        "   background-color: darkred;"
        "}"
    );
}

void CityPanel::unselectPanel()
{
    dataLabel->setStyleSheet(
        "background-color: lightgray;"
        "border: 1px solid gray;"
        "border-radius: 5px;"
        "padding: 5px;"
        "font-size: 16px;"
    );

    removeLabel->setStyleSheet(
        "QLabel {"
        "   background-color: red;"
        "border-radius: 5px;"
        "   border: none;"
        "}"
        "QLabel:hover {"
        "   background-color: darkred;"
        "}"
    );
}

bool CityPanel::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == removeLabel && event->type() == QEvent::MouseButtonRelease)
    {
        emit removeRequest(cityName);
        return true;
    }
    if (obj == dataLabel && event->type() == QEvent::MouseButtonRelease)
    {
        emit selectRequest(cityName);
        return true;
    }
    return QWidget::eventFilter(obj, event);
}
