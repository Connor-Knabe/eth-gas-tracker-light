#include <stdio.h>
#include <math.h>

const pin_t yellowLED = A0;
const pin_t redLED = A1;
const pin_t greenLED = A2;

int i = 0, val, len;
// long long decimalNum;
unsigned int nextTime = 0;   
unsigned int nextTimeNotify = 0;   
unsigned int nextTimeFlash = 0;

int MINS_TILL_CHECK = 10;
int MINS_TILL_NOTIFY = 60;
int MINS_TILL_FLASH = 15;


int RED_FLASH = 350;
int RED = 100;
int YELLOW = 60;
int GREEN = 30;
int GREEN_FLASH = 0;


/* Tinker
 * This is a simple application to read and toggle pins on a Particle device.
 * For the extended version of the Tinker app supporting more pins, see
 * https://github.com/particle-iot/device-os/blob/develop/user/applications/tinker/application.cpp
 */

/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);
int redLightOn();
int yellowLightOn();
int greenLightOn();

void myHandler(const char *event, const char *data) {
  // Handle the webhook response
    char hex[30];
    strcpy(hex, data);
    double decimalNum = 0, base = 1;
    int i = 0, length;
    char *hexx = hex+2; // removes first 2 characters
    length = strlen(hexx);
    for(i = length--; i >= 0; i--)
    {
        if(hexx[i] >= '0' && hexx[i] <= '9')
        {
            decimalNum += (hexx[i] - 48) * base;
            base *= 16;
        }
        else if(hexx[i] >= 'A' && hexx[i] <= 'F')
        {
            decimalNum += (hexx[i] - 55) * base;
            base *= 16;
        }
        else if(hexx[i] >= 'a' && hexx[i] <= 'f')
        {
            decimalNum += (hexx[i] - 87) * base;
            base *= 16;
        }
    }
    char gasPriceStr[30];
    decimalNum = decimalNum/1000000000;
    int gasPrice = (int) decimalNum;
    sprintf(gasPriceStr, "%d", gasPrice);

    if(gasPrice>RED_FLASH){
		nextTimeFlash = millis() + MINS_TILL_FLASH*60*1000;
        redLightOnFlash();
    } else if (gasPrice>RED) {
		nextTimeFlash = millis() + MINS_TILL_FLASH*60*1000;
        redLightOn();
    } else if (gasPrice>YELLOW) {
		nextTimeFlash = millis() + MINS_TILL_FLASH*60*1000;
        yellowLightOn();
    } else if (gasPrice>GREEN) {
		nextTimeFlash = millis() + MINS_TILL_FLASH*60*1000;
        greenLightOn();
		notifyGreen(gasPriceStr);
	} else if (gasPrice>GREEN_FLASH) {
        greenLightOn();
		shouldFlashGreenLight();
		notifyGreen(gasPriceStr);
    }

}

void notifyGreen(char* gasPriceStr){
	if (nextTimeNotify < millis()) {
		Particle.publish("TurnGreenOn", String(gasPriceStr), 60, PRIVATE);
		nextTimeNotify = millis() + MINS_TILL_NOTIFY*60*1000;
	} 
	return; 
}

void shouldFlashGreenLight(){
	if (nextTimeFlash < millis()) {
		greenLightOnFlash();
		nextTimeFlash = 0;
	}
	return;
}

/* This function is called once at start up ----------------------------------*/
void setup()
{
    Particle.subscribe("hook-response/get-eth-gas-price", myHandler,MY_DEVICES);
	//Register all the Tinker functions
	Particle.function("digitalread", tinkerDigitalRead);
	Particle.function("digitalwrite", tinkerDigitalWrite);
	Particle.function("analogread", tinkerAnalogRead);
	Particle.function("analogwrite", tinkerAnalogWrite);
	
   	pinMode(greenLED, OUTPUT);
	pinMode(redLED, OUTPUT);
	pinMode(yellowLED, OUTPUT);

    digitalWrite(yellowLED, HIGH);  
    digitalWrite(greenLED, HIGH);  
    digitalWrite(redLED, HIGH); 
}

