int yellowLED = A0;
int redLED = A1;
int greenLED = A2;


void setup() {
    digitalWrite(yellowLED, HIGH);    

}

void loop() {

}

int yellowLightOn(String command){
    digitalWrite(yellowLED, LOW);  
    return 1;
}

int yellowLightOff(String command){
    digitalWrite(yellowLED, HIGH);    
    return 1;
}

int redLightOn(String command){
    digitalWrite(redLED, LOW);    
    return 1;
}

int redLightOff(String command){
    digitalWrite(redLED, HIGH);    
    return 1;
}

int greenLightOn(String command){
    digitalWrite(greenLED, LOW);    
    return 1;
}

int greenLightOff(String command){
    digitalWrite(greenLED, HIGH);    
    return 1;
}