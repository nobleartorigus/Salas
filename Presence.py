#GNU nano 2.7.4                             File: Presence.py                                       

from SensorPIR import SensorPIR
from Camera import Camerashoot
import time


x, y = SensorPIR()
print(x, y)
secs = 0
min = 0
#ID de sala 1, pendiente sistema para identificar IDs de salas diversas 
id = 1
while x == 1:
#<<<<<<< HEAD
	x, y = SensorPIR()
	if secs != 10:
		time.sleep(1)
        	secs += 1
	time.sleep(60)
	Camerashoot()
	#Despues de 60 s se suma un minuto al contador y este toma una captura cada minuto mientras permanezca en alto
	min += 1 
#=======
        x, y = SensorPIR()
        if secs != 10:
                time.sleep(1)
                secs += 1
        time.sleep(60)
        Camerashoot()
        #Despues de 60 s se suma un minuto al contador y este toma una captura cada minuto mientras $
        min += 1 
#>>>>>>> 774c840fb09ee19ade6f806bdf98ef4f184535bd

min = str(min)
id = str(id)
print('\n' + min)
x = str(x)
y = str(y)
f = open('/home/chicago/Salas/Registro.txt','a')
f1 = open('/home/chicago/Salas/UsoSalas.csv', 'a')
f.write('\n' + x + '\n' + y)
f.close()
#<<<<<<< HEAD
f1.write('\n' + id + '\t' + t)
f1.close()
#=======
f1.write('\n' + id + '\t' + min)
f1.close()

#>>>>>>> 774c840fb09ee19ade6f806bdf98ef4f184535bd
