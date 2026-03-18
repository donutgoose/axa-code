import RPi.GPIO as GPIO
from picamera2 import Picamera2, Preview
from time import sleep, time

GPIO.setmode(GPIO.BOARD)

def step_motor1A():
    GPIO.output(motor1A, GPIO.HIGH)
    sleep(0.524)
    GPIO.output(motor1A, GPIO.LOW)
    sleep(604800 - 0.524)

motor1A = 16
camera_interval = 2  # Time in seconds between capturing images
camera_next_capture = time() + camera_interval
photos = 0

GPIO.setup(motor1A, GPIO.OUT)

picam2 = Picamera2()
camera_config = picam2.create_still_configuration(main={"size": (1920, 1080)}, lores={"size": (1080, 1080)},  display="lores")
picam2.configure(camera_config)
picam2.start_preview(Preview.QTGL)
picam2.start()

while True:
    current_time = time()
    if current_time >= camera_next_capture:
        photos += 1
        picam2.capture_file(f'/home/cubesat/Pictures/test/photo{photos}.jpg'.format(int(current_time)))
        camera_next_capture += camera_interval
    sleep(1) 