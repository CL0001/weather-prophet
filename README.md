# Weather Prophet – Weather Forecasts and Information Around the World


![Image](https://github.com/user-attachments/assets/65f392e4-1e3f-421c-b53f-9a9adb56ca1c)


## About
Weather Prophet is a weather forecasting application developed as my vocational graduation thesis. This project allowed me to gain hands-on experience with C++ and the Qt framework, as I am passionate about C++ development. Qt provides seamless integration through Qt Creator and offers built-in networking support, making it an excellent choice for rapid application development.

The application consists of a C++ frontend built with Qt and a Python backend using FastAPI. The backend collects and stores weather data to generate five-day forecasts. Users can search for any city worldwide with an autofill suggestion feature, and the application displays detailed weather and location data.


## Tech Stack
- C++ with Qt framework for the application
- Python with FastAPI, PyTorch, and Docker for the REST API
- SQLite for data storage


## Integration Process
The application is built using QtWidgets, as I found it more intuitive compared to QtQuick. This choice allowed me to develop the UI in a pure C++ approach rather than learning QML. As a result, the application's interface is simple but functional.

<<<<<<< HEAD
To optimize performance and minimize the executable size, I used Qt Resource Compiler (qrc) to embed a list of cities for autofill directly into the application. Instead of storing weather data locally, the application fetches data from the REST API. When a user selects a city, a request is sent to the API, which retrieves the latest weather information, processes it through a prediction model, and returns the forecast for display.

To avoid redundant calculations and reduce server load, forecast data for each city and date is cached in the database once retrieved, ensuring faster responses for future queries.


## Installation
The latest executable build of Weather Prophet is available for download in the Releases section.


## Project Setup
1. Clone the Repository
Clone the repository using Git or download it as a ZIP file:
```bash
git clone https://github.com/CL0001/weather-prophet.git
```
2. Run the REST API with Docker
Navigate to the WeatherProphet.api directory and build the Docker container:
=======
To optimize performance and minimize the executable size, I used Qt Resource Compiler (qrc) to embed a list of cities for autofill directly into the application. Instead of storing weather data locally, the application fetches data from the REST API. When a user selects a city, a request is sent to the API, which retrieves the latest weather information, processes it through a prediction model, and returns the forecast with rest of the data for display.

To avoid redundant calculations and reduce server load, forecast data for each city and date is cached in the database once retrieved, ensuring faster responses for future queries. Additionally, the data is stored to further improve the model's accuracy over time.


## Installation
The latest executable build of Weather Prophet is available for download in the [Releases section](https://github.com/CL0001/weather-prophet/releases).


## Project Setup
1. Clone the Repository using Git or download it as a ZIP file:
```bash
git clone https://github.com/CL0001/weather-prophet.git
```
2. Run the REST API with Docker, navigate to the `WeatherProphet.api` directory and build the Docker container:
>>>>>>> ffbd62125087d6a0b582cff57f0493da1e1d70c0
```bash
docker build -t weather-prophet-api .
docker run -d --name weather-prophet-api -p 8080:80 weather-prophet-api
```
3. Open and Build the Application in QtCreator
    1. Open QtCreator.
<<<<<<< HEAD
    2. Click "Open Project" and select CMakeLists.txt from the WeatherProphet.app directory.
    3. Configure the project as needed, then build and run it.
=======
    2. Click **"Open Project"** and select `CMakeLists.txt` from the `WeatherProphet.app` directory.
    3. Configure the project as needed, then build and run it.
>>>>>>> ffbd62125087d6a0b582cff57f0493da1e1d70c0
