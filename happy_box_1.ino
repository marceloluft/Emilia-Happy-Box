// This #include statement was automatically added by the Particle IDE.
#include "SoftPWM/SoftPWM.h"

// This #include statement was automatically added by the Particle IDE.
#include "SparkIntervalTimer/SparkIntervalTimer.h"

// EMILIA PROTOTYPE I

/*EMILIA - Thesis Project by Marcelo Luft
OCAD University - Digital Futures*/

//TOP SENSORS
#define sens1 0  //IR 1 - Top Left conrner
#define sens2 1  //IR 2 - Top Center
#define sens3 2  //IR 3 - Top Right Corner

//BOTTOM SENSORS
#define sens4 3  //IR 4 - Bottom Left Corner
#define sens5 4  //IR 5 - Bottom Center
#define sens6 5  //IR 6 - Bottom Right Corner

int _sens1;
int _sens2;
int _sens3;
int _sens4;
int _sens5;
int _sens6;


int there_is_an_object = 600;

//----------------------------//

//TOP LIGHTS
#define led1 D1 //LED 1 - Top Left conrner
#define led2 D2 //LED 2 - Top Center
#define led3 D3 //LED 3 - Top Right Corner

//BOTTOM LIGHTS
#define led4 D5 //LED 4 - Bottom Center
#define led5 D4 //LED 5 - Bottom Center
#define led6 D6 //LED 6 - Bottom Right Corner

//----------------------------//

uint8_t leds[6] = {D1, D2, D3, D5, D4, D6};

//---------------------------//

//LED STATES
int led1_state = LOW;
int led2_state = LOW;
int led3_state = LOW;
int led4_state = LOW;
int led5_state = LOW;
int led6_state = LOW;


int _led1_state = LOW;
int _led2_state = LOW;
int _led3_state = LOW;
int _led4_state = LOW;
int _led5_state = LOW;
int _led6_state = LOW;


//----------------------------//


//PULSING STATES

int pulsing_led[6] = {LOW, LOW, LOW, LOW, LOW, LOW};

//----------------------------//

String led_states;
String previous_state;


unsigned long previousMillis = 0;

unsigned long interval = 1600;

unsigned int fadeUpTime = 2600;

unsigned int fadeDownTime = 2600;

int ledState = 0;

int brightness = 100;


//----------------------------//



void setup()
{
    String wifi_ssid = WiFi.SSID();

    if (WiFi.ready()) {
        Particle.publish("status_emilia_1", "Connected:"+wifi_ssid);

    } else {
       Particle.publish("status_emilia_1", "Not Connected");
    }

    Serial.begin(9600);

    SoftPWMBegin();

    SoftPWMSet(led1, 0);
    SoftPWMSet(led2, 0);
    SoftPWMSet(led3, 0);
    SoftPWMSet(led4, 0);
    SoftPWMSet(led5, 0);
    SoftPWMSet(led6, 0);
    Particle.subscribe("sensor_emilia_2", string_state);
    // Particle.function("emilia1_change_brightness",checkBrightness);
    Particle.function("listen_mode",listenMode);
    Particle.function("connect_to",connectTo);
    Particle.function("which_ssid",which_ssid);
    Particle.function("getAllSSID",getAllSSID);
}

void loop()
{

    _sens1 = (analogRead(sens1));
    _sens2 = (analogRead(sens2));
    _sens3 = (analogRead(sens3));
    _sens4 = (analogRead(sens4));
    _sens5 = (analogRead(sens5));
    _sens6 = (analogRead(sens6));


    if (_sens1 <= there_is_an_object) {
        led1_state = HIGH;

    } else {
        led1_state = LOW;
    }

    if (_sens2 <= there_is_an_object) {
        led2_state = HIGH;
    } else {;
        led2_state = LOW;
    }

     if (_sens3 <= there_is_an_object) {
        led3_state = HIGH;
    } else {
        led3_state = LOW;
    }

    if (_sens4 <= there_is_an_object) {
        led4_state = HIGH;
    } else {
        led4_state = LOW;
    }

    if (_sens5 <= there_is_an_object) {
        led5_state = HIGH;
    } else {
        led5_state = LOW;
    }

    if (_sens6 <= there_is_an_object) {
        led6_state = HIGH;
    } else {
        led6_state = LOW;
    }

    led_state();
    handle_sensors();

    #define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)
    unsigned long lastSync = millis();


    if (millis() - lastSync > ONE_DAY_MILLIS) {
        // Request time synchronization from the Particle Cloud
        Particle.syncTime();
        lastSync = millis();
    }

}

