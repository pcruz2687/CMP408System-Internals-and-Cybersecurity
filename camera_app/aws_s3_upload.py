import boto3
import uuid

s3 = boto3.resource('s3')
s3_bucket = 'elasticbeanstalk-us-east-1-696248800713';
s3_url = 'https://elasticbeanstalk-us-east-1-696248800713.s3.amazonaws.com/'

def s3_upload(file_name):
	data = open(file_name, 'rb')
	key_name = 'captured_images/' + str(uuid.uuid4()) + '.jpg'
	s3.Bucket(s3_bucket).put_object(Key=key_name, Body=data, ACL='public-read', ContentType='image/jpg')
	image_url = s3_url + key_name
	return image_url
