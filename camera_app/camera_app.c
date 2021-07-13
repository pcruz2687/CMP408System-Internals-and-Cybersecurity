#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "piio_driver.h"
#include "MQTTClient.h"

#define ADDRESS "52.91.232.191"
#define CLIENTID "RaspberryPiApp"
#define TOPIC "Raspberry Control"
#define QOS 1
#define TIMEOUT 10000L

volatile MQTTClient_deliveryToken deliveredtoken;
char mqttmsg[8]; // Global array to store message->payload 

lkm_data lkmdata;
gpio_pin gpiopin;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
    	deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	int i;
	char* payloadptr;
	printf("Message Received:\n");
	printf("     Topic: %s\n", topicName);
	printf("   Message: ");
	payloadptr = message->payload;
	for(i=0; i<message->payloadlen; i++)
	{
		putchar(*payloadptr++);
	}
	putchar('\n');
	memcpy(mqttmsg, message->payload, strlen(message->payload) + 1);
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}
void connlost(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}

int main(int argc, char *argv[]) {
	int fd, ret;
	fd = open("//dev//piio_driver_dev", O_WRONLY);

	// Associated pins for green and red LEDs
	char green_led_pin[2] = "23";
	char red_led_pin[2] = "12";
	char on[1] = "1";
	char off[1] = "0";

	// Set app as an MQTT subscriber
	MQTTClient client;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	int rc;
	int ch;

	MQTTClient_create(&client, ADDRESS, CLIENTID,
		MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
		printf("Failed to connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}

	printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
  		"Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
	MQTTClient_subscribe(client, TOPIC, QOS);

	do {
		printf("Waiting for message...\n");
	} while(!strlen(mqttmsg));

	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);

	if(fd < 0) {
		printf("Can't open device file: %s\n", DEVICE_NAME);
		exit(-1);
	}

	// Execute if MQTT message is "capture"
	if(strncmp(mqttmsg, "capture", 8) == 0) {
		// Turn off Red LED if it's on
		memset(&gpiopin, 0, sizeof(gpiopin));
		gpiopin.red_pin = strtol(red_led_pin, NULL, 10);
		ret = ioctl(fd, IOCTL_PIIO_GPIO_RED_LED_WRITE, &gpiopin);
		// Turn on Green LED
		gpiopin.green_pin = strtol(green_led_pin, NULL, 10);
		gpiopin.green_value = strtol(on, NULL, 10);
		strcpy(gpiopin.desc, "Green LED Pin");
		ret = ioctl(fd, IOCTL_PIIO_GPIO_GREEN_LED_WRITE, &gpiopin);
		printf("Green LED ON: pin %i - value %i\n", gpiopin.green_pin, gpiopin.green_value);

		// Run the python script to turn on the camera and take photos
		int a = system("python camera_app.py");

		// Camera was successful, turn off Green LED
		if(!a){
			gpiopin.green_value = strtol(off, NULL, 10);
			ret = ioctl(fd, IOCTL_PIIO_GPIO_GREEN_LED_WRITE, &gpiopin);
			printf("Green LED OFF: pin %i - value %i\n", gpiopin.green_pin, gpiopin.green_value);

			// Turn on Red LED
			memset(&gpiopin, 0, sizeof(gpiopin));
			gpiopin.red_pin = strtol(red_led_pin, NULL, 10);
			gpiopin.red_value = strtol(on, NULL, 10);
			strcpy(gpiopin.desc, "Red LED Pin");
			ret = ioctl(fd, IOCTL_PIIO_GPIO_RED_LED_WRITE, &gpiopin);
			printf("RED LED ON: pin %i - value %i\n", gpiopin.red_pin, gpiopin.red_value);
			printf("Code: %i - Photo successfully taken.\n", a);
		}
	} else {
		printf("Invalid command message");
	}

	close(fd);
	printf("Exit 0\n");
	return rc;
}
