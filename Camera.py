import picamera
import time
from datetime import datetime

def Camerashoot():
	with picamera.PiCamera() as picam:
		picam.vflip = True
		picam.start_preview()
		time.sleep(1)
		clck = datetime.now()
		clck.strftime("%A, %d. %B %Y %H:%M%p")
		picam.capture('/home/chicago/Salas/Photos/foto{0:%d}{0:%B}{0:%H:%M%p}.jpg'.format(clck))
		picam.stop_preview()
		picam.close()
	
