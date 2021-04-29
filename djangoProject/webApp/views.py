from django.shortcuts import render

# Create your views here.

from rest_framework import viewsets, filters, pagination
from django_filters.rest_framework import DjangoFilterBackend
from webApp import serializers, models


class UserSensorDataPagination(pagination.PageNumberPagination):
    page_size = 5
    max_page_size = 100


class UserSensorDataViewSet(viewsets.ModelViewSet):
    """
    无线火灾报警系统 获取装置当前状态 API。
    """
    queryset = models.UserSensorDataModel.objects.all()
    pagination_class = UserSensorDataPagination
    serializer_class = serializers.UserSensorDataSerializer
    filter_backends = [DjangoFilterBackend, filters.SearchFilter, filters.OrderingFilter]
    filter_fields = '__all__'


class UserSensorConfigViewSet(viewsets.ModelViewSet):
    """
    无线火灾报警系统 获取装置当前配置 API。
    """
    queryset = models.UserSensorConfigModel.objects.all()
    pagination_class = UserSensorDataPagination
    serializer_class = serializers.UserSensorConfigSerializer
    filter_backends = [DjangoFilterBackend, filters.SearchFilter, filters.OrderingFilter]
    filter_fields = '__all__'
