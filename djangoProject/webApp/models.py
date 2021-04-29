from django.db import models


# Create your models here.

class UserSensorDataModel(models.Model):
    temperature = models.FloatField('温度')
    smokescope = models.FloatField('烟雾浓度')
    updateTime = models.DateTimeField(auto_now=True)


class UserSensorConfigModel(models.Model):
    temperatureThreshold = models.FloatField('温度阈值')
    concentrationThreshold = models.FloatField('浓度阈值')
    updateTime = models.DateTimeField(auto_now=True)