void pulse(int led, bool state) {

    SoftPWMSetFadeTime(led, 450, 450);

    if (state == HIGH) {
      SoftPWMSet(led, brightness);
    } else {
      SoftPWMSet(led, 0);
    }
}



void pulsing () {

    unsigned long currentMillis = millis();


    if (previousMillis<=0){
        previousMillis = currentMillis;
    }

    if (currentMillis - previousMillis > interval) {

        if (ledState == 0) {
            ledState = brightness;
        } else {
            ledState = 0;
        }


        for (int i = 0; i<6; i++) {

            if (pulsing_led[i] == HIGH) {
                SoftPWMSetFadeTime(leds[i], fadeUpTime, fadeDownTime);
                SoftPWMSet(leds[i], ledState);
            }
        }

        previousMillis = 0;

    }
}


void string_state(const char *name, const char *data) {

    int sensor1, sensor2, sensor3, sensor4, sensor5, sensor6;

    String sensors = data;

    for (unsigned int i = 0; i < sensors.length(); i++) {
        if (led_states.substring(i, i+1) == ",") {
            sensor1 = sensors.substring(i+1).toInt();
            sensor2 = sensors.substring(i+3).toInt();
            sensor3 = sensors.substring(i+5).toInt();
            sensor4 = sensors.substring(i+7).toInt();
            sensor5 = sensors.substring(i+9).toInt();
            sensor6 = sensors.substring(i+11).toInt();
            break;
        }
    }

    check_received_data(sensor1,sensor2,sensor3,sensor4,sensor5,sensor6);
}


void check_received_data (int sensor1,int sensor2,int sensor3,int sensor4,int sensor5,int sensor6) {


    if (sensor1 == HIGH) {
        _led1_state = HIGH;
    } else if (sensor1 == LOW) {
        _led1_state = LOW;
    }

    if (sensor2 == HIGH) {
        _led2_state = HIGH;
    } else if (sensor2 == LOW) {
        _led2_state = LOW;
    }

    if (sensor3 == HIGH) {
        _led3_state = HIGH;
    } else if (sensor3 == LOW) {
        _led3_state = LOW;
    }

    if (sensor4 == HIGH) {
        _led4_state = HIGH;
    } else if (sensor4 == LOW) {
        _led4_state = LOW;
    }

    if (sensor5 == HIGH) {
        _led5_state = HIGH;
    } else if (sensor5 == LOW) {
        _led5_state = LOW;
    }

    if (sensor6 == HIGH) {
        _led6_state = HIGH;
    } else if (sensor6 == LOW) {
        _led6_state = LOW;
    }

}



