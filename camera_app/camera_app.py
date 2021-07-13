from picamera import PiCamera
from time import sleep
from aws_s3_upload import *
from mysql_insert_url import *

camera = PiCamera()
image_path = 'captured_images/piimg.jpg'
try:
	camera.start_preview()
	sleep(2) # for camera to sense light levels
	camera.capture(image_path)
	camera.stop_preview()

	# Check if the image has been saved
	f = open("captured_images/piimg.jpg")
    	if(f):
		f.close()

	# Insert image url to the database
	insert_url(s3_upload(image_path))
except IOError:
	print("File not accessible")
