import mysql.connector
from mysql.connector import Error
from mysql.connector import errorcode

def insert_url(image_url):
	try:
		my_db = mysql.connector.connect(
					user = "admin",
					password = "adminpass",
					host = "aae50jppq99xii.cb2mjdifr9ad.us-east-1.rds.amazonaws.com",
					database = "dbcmp408")

		my_cursor = my_db.cursor()

		query = "INSERT INTO images (image_url) VALUES (%s)"
		value = (image_url,)
		my_cursor.execute(query, value)
		my_db.commit()
	except mysql.connector.Error as error:
		print("Failed to insert record into images table {}".format(error))
	finally:
		my_db.close()
		print("MySQL connection is closed")

		return("image url inserted.")
