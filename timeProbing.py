#Sensor PIR
import RPi.GPIO as GPIO
import time

GPIO.cleanup()           #Clean ports
GPIO.setmode(GPIO.BOARD) #Physical pin counter

sensor_pir_input = 7
led_pir_indicator = 11

time_initiate_counting = 0
time_total_counted = 0

GPIO.setup(sensor_pir_input,GPIO.IN)
GPIO.setup(led_pir_indicator,GPIO.OUT)

try:
  while True:
    x = GPIO.input(sensor_pir_input)

    if x == 1:
      GPIO.output(led_pir_indicator,1)
    else:
      GPIO.output(led_pir_indicator,0)
    #y = time.strftime("%c")
    print x
    time.sleep(0.2)

except KeyboardInterrupt:
  GPIO.cleanup()

