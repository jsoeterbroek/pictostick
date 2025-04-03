from django.db import models

# Create your models here. #_default array
_default = {
    "comment":  "",
    "version": "1.0.1",
    "name": "Thomas",
    "device_ip": "128.8.2.123",
    "activities": [
    {
        "order": "001",
        "picto": "alarm.png",
        "name_en": "Wake up",
        "name_nl": "Opstaan"
    },
    {
        "order": "002",
        "picto": "shower.png",
        "name_en": "Shower",
        "name_nl": "Douchen"
    },
    {
        "order": "003",
        "picto": "selfcare.png",
        "name_en": "Self care",
        "name_nl": "Zelfzorg"
    },
    {
        "order": "004",
        "picto": "breakfast.png",
        "name_en": "Breakfast",
        "name_nl": "Ontbijt"
    },
    {
        "order": "005",
        "picto": "book.png",
        "name_en": "Reading",
        "name_nl": "Lezen"
    },
    {
        "order": "006",
        "picto": "lunch.png",
        "name_en": "Lunch",
        "name_nl": "Lunch"
    },
    {
        "order": "007",
        "picto": "shoppingcart.png",
        "name_en": "Shopping",
        "name_nl": "Boodschappen"
    },
    {
        "order": "008",
        "picto": "tennis.png",
        "name_en": "Tennis",
        "name_nl": "Tennis"
    },
    {
        "order": "009",
        "picto": "pizza.png",
        "name_en": "Dinner",
        "name_nl": "Avondmaal"
    },
    {
        "order": "010",
        "picto": "games.png",
        "name_en": "Games",
        "name_nl": "Spelletjes"
    },
    {
        "order": "011",
        "picto": "tv.png",
        "name_en": "Watch tv",
        "name_nl": "Tv kijken"
    },
    {
        "order": "012",
        "picto": "taxi.png",
        "name_en": "Taxi",
        "name_nl": "Taxi"
    },
    {
        "order": "013",
        "picto": "bedtime.png",
        "name_en": "Bedtime",
        "name_nl": "Slapen"
    }
    ],
    "date_created": "29-03-2025",
    "date_valid": "30-03-2025"
}


print(type(_default))
print(_default)
