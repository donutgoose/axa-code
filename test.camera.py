from picamera2 import Picamera2, Preview
from time import sleep

open('test1.txt','w+')

picam2 = Picamera2()
camera_config = picam2.create_still_configuration(main={"size": (1920, 1080)}, lores={"size": (1080, 1080)}, display="lores")
picam2.configure(camera_config)
picam2.start_preview(Preview.QTGL)
picam2.start()

open('test.txt','w+')

for i in range(1,7500):
    picam2.capture_file('/home/cubesat/Pictures/test/photo{0:04d}.jpg'.format(i))
    sleep(60)






