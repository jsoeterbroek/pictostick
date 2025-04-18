from django.urls import path

from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("change", views.change, name="change"),
    path("list", views.list, name="list"),
    path("dragdrop", views.dragdrop, name="dragdrop"),
]
