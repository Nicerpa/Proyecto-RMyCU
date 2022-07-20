from paho.mqtt import client as mqtt_client
import random
import csv
import matplotlib.pyplot as plt

broker = 'localhost'
port = 1883
topic = "/topico"
client_id = f'python-mqtt-{random.randint(0, 1000)}'

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def subscribe(client: mqtt_client):

    f_name = "data_sub_"+client_id+".csv"
    f = open(f_name, 'a')
    # esta funcion es lo que se quiere hacer con la data
    def on_message(client, userdata, msg):
        mensaje = msg.payload.decode()
        print(msg.payload.decode())
        f.write(mensaje)

    
    client.subscribe(topic)
    client.on_message = on_message


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()

if __name__ == '__main__':
    run()
