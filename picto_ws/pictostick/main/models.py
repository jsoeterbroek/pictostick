from django.db import models

class Activities(models.Model):
    pub_date = models.DateTimeField("date published")

class Activity(models.Model):
    order = models.CharField(max_length = 3)
    picto = models.CharField(max_length = 20)
    name_nl = models.CharField(max_length = 20)
    name_en = models.CharField(max_length = 20)
    activities = models.ForeignKey(Activities, on_delete=models.CASCADE)

    def __str__(self):
        return self.picto

    class Meta:
        ordering = ["order"]


