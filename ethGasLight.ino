#include <stdio.h>
#include <math.h>


// This #include statement was automatically added by the Particle IDE.
int yellowLED = A0;
int redLED = A1;
int greenLED = A2;
int i = 0, val, len;
// long long decimalNum;
unsigned int nextTime = 0;    



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

void myHandler(const char *event, const char *data) {
  // Handle the webhook response
    Particle.publish("Resp", data, 60, PRIVATE);
    // Particle.publish("R event", String(event), 60, PRIVATE);

    
    // char hex[30] = data;

    Particle.publish("result", data, 60, PRIVATE);

    char *hex = "0x107ee13223";
    // strcpy(hex, data);

    // hex = "0x107ee13223";

    unsigned long decimalNum = 0, base = 1;
    int i = 0, length;
    char *hexx = hex+1; // removes first character
    Particle.publish("hexx", hexx, 60, PRIVATE);

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
    // char decimalStr[30];
    // sprintf(decimalStr, "%ld", decimalNum);
    // Particle.publish("Decimal Num", decimalNum, 60, PRIVATE);
    Particle.publish("Decimal Hex Number", String::format("%ld", decimalNum));


    // Particle.publish("Decimal Str", String(decimalStr), 60, PRIVATE);

    if(decimalNum<102278815599){
        redLightOn("1");
    } else if (decimalNum<80278815599) {
        yellowLightOn("1");
    } else if (decimalNum<75848164387) {
        greenLightOn("1");
    }

    // printf("\nHexadecimal number = %s", hex);
    // printf("Decimal number = %ld\n", decimalNum);

}

/* This function is called once at start up ----------------------------------*/
void setup()
{
    Particle.subscribe("hook-response/get-eth-gas-price", myHandler,MY_DEVICES);
    Particle.publish("setup", "SETUPa", 60, PRIVATE);


	//Setup the Tinker application here

	//Register all the Tinker functions
	Particle.function("digitalread", tinkerDigitalRead);
	Particle.function("digitalwrite", tinkerDigitalWrite);
	Particle.function("analogread", tinkerAnalogRead);
	Particle.function("analogwrite", tinkerAnalogWrite);
	
    digitalWrite(yellowLED, HIGH);  
    digitalWrite(greenLED, HIGH);  
    digitalWrite(redLED, HIGH); 
	Particle.function("yellowLightOn",yellowLightOn);
	Particle.function("redLightOn",redLightOn);
	Particle.function("greenLightOn",greenLightOn);
    

}



/* This function loops forever --------------------------------------------*/
void loop()
{
    String data = String(10);

    // Particle.publish("GetEthGasPrice", data, PRIVATE);

    if (nextTime > millis()) {
        return;
    }

    Particle.publish("get-eth-gas-price", data, PRIVATE);

    nextTime = millis() + 60*1000;

    //This will run in a loop
}


int yellowLightOn(String command){
    digitalWrite(yellowLED, LOW);  
    digitalWrite(greenLED, HIGH);  
    digitalWrite(redLED, HIGH);  
    return 1;
}

int redLightOn(String command){
    digitalWrite(yellowLED, HIGH);  
    digitalWrite(greenLED, HIGH);  
    digitalWrite(redLED, LOW);  
    return 1;
}


int greenLightOn(String command){
    digitalWrite(yellowLED, HIGH);  
    delay(500);
    digitalWrite(greenLED, LOW);  
    digitalWrite(redLED, HIGH);  
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
