from django.urls import path
from . import views

app_name = 'graph'

urlpatterns = [
    path('', views.live_graph, name='live_graph'),
    path('data/', views.graph_data, name='graph_data'),
]
