from SensorPIR import SensorPIR
from Camera import Camerashoot
import time

x, y = SensorPIR()
print(x, y)
if x == 1:
        Camerashoot()

x = str(x)
y = str(y)
f = open('/home/chicago/Salas/Registro.txt','a')
f.write('\n' + x + '\n' + y)
f.close()