/* This function loops forever --------------------------------------------*/
void loop()
{
    if (nextTime > millis()) {
        return;
    }
    Particle.publish("get-eth-gas-price", PRIVATE);
    nextTime = millis() + MINS_TILL_CHECK*60*1000;
}

int yellowLightOn(){
    digitalWrite(yellowLED, LOW);  
    delay(1000);
    digitalWrite(greenLED, HIGH); 
    delay(1000);
    digitalWrite(redLED, HIGH);  
    return 1;
}

int redLightOn(){
    digitalWrite(yellowLED, HIGH);  
    delay(1000);
    digitalWrite(greenLED, HIGH); 
    delay(1000);
    digitalWrite(redLED, LOW);  
    return 1;
}

int redLightOnFlash(){
    digitalWrite(yellowLED, HIGH);  
    delay(1000);
    digitalWrite(greenLED, HIGH); 
    delay(1000);
    digitalWrite(redLED, LOW); 
    delay(1000);
    digitalWrite(redLED, HIGH);  
    delay(3000);
    digitalWrite(redLED, LOW);  
    delay(3000);
    digitalWrite(redLED, HIGH);  
    delay(3000);
    digitalWrite(redLED, LOW);  
    delay(3000);
    digitalWrite(redLED, HIGH);  
    delay(3000);
    digitalWrite(redLED, LOW);  
    delay(3000);
    digitalWrite(redLED, HIGH);  
    delay(3000);
    digitalWrite(redLED, LOW);  
    delay(3000);
    digitalWrite(redLED, HIGH);  
    delay(3000);
    digitalWrite(redLED, LOW);  
 
    return 1;
}

int greenLightOn(){
    digitalWrite(yellowLED, HIGH);  
    delay(1000);
    digitalWrite(greenLED, LOW);  
    delay(1000);
    digitalWrite(redLED, HIGH);  
    return 1;
}

int greenLightOnFlash(){
    digitalWrite(yellowLED, HIGH);  
    delay(1000);
    digitalWrite(greenLED, LOW);  
    delay(1000);
    digitalWrite(redLED, HIGH);  
    delay(1000);
    digitalWrite(greenLED, HIGH);  
    delay(3000);
    digitalWrite(greenLED, LOW);  
    delay(3000);
    digitalWrite(greenLED, HIGH);  
    delay(3000);
    digitalWrite(greenLED, LOW);  
    delay(3000);
    digitalWrite(greenLED, HIGH);  
    delay(3000);
    digitalWrite(greenLED, LOW);  
    delay(3000);
    digitalWrite(greenLED, HIGH);  
    delay(3000);
    digitalWrite(greenLED, LOW);  
    delay(3000);
    digitalWrite(greenLED, HIGH);  
    delay(3000);
    digitalWrite(greenLED, LOW);  

    return 1;
}

/*******************************************************************************
 * Function Name  : tinkerDigitalRead
 * Description    : Reads the digital value of a given pin
 * Input          : Pin
 * Output         : None.
 * Return         : Value of the pin (0 or 1) in INT type
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerDigitalRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT_PULLDOWN);
		return digitalRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT_PULLDOWN);
		return digitalRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerDigitalWrite
 * Description    : Sets the specified pin HIGH or LOW
 * Input          : Pin and value
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerDigitalWrite(String command)
{
	bool value = 0;
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(command.substring(3,7) == "HIGH") value = 1;
	else if(command.substring(3,6) == "LOW") value = 0;
	else return -2;

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, value);
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		digitalWrite(pinNumber+10, value);
		return 1;
	}
	else return -3;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogRead
 * Description    : Reads the analog value of a pin
 * Input          : Pin
 * Output         : None.
 * Return         : Returns the analog value in INT type (0 to 4095)
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerAnalogRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		return -3;
	}
	else if (pin.startsWith("A"))
	{
		return analogRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogWrite
 * Description    : Writes an analog value (PWM) to the specified pin
 * Input          : Pin and Value (0 to 255)
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerAnalogWrite(String command)
{
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	String value = command.substring(3);

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		analogWrite(pinNumber, value.toInt());
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		analogWrite(pinNumber+10, value.toInt());
		return 1;
	}
	else return -2;
}
