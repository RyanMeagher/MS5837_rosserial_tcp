#include <Arduino.h>
#include "../lib/WiFiNINA/src/WiFi.h"
#include "../lib/rosserial_arduino/src/std_msgs/String.h"
#include "../lib/rosserial_arduino/src/ros.h"
#include "../lib/rosserial_arduino/src/std_msgs/Int16.h"

const char ssid[] = "ATTGI6AFpA";        // your network SSID (name)
const char password[] = "qi8?8+5jg?%j";    // your network password (use for WPA, or use as key for WEP)
IPAddress server(192, 168, 1, 125); // ip of your ROS server
WiFiClient client;

int status = WL_IDLE_STATUS;

//add a class that provides the interface between your WiFi shield and rosserial_arduino
class WiFiHardware {
public:
    WiFiHardware() = default;

    void init(nullptr_t aNullptr) {
        // do your initialization here. this probably includes TCP server/client setup
        Serial.println("connecting to server");
        client.connect(server, 11411);
        Serial.println("connected to server");
    }

    // read a byte from the serial port. -1 = failure
    int read() {
        // implement this method so that it reads a byte from the TCP connection and returns it
        //  you may return -1 is there is an error; for example if the TCP connection is not open
        return client.read();         //will return -1 when it will works
    }

    // write data to the connection to ROS
    void write(uint8_t *data, int length) {
        // implement this so that it takes the arguments and writes or prints them to the TCP connection
        for (int i = 0; i < length; i++)
            client.write(data[i]);
    }

    // returns milliseconds since start of program
    unsigned long time() {
        return millis(); // easy; did this one for you
    }
};

//setup a ros node that will publish a standard string msg
std_msgs::Int16 str_msg;
ros::NodeHandle_<WiFiHardware> nh;
ros::Publisher depthPublisher("depthPublisher", &str_msg);
char hello[13] = "hello world!";


void setupWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("\nConnecting to ");
    Serial.println(ssid);
    uint8_t i = 0;
    while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
    if (i == 21) {
        Serial.print("Could not connect to");
        Serial.println(ssid);
        while (1) delay(500);
    }
    Serial.print("Ready! Use ");
    Serial.print(WiFi.localIP());
    Serial.println(" to access client");
}

void setup() {

    Serial.begin(9600);
    setupWiFi();
    nh.initNode();
    nh.advertise(depthPublisher);

    delay(2000);


}

void loop() {
    static int i = 0;
    str_msg.data = i++;
    depthPublisher.publish(&str_msg);
    nh.spinOnce();
    delay(100);
}