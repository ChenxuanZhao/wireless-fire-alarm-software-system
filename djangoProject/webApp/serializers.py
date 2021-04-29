from rest_framework import serializers
from webApp import models


class UserSensorDataSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = models.UserSensorDataModel
        fields = '__all__'


class UserSensorConfigSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = models.UserSensorConfigModel
        fields = '__all__'
