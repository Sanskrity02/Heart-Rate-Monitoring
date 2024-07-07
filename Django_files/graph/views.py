import serial
import time
from django.http import JsonResponse
from django.shortcuts import render

serialInst = serial.Serial()
serialInst.baudrate = 9600
serialInst.port = 'COM14'  # Specify the COM port you want to use

def live_graph(request):
    if not serialInst.is_open:
        serialInst.open()

    graph_data = {
        'x': [],
        'y': []
    }

    while True:
        if serialInst.in_waiting:
            packet = serialInst.readline()
            data = packet.decode('utf').rstrip('\n')
            current_time = time.strftime('%H:%M:%S')
            try:
                data_float = float(data)
                graph_data['x'].append(current_time)
                graph_data['y'].append(data_float)
                print(data_float)
            except ValueError:
                print(f"Could not convert string to float: {data}")

        return render(request, 'graph/live_graph.html', {'graph_data': graph_data})

def graph_data(request):
    if not serialInst.is_open:
        serialInst.open()

    if serialInst.in_waiting:
        packet = serialInst.readline()
        data = packet.decode('utf').rstrip('\n')
        current_time = time.strftime('%H:%M:%S')
        try:
            data_float = (data)
            graph_data = {
                'x': current_time,
                'y': data_float
            }
        except ValueError:
            graph_data = {
                'x': None,
                'y': None
            }
            print(f"Could not convert string to float: {data}")
    else:
        graph_data = {
            'x': None,
            'y': None
        }

    return JsonResponse(graph_data)
