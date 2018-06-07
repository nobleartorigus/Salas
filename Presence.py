#
from SensorPIR import SensorPIR
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD) #Physical pin counter
led_pir_indicator = 11
secs = 0
min = 0
#time_initiate_1_counting = 0
#time_initiate_0_counting = 0
time_total_1_counted = 0
time_total_0_counted = 0
GPIO.setup(led_pir_indicator, GPIO.OUT)
#ID de sala 1, pendiente sistema para identificar IDs de salas diversas 
id = 1
try:
	while True:
		x, y = SensorPIR()
#		print(x)
		if x == 1:
			time_total_1_counted += 1
			print(str(time_total_1_counted)+' ON')
			#GPIO.output(led_pir_indicator, 1)
			#time_initiate_1_counting = time.time()
			#time_total_0_counted = time.time() - time_initiate_0_counting
			#time_total_0_counted_int = int(time_total_0_counted)
			if time_total_1_counted > 15:
				GPIO.output(led_pir_indicator, 1)
				time_total_0_counted = 0

		elif x == 0:
			time_total_0_counted += 1
			print(str(time_total_0_counted) + ' off')
			#GPIO.output(led_pir_indicator, 0)
			#time_initiate_0_counting = time.time()
			#time_total_1_counted = time.time() - time_initiate_1_counting
			#time_total_1_counted_int = int(time_total_1_counted) 
			if time_total_0_counted > 15:
				GPIO.output(led_pir_indicator, 0)
				time_total_1_counted = 0


#		x = str(x)
#		y = str(y)
#		id = str(id)
#		f1 = open('/home/chicago/Salas/UsoSalas.csv', 'a')
#		f1.write('\n' + id + '\t' + y + '\t' +  x)
#		f1.close()
		time.sleep(0.5)


except KeyboardInterrupt:
  GPIO.cleanup()

