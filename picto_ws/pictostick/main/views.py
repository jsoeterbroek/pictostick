# Create your views here.
from django.http import HttpResponse
from django.template import loader
from .models import Serial, Activities, Activity, FuActivity

def index(request):
    return HttpResponse("Hello, world. You're at the main index view.")

def change(request):
    return HttpResponse("Hello, world. You're at the main change view.")

def list(request):
    sorted_activities_list = Activity.objects.order_by("order")
    template = loader.get_template("activities/list.html")
    context = {"sorted_activities_list": sorted_activities_list}
    return HttpResponse(template.render(context, request))

weekdays = [
    { 'day': 'Mon', 'id': 1},
    { 'day': 'Tue', 'id': 2},
    { 'day': 'Wed', 'id': 3},
    { 'day': 'Thu', 'id': 4},
    { 'day': 'Fri', 'id': 5},
    { 'day': 'Sat', 'id': 6},
    { 'day': 'Sun', 'id': 7},
        ]
def dragdrop(request):
    sorted_av_activities_list = Activity.objects.order_by("order")
    sorted_fu_activities_list = FuActivity.objects.order_by("order")
    template = loader.get_template("activities/drag_drop.html")
    context = {"sorted_av_activities_list": sorted_av_activities_list,
               "weekdays": weekdays, }
    return HttpResponse(template.render(context, request))