void handle_sensors () {


   //   SENSOR 1
   if (led1_state == HIGH && _led1_state == LOW) {
       pulse(led1, HIGH);
       pulsing_led[0] = LOW;
   } else if (led1_state == HIGH && _led1_state == HIGH) {
       pulsing_led[0] = HIGH;
   } else if (led1_state == LOW && _led1_state == HIGH) {
       pulse(led1, HIGH);
       pulsing_led[0] = LOW;
   } else if (led1_state == LOW && _led1_state == LOW) {
       pulse(led1, LOW);
       pulsing_led[0] = LOW;
   } else {
       pulsing_led[0] = LOW;
       pulse(led1, LOW);
   }


   //   SENSOR 2
   if (led2_state == HIGH && _led2_state == LOW) {
       pulse(led2, HIGH);
       pulsing_led[1] = LOW;
   } else if (led2_state == HIGH && _led2_state == HIGH) {
       pulsing_led[1] = HIGH;
   } else if (led2_state == LOW && _led2_state == HIGH) {
       pulse(led2, HIGH);
       pulsing_led[1] = LOW;
   } else if (led2_state == LOW && _led2_state == LOW) {
       pulse(led2, LOW);
       pulsing_led[1] = LOW;
   } else {
       pulsing_led[1] = LOW;
       pulse(led2, LOW);
   }


   //   SENSOR 3
   if (led3_state == HIGH && _led3_state == LOW) {
       pulse(led3, HIGH);
       pulsing_led[2] = LOW;
   } else if (led3_state == HIGH && _led3_state == HIGH) {
       pulsing_led[2] = HIGH;
   } else if (led3_state == LOW && _led3_state == HIGH) {
       pulse(led3, HIGH);
       pulsing_led[2] = LOW;
   } else if (led3_state == LOW && _led3_state == LOW) {
       pulse(led3, LOW);
       pulsing_led[2] = LOW;
   } else {
       pulsing_led[2] = LOW;
       pulse(led3, LOW);
   }

   //   SENSOR 4
   if (led4_state == HIGH && _led4_state == LOW) {
       pulse(led4, HIGH);
       pulsing_led[3] = LOW;
   } else if (led4_state == HIGH && _led4_state == HIGH) {
       pulsing_led[3] = HIGH;
   } else if (led4_state == LOW && _led4_state == HIGH) {
       pulse(led4, HIGH);
       pulsing_led[3] = LOW;
   } else if (led4_state == LOW && _led4_state == LOW) {
       pulse(led4, LOW);
       pulsing_led[3] = LOW;
   } else {
       pulsing_led[3] = LOW;
       pulse(led4, LOW);
   }

   //   SENSOR 5
   if (led5_state == HIGH && _led5_state == LOW) {
       pulse(led5, HIGH);
       pulsing_led[4] = LOW;
   } else if (led5_state == HIGH && _led5_state == HIGH) {
       pulsing_led[4] = HIGH;
   } else if (led5_state == LOW && _led5_state == HIGH) {
       pulse(led5, HIGH);
       pulsing_led[4] = LOW;
   } else if (led5_state == LOW && _led5_state == LOW) {
       pulse(led5, LOW);
       pulsing_led[4] = LOW;
   } else {
       pulsing_led[4] = LOW;
       pulse(led5, LOW);
   }


   //   SENSOR 6
   if (led6_state == HIGH && _led6_state == LOW) {
       pulse(led6, HIGH);
       pulsing_led[5] = LOW;
   } else if (led6_state == HIGH && _led6_state == HIGH) {
       pulsing_led[5] = HIGH;
   } else if (led6_state == LOW && _led6_state == HIGH) {
       pulse(led6, HIGH);
       pulsing_led[5] = LOW;
   } else if (led6_state == LOW && _led6_state == LOW) {
       pulse(led6, LOW);
       pulsing_led[5] = LOW;
   } else {
       pulsing_led[5] = LOW;
       pulse(led6, LOW);
   }

   pulsing();

}



void led_state() {

    led_states = "";
    led_states = ","+ String(led1_state) +","+ String(led2_state)+","+ String(led3_state)+","+String(led4_state) +","+ String(led5_state) +","+String(led6_state);

    if (previous_state != led_states) {
        Particle.publish("sensor_emilia_1", led_states);
        previous_state = led_states;
    }
}

// int checkBrightness (String _brightness) {

//     if (_brightness != NULL) {
//         int num = _brightness.toInt();

//         brightness = num;

//         return 1;
//     } else {
//         return -1;
//     }
// }




int listenMode (String listening) {
        if (listening == "ON" ) {
            WiFi.listen();
            return 1;
        } else {
          return -1;
        }
}

int which_ssid (String whatever) {
    String wifi_ssid = WiFi.SSID();
    Particle.publish("status_emilia_1", "Connected to:"+wifi_ssid);
    return 1;
}




int connectTo(String data) {

    String SSID, PASSWORD;

    String info = data;

    int commaIndex = info.indexOf(',');

    //  Search for the next comma just after the first
    SSID = info.substring(0, commaIndex);
    PASSWORD = info.substring(commaIndex+1, ',');

    WiFi.setCredentials(SSID, PASSWORD);
    WiFi.connect();

    return 1;
}


int getAllSSID(String data) {
    WiFiAccessPoint ap[6];
    int found = WiFi.getCredentials(ap, 6);

    for (int i = 0; i < found; i++) {
        String t = (ap[i].ssid);
        Particle.publish("status_emilia_1", "SSID:"+t);
    }
    return 1;
}
