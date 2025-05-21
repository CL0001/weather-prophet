import os
import datetime
import requests

import torch

from fastapi import FastAPI, Depends, HTTPException
from sqlalchemy.orm import Session
from geopy.geocoders import Nominatim
from geopy.exc import GeocoderTimedOut
from dotenv import load_dotenv

from model import WeatherProphet
from helper_functions import format_response, format_query, format_for_database
from database import *

load_dotenv()

model = WeatherProphet()
model.load_state_dict(torch.load("weather_prophet_model.pth", weights_only=False))
model.eval()

app = FastAPI()
location = Nominatim(user_agent="Geopy Library")

def get_db():
    db = session()
    try:
        yield db
    finally:
        db.close()

@app.get('/')
def root():
    return {"message": "Hello World!"}

@app.get('/city/{name}')
def city_weather_data(name: str, db: Session = Depends(get_db)):
    try:
        city_record = db.query(CityData).filter(CityData.name == name).order_by(CityData.id.desc()).first()
        today_date = datetime.datetime.now(datetime.UTC).date()
        if city_record and city_record.created_at.date() == today_date:
            return format_query(city_record)
        else:
            city_location = location.geocode(name, timeout=10)
            if city_location is None:
                raise ValueError("City not found")

            api_key = os.getenv('openweather_key')
            if not api_key:
                raise EnvironmentError("OpenWeather API key not found in environment variables")

            city_stats = requests.get(f"https://api.openweathermap.org/data/2.5/weather?lat={city_location.latitude}&lon={city_location.longitude}&appid={api_key}")
            if city_stats.status_code != 200:
                raise HTTPException(status_code=city_stats.status_code, detail="Failed to fetch weather data")

            formatted_data = format_response(city_stats.json(), model)

            new_city_data = format_for_database(formatted_data)

            db.add(new_city_data)
            db.commit()
            db.refresh(new_city_data)

        return formatted_data
    except GeocoderTimedOut:
        raise HTTPException(status_code=504, detail="Geocoding service timed out. Please try again.")
    except ValueError as e:
        raise HTTPException(status_code=404, detail=str(e))
    except EnvironmentError as e:
        raise HTTPException(status_code=500, detail=str(e))
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"An unexpected error occurred: {e}")
