import RPi.GPIO as GPIO
import http.client
import sys
import Adafruit_DHT

import urllib
import time
key = "ZPSHDKHE8K1CRFES"
sleep=30
channel = 21
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(channel, GPIO.IN)
GPIO.setup(12, GPIO.OUT)
def thermometer():
    while True:
       

        a=int(GPIO.input(channel))
        if(a>50):
            print("Water  Not Detected!")
            GPIO.output(12,GPIO.HIGH)
            #time.sleep(10)
            H = 0
                       
        else:
             print("Water Detected!")
             GPIO.output(12,GPIO.LOW)
            # time.sleep(10)
             H = 1
        humidity, temperature = Adafruit_DHT.read_retry(11,20)
        print('Air Temperature: {0:0.1f} C'.format(temperature))
        print('Humidity in air: {0:0.1f}%'.format(humidity))
       
        T = temperature
        A = humidity
        params = urllib.parse.urlencode({'field1': H,'field2':T,'field3':A, 'key':key })
        headers = {"Content-typZZe": "application/x-www-form-urlencoded","Accept": "text/plain"}
        conn = http.client.HTTPConnection("api.thingspeak.com:80")
        try:
            conn.request("POST", "/update", params, headers)
            response = conn.getresponse()
            #print (H)
            #print (response.status, response.reason)
            data = response.read()
            conn.close()
        except:
            print ("connection failed")
        break
if __name__ == "__main__":
        while True:
                thermometer()
                time.sleep(10)
