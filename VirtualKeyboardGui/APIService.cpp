#include "APIService.h"
#include "MQTTClient.h"
#include <iostream>
#include <string>
#include <array>
#include <stdio.h>
#include <QFile>
#include <QTextStream>
#include <windows.h>

using namespace std;

#define ADDRESS     "192.168.0.4:1883"
//#define ADDRESS     "192.168.0.4:3123"
#define CLIENTID    "UI_IOT"

//#define TOPIC		"CyberLink/commands/1037600"
//#define TOPIC		  "iot/control"

#define QOS         1
#define TIMEOUT     10000L

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;
int rc;

void APIService::PublishMessage(char* topic, char* message, int messLen)
{
	if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
		MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
	{
		//printf("Failed to create client, return code %d\n", rc);
		//exit(EXIT_FAILURE);
	}

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		//printf("Failed to connect, return code %d\n", rc);
		//exit(EXIT_FAILURE);
	}

	pubmsg.payload = message;
	pubmsg.payloadlen = messLen;
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	if ((rc = MQTTClient_publishMessage(client, topic, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
	{
		//printf("Failed to publish message, return code %d\n", rc);
		//exit(EXIT_FAILURE);
	}
}

void APIService::GetDevicesAPI()
{
	std::unique_ptr<FILE> pipe(_popen("python GetDeviceAPI.py", "r"));
	pipe.release();

	Sleep(1000);
}

QString APIService::ReadFile()
{
	QFile file("DeviceList.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "";
	QTextStream in(&file);
	QString data;
	data = in.readAll();
	file.close();

	return data;
}
