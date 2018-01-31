import serial

ADC_POS_REF = 3.3
ADC_RES = 12.0

V_CAL = [0,0,0,0,0]  #[V0,V1,V2,V3,V4]
I_CAL = [0,0,0,0,0]  #[I0,I1,I2,I3,I4]
RES_CAL = [1000,10000,100000,1000000]

def dis2dec(n):
    global ADC_POS_REF,ADC_RES
    return n*ADC_POS_REF/2**ADC_RES

s =  serial.Serial('/dev/ttyACM0')

"""
***** obtener valores del ADC para V_CAL *****
***** obtener alimentacion para calcular I_CAL*****
"""
Vcc = 3.3

V_CAL = [dis2dec(x) for x in V_CAL]

for i in range(1,len(I_CAL)):
    I_CAL[i] = Vcc/(RES_CAL[i-1]+4.7+0.7)

while True:
    line = s.readline().split("XX")
    value = line[0]
    print(value)
