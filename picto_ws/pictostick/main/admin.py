from django.contrib import admin

# Register your models here.
from .models import Serial
from .models import ActivityList
from .models import Picto

admin.site.register(Serial)
admin.site.register(ActivityList)
admin.site.register(Picto)
