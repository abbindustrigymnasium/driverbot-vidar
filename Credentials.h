#ifndef ARDUINO_CREDENTIALS_H
#define ARDUINO_CREDENTIALS_H

/* WiFi Credentials to connect Internet */
#define STA_SSID "Bifrost"
#define STA_PASS "Roggbi55"

/* Provide MQTT broker credentials as denoted in maqiatto.com. */
#define MQTT_BROKER       "maqiatto.com"
#define MQTT_BROKER_PORT  1883
#define MQTT_USERNAME     "vidar.morin@hitachigymnasiet.se"
#define MQTT_KEY          "1234"


/* Provide topic as it is denoted in your topic list. 
 * For example mine is : cadominna@gmail.com/topic1
 * To add topics, see https://www.maqiatto.com/configure
 */
#define TOPIC    "vidar.morin@hitachigymnasiet.se/broker_driverbot"

#endif /* ARDUINO_CREDENTIALS_H */
