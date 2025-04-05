from django.contrib import admin

# Register your models here.
from .models import Serial
from .models import Activity
from .models import Activities

admin.site.register(Serial)
admin.site.register(Activity)
admin.site.register(Activities)
