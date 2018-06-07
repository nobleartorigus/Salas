#Sensor PIR
import RPi.GPIO as GPIO
import time

def SensorPIR():
	
	GPIO.setmode(GPIO.BOARD)
	PIR_PIN = 40
	GPIO.setup(PIR_PIN, GPIO.IN)
	x = GPIO.input(PIR_PIN)
	y = time.strftime("%c")
	return x, y
