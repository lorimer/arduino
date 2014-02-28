#include <idDHT11.h>

// 
//   FILE:  dht11_test1.pde
// PURPOSE: DHT11 library test sketch for Arduino
//

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}

// fast integer version with rounding
//int Celcius2Fahrenheit(int celcius)
//{
//  return (celsius * 18 + 5)/10 + 32;
//}


//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
	return celsius + 273.15;
}

// dewPoint function NOAA
// reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
// reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm
//
double dewPoint(double celsius, double humidity)
{
	// (1) Saturation Vapor Pressure = ESGG(T)
	double RATIO = 373.15 / (273.15 + celsius);
	double RHS = -7.90298 * (RATIO - 1);
	RHS += 5.02808 * log10(RATIO);
	RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
	RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
	RHS += log10(1013.246);

        // factor -3 is to adjust units - Vapor Pressure SVP * humidity
	double VP = pow(10, RHS - 3) * humidity;

        // (2) DEWPOINT = F(Vapor Pressure)
	double T = log(VP/0.61078);   // temp var
	return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
	double a = 17.271;
	double b = 237.7;
	double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
	double Td = (b * temp) / (a - temp);
	return Td;
}


#include <idDHT11.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define trigPin 13
#define echoPin 12

int idDHT11pin = 2; //Digital pin for comunications
int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)

//declaration
void dht11_wrapper(); // must be declared before the lib initialization
// This wrapper is in charge of calling


// Lib instantiate
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);


#define DHT11PIN 2

void setup()
{
 Serial.begin (9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 lcd.init();                      // initialize the lcd 
 lcd.backlight();
}
// mus be defined like this for the lib work
void dht11_wrapper() {
  DHT11.isrCallback();
}

void loop()
{
  // Print a message to the LCD.
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Read sensor: "); 

  DHT11.acquire();
  while (DHT11.acquiring())
    ;
  int result = DHT11.getStatus();
  switch (result)
  {
    case IDDHTLIB_OK:
		lcd.print("OK"); 
		break;
    case IDDHTLIB_ERROR_CHECKSUM:
  		lcd.print("Checksum error"); 
		break;
    case IDDHTLIB_ERROR_ISR_TIMEOUT:
		lcd.print("ISR Time out error"); 
		break;
    case IDDHTLIB_ERROR_RESPONSE_TIMEOUT:
		lcd.print("Response Time out error"); 
		break;
    case IDDHTLIB_ERROR_DATA_TIMEOUT:
		lcd.print("Data Time out error"); 
		break;
    case IDDHTLIB_ERROR_ACQUIRING:
		lcd.print("Error Acquiring"); 
		break;
    case IDDHTLIB_ERROR_DELTA:
		lcd.print("Delta time too small"); 
		break;
    case IDDHTLIB_ERROR_NOTSTARTED:
		lcd.print("Error.  Not started."); 
		break;
    default: 
		lcd.print("Unknown error"); 
		break;
  }
  // Print a message to the LCD.
  lcd.clear();
  lcd.setCursor(0,1); 
  lcd.print("Humidity (%): "); 
  lcd.print(DHT11.getHumidity());
  delay(500);
  lcd.setCursor(0,1); 
  lcd.print("                 "); 

  lcd.print("Temperature (°C): ");
  lcd.print(DHT11.getCelsius());
  delay(500);
  lcd.setCursor(0,1); 
  lcd.print("                 "); 

  lcd.print("Temperature (°F): ");
  lcd.print(DHT11.getFahrenheit());
  delay(500);
  lcd.setCursor(0,1); 
  lcd.print("                 "); 

  lcd.print("Temperature (°K): ");
  lcd.print(DHT11.getKelvin());
  delay(500);
  lcd.setCursor(0,1); 
  lcd.print("                 "); 

  lcd.print("Dew Point (°C): ");
  lcd.print(DHT11.getDewPointSlow());
  delay(500);
  lcd.setCursor(0,1); 
  lcd.print("                 "); 

  lcd.print("Dew PointFast (°C): ");
  lcd.print(DHT11.getDewPoint());
  delay(500);
  lcd.setCursor(0,1); 
  lcd.print("                 "); 

  delay(2000);
}
//
// END OF FILE
//

