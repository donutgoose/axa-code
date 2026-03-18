import RPi.GPIO as GPIO
from time import sleep
GPIO.setmode(GPIO.BOARD)

def step_motor1A():
    GPIO.output(motor1A, GPIO.HIGH)
    sleep(0.524)
    GPIO.output(motor1A, GPIO.LOW)

motor1A=16

GPIO.setup(motor1A, GPIO.OUT)

while True:
    step_motor1A()
    sleep(604800)