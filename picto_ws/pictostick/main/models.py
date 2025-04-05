from django.db import models

ACTIVITY_TYPES = (
    ('daily','DAILY'),
    ('weekly', 'WEEKLY'),
)

class Serial(models.Model):
    number = models.CharField(max_length = 20)

    def __str__(self):
        return self.number

class Activities(models.Model):
    pub_date = models.DateTimeField("date published")
    name_nl = models.CharField(max_length = 20, default='name_nl')
    name_en = models.CharField(max_length = 20, default='name en')
    serial = models.ForeignKey(Serial, on_delete=models.CASCADE)
    type = models.CharField(max_length=6, choices=ACTIVITY_TYPES, default='daily')

    def __str__(self):
        return self.type

# all available activities
class Activity(models.Model):
    order = models.IntegerField()
    picto = models.CharField(max_length = 20)
    name_nl = models.CharField(max_length = 20)
    name_en = models.CharField(max_length = 20)
    #activities = models.ForeignKey(Activities, on_delete=models.CASCADE)

    def __str__(self):
        return self.picto

    class Meta:
        ordering = ["order"]

# all frequantly used activities
class FuActivity(models.Model):
    order = models.IntegerField()
    picto = models.CharField(max_length = 20)
    name_nl = models.CharField(max_length = 20)
    name_en = models.CharField(max_length = 20)
    #activities = models.ForeignKey(Activities, on_delete=models.CASCADE)

    def __str__(self):
        return self.picto

    class Meta:
        ordering = ["order"]


