import random
import sys
from paho.mqtt import client as mqtt_client


broker = '34.228.255.141'
port = 1883
topic = "/topico"
client_id = f'python-mqtt-{random.randint(0, 1000)}'

def read_file(file):
    with open(file, 'r') as f:
        s = f.read()
    return s

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


def publish(client, msg):
    msg_count = 0
    result = client.publish(topic, msg)
    status = result[0]
    if status == 0:
        print(f"Send `{msg}` to topic `{topic}`")
    else:
        print(f"Failed to send message to topic {topic}")
    msg_count += 1


def run():
    client = connect_mqtt()
    client.loop_start()
    msg = read_file(sys.argv[1])
    publish(client, msg)


if __name__ == '__main__':
    run()
