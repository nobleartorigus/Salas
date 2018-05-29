#Sensor PIR
import RPi.GPIO as GPIO
import time

GPIO.cleanup()
GPIO.setmode(GPIO.BOARD)

pir_in = 7
led_out = 11

GPIO.setup(pir_in,GPIO.IN)
GPIO.setup(led_out,GPIO.OUT)

try:
  while True:
    x = GPIO.input(pir_in)

    if x == 1:
      GPIO.output(led_out,True)
    else:
      GPIO.output(led_out,False)
    #y = time.strftime("%c")
    print x
    time.sleep(5)

except KeyboardInterrupt:
  GPIo.cleanup()

