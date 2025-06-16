import torch
import numpy as np
import calendar

from datetime import datetime, timezone, timedelta
from database import CityData


def kelvin_to_celsius(kelvin):
    return kelvin - 273.15


def kelvin_to_fahrenheit(kelvin):
    return (kelvin - 273.15) * 9/5 + 32


def fahrenheit_to_celsius(fahrenheit):
    return (fahrenheit - 32) * 5/9


def timestamp_to_datetime(timestamp):
    return datetime.fromtimestamp(timestamp, tz=timezone.utc).strftime('%H:%M:%S')


def get_prediction(model, vals):
    with torch.inference_mode():
        vals = vals.clone().detach().float()
        vals = vals.to("cpu")
        model = model.to("cpu")
        
        vals = vals.view(1, -1)
        pred = model(vals)
        pred_np = pred.cpu().numpy()
        pred_rounded = np.round(pred_np, 1)
        pred_rounded = pred_rounded.flatten()

    return pred_rounded


def format_response(weather_data, model):
    features = [
        kelvin_to_fahrenheit(weather_data["main"]["temp"]),
        kelvin_to_fahrenheit(weather_data["main"]["feels_like"]),
        weather_data["main"]["humidity"],
        weather_data["wind"]["speed"],
        weather_data["main"]["pressure"]
    ]
    
    tensor = torch.tensor(features, dtype=torch.float32)
    prediction = get_prediction(model, tensor)
    prediction = prediction.astype(float)

    today = datetime.now()
    days = [(today + timedelta(days=i)).strftime("%A") for i in range(1, 6)]

    return {
        "name": weather_data["name"],
        "lon": round(weather_data["coord"]["lon"], 1),
        "lat": round(weather_data["coord"]["lat"], 1),
        "weather_description": weather_data["weather"][0]["description"],
        "temp": round(kelvin_to_celsius(weather_data["main"]["temp"]), 1),
        "feels_like": round(kelvin_to_celsius(weather_data["main"]["feels_like"]), 1),
        "temp_min": round(kelvin_to_celsius(weather_data["main"]["temp_min"]), 1),
        "temp_max": round(kelvin_to_celsius(weather_data["main"]["temp_max"]), 1),
        "pressure": round(weather_data["main"]["pressure"], 1),
        "humidity": round(weather_data["main"]["humidity"], 1),
        "sea_level": round(weather_data["main"]["sea_level"], 1),
        "wind_speed": round(weather_data["wind"]["speed"], 1),
        "sunrise": timestamp_to_datetime(weather_data["sys"]["sunrise"]),
        "sunset": timestamp_to_datetime(weather_data["sys"]["sunset"]),

        "day1": days[0],
        "value1": f"{min(round(float(fahrenheit_to_celsius(prediction[0])), 1), round(float(fahrenheit_to_celsius(prediction[1])), 1))} - {max(round(float(fahrenheit_to_celsius(prediction[0])), 1), round(float(fahrenheit_to_celsius(prediction[1])), 1))}°C",
        "day2": days[1],
        "value2": f"{min(round(float(fahrenheit_to_celsius(prediction[2])), 1), round(float(fahrenheit_to_celsius(prediction[3])), 1))} - {max(round(float(fahrenheit_to_celsius(prediction[2])), 1), round(float(fahrenheit_to_celsius(prediction[3])), 1))}°C",
        "day3": days[2],
        "value3": f"{min(round(float(fahrenheit_to_celsius(prediction[4])), 1), round(float(fahrenheit_to_celsius(prediction[5])), 1))} - {max(round(float(fahrenheit_to_celsius(prediction[4])), 1), round(float(fahrenheit_to_celsius(prediction[5])), 1))}°C",
        "day4": days[3],
        "value4": f"{min(round(float(fahrenheit_to_celsius(prediction[6])), 1), round(float(fahrenheit_to_celsius(prediction[7])), 1))} - {max(round(float(fahrenheit_to_celsius(prediction[6])), 1), round(float(fahrenheit_to_celsius(prediction[7])), 1))}°C",
        "day5": days[4],
        "value5": f"{min(round(float(fahrenheit_to_celsius(prediction[8])), 1), round(float(fahrenheit_to_celsius(prediction[9])), 1))} - {max(round(float(fahrenheit_to_celsius(prediction[8])), 1), round(float(fahrenheit_to_celsius(prediction[9])), 1))}°C"
    }


def format_query(city_record):
    return {
        "name": city_record.name,
        "lon": city_record.lon,
        "lat": city_record.lat,
        "weather_description": city_record.weather_description,
        "temp": city_record.temperature,
        "feels_like": city_record.feels_like,
        "temp_min": city_record.min_temperature,
        "temp_max": city_record.max_temperature,
        "pressure": city_record.pressure,
        "humidity": city_record.humidity,
        "sea_level": city_record.sea_level,
        "wind_speed": city_record.wind_speed,
        "sunrise": city_record.sunrise,
        "sunset": city_record.sunset,
        "day1": city_record.day1,
        "value1": city_record.value1,
        "day2": city_record.day2,
        "value2": city_record.value2,
        "day3": city_record.day3,
        "value3": city_record.value3,
        "day4": city_record.day4,
        "value4": city_record.value4,
        "day5": city_record.day5,
        "value5": city_record.value5,
    }


def format_for_database(weather_data):
    return CityData(
        name=weather_data["name"],
        lon=weather_data["lon"],
        lat=weather_data["lat"],
        weather_description=weather_data["weather_description"],
        temperature=weather_data["temp"],
        feels_like=weather_data["feels_like"],
        min_temperature=weather_data["temp_min"],
        max_temperature=weather_data["temp_max"],
        pressure=weather_data["pressure"],
        humidity=weather_data["humidity"],
        sea_level=weather_data["sea_level"],
        wind_speed=weather_data["wind_speed"],
        sunrise=weather_data["sunrise"],
        sunset=weather_data["sunset"],
        day1=weather_data["day1"],
        value1=weather_data["value1"],
        day2=weather_data["day2"],
        value2=weather_data["value2"],
        day3=weather_data["day3"],
        value3=weather_data["value3"],
        day4=weather_data["day4"],
        value4=weather_data["value4"],
        day5=weather_data["day5"],
        value5=weather_data["value5"],
        created_at=datetime.now(timezone.utc)
    )