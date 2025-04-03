from django.contrib import admin

# Register your models here.
from .models import Activity
from .models import Activities

admin.site.register(Activity)
admin.site.register(Activities)
