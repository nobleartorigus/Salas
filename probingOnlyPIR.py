
#Sensor PIR
import RPi.GPIO as GPIO
import time

#GPIO.cleanup()           #Clean ports
GPIO.setmode(GPIO.BOARD) #Physical pin counter

sensor_pir_input = 40
led_pir_indicator = 11

time_initiate_counting = 0
time_total_counted = 0

GPIO.setup(sensor_pir_input,GPIO.IN)
GPIO.setup(led_pir_indicator,GPIO.OUT)

try:
  while True:
    pir_lecture = GPIO.input(sensor_pir_input)
    if pir_lecture == 1:
      GPIO.output(led_pir_indicator,1)
      time_initiate_counting = time.time()
      #print pir_lecture
      while pir_lecture == 1:
        pir_lecture = GPIO.input(sensor_pir_input)
        time.sleep(0.1)
      else:
        time_total_counted = time.time() - time_initiate_counting
        #print time_total_counted
        GPIO.output(led_pir_indicator,0)
        #print pir_lecture
        #print
    #y = time.strftime("%c")
    time.sleep(0.2)

except KeyboardInterrupt:
  GPIO.cleanup()

