#include "./pulse.h"

void setup() {

  // Define sending Pin 
  pinMode(13, OUTPUT);

  Serial.begin(9600);
  delay(1000);
  
  Serial.print("Pulses: ");
    for (int j = 0; j < (sizeof(pulses)/sizeof(pulses[0])) ; j++) {
      Serial.print(pulses[j]);
      Serial.print(", ");
    }
    Serial.println();
}

void sendData(){
		bool onoff = false;
		for(int i = 0; i < (sizeof(pulses)/sizeof(pulses[0])); i++){
				digitalWrite(13,!onoff);
				delayMicroseconds(pulses[i]);
				onoff = !onoff;
		}
		digitalWrite(13,LOW);
		Serial.println("Seding: DONE!");
}

void loop() {
		if(Serial.available() > 0){
			String data = Serial.readString();
			if(data.length() > 0){
					if(data == String("send")){
						Serial.println("Sending data");
						sendData();
				
		}else{
		  Serial.println("Unknown command");
		}
     }
}
// sendData();
// delay(1000);
}
