# Weather Prophet – Weather Forecasts and Information Around the World


![image](https://github.com/user-attachments/assets/bbb49a95-5f64-41cd-b483-d58823e525ad)


## About
Weather Prophet is a weather forecasting application developed as my vocational graduation thesis. This project allowed me to gain hands-on experience with C++ and the Qt framework, as I am passionate about C++ development.
Qt provides seamless integration through Qt Creator and offers built-in networking support, making it an excellent choice for rapid application development.

The application consists of a C++ frontend built with QtWidgets and a Python backend using FastAPI framework. The backend collects and stores weather data to generate five-day forecasts.
Users can search for any city worldwide with an autofill suggestion feature, and the application displays detailed weather and location data.


## Tech Stack
**Application:** C++ with Qt framework

**REST API:** Python with FastAPI, PyTorch, and Docker

**Database:** SQLite


## Integration Process
I built the application using QtWidgets because I found it more intuitive and closer to standard C++ than QtQuick. This way, I was able to design the UI without learning QML and focus on writing everything in C++.
The result is a simple but functional interface that meets the basic needs of the app.

To optimize performance and reduce the executable size, I used the Qt Resource Compiler (qrc) to embed a list of cities directly into the application for the search bar autofill feature.
Instead of storing weather data locally, the application fetches it from a REST API. The REST API is responsible for fetching, parsing, and weather prediction to provide accurate weather information. 

To avoid redundant calculations and reduce server load, forecast data for each city and date is cached in the database once per day. While this approach isn't ideal for the most accurate weather updates, it was a practical choice due to resource constraints.
It still ensures faster responses for future queries and can be easily adjusted later if needed.


## Installation
The latest executable build of Weather Prophet is available for download in the [Releases section](https://github.com/CL0001/weather-prophet/releases).

## Project Setup
### 1. Get the Source
Clone the Repository using Git or download it as a ZIP file:
```bash
git clone https://github.com/CL0001/weather-prophet.git
```

### 2. Run the REST API with Docker
Navigate to the `WeatherProphet.api` directory and build the Docker container, note this may take a few minutes:
```bash
docker build -t weather-prophet-api .
docker run -d -p 8080:80 weather-prophet-api
```
### 3. Open and Build the Application in QtCreator
1. Open QtCreator.
2. Click **"Open Project"** and select `CMakeLists.txt` from the `weather-prophet-app` directory
3. Configure the project as needed, then build and run it.
