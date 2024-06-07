/*
 * Written by MIN SOO HAN, Junior Development Engineer
 * at UCLA CMOS LAB at UCLA Electrical Engineering Department.
 * Unauthorized use of the following code is strictly prohibited.
 */

#include <SPI.h>

 // Define pin with macro
#define DATAOUT 11  //MOSI
#define DATAIN  12  //MISO 
#define SPICLOCK  13  //sck
#define SLAVESELECT 10  //ss

// Op_Code for ReRAM
#define WREN  6
#define WRDI  4
#define RDSR  5
#define WRSR  1
#define READ  3
#define WRITE 2

int address = 0;
unsigned long number_Size = 0;
int buffer_Size = 256;

void userInputStart(void)
{
	byte junk = ' ';

	Serial.println("How many byte do you want to read? ");

	while (Serial.available() == 0);
	{
		number_Size = Serial.parseInt();

		while (number_Size < 1)
		{
			Serial.println("*** Please enter number that is greater than 0: ");

			while (Serial.available() > 0)
				junk = Serial.read();

			while (Serial.available() == 0);
			number_Size = Serial.parseInt();
		}

		while (Serial.available() > 0)
			junk = Serial.read();
	}

	Serial.print("Total number of byte: ");
	Serial.println(number_Size);
	Serial.println();
}

void setup(void)
{
	// Set data rate in bits per second
	Serial.begin(9600);

	// Set mode for each pin either output or input
	pinMode(DATAOUT, OUTPUT);
	pinMode(DATAIN, INPUT);
	pinMode(SPICLOCK, OUTPUT);
	pinMode(SLAVESELECT, OUTPUT);

	// SPI setting with operating frequency and SPI mode
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	SPI.begin();

	// Set SS as HIGH so that there is no false transmission
	digitalWrite(SLAVESELECT, HIGH);
	delay(10); // Originally 3000ms == 3s

	Serial.println("***** Read Testing *****");

	userInputStart();
}

void selectAddress(size_t address)
{
	//byte result;

	// Send MSB first
	SPI.transfer((char)(address >> 16));
	delay(100);
	//Serial.print("Got back from address command 1: ");
	//Serial.println(result);

	// Send next byte
	SPI.transfer((char)(address >> 8));
	delay(100);
	//Serial.print("Got back from address command 2: ");
	//Serial.println(result);

	// Send LSB last
	SPI.transfer((char)(address >> 0));
	delay(100);
	//Serial.print("Got back from address command 3: ");
	//Serial.println(result);
}

void writeReadDisable(void)
{
	digitalWrite(SLAVESELECT, HIGH);
	delay(10); // Originally 3000ms == 3s
}

void readData(void)
{
	//byte result;

	// Set SS as LOW to be activated
	digitalWrite(SLAVESELECT, LOW);
	delay(10); // Originally 3000ms == 3s

	// Send Read Memory Code command
	//result = SPI.transfer(READ);
	SPI.transfer(READ);
	delay(100);

	// Print to check whether the input value is properly stored
	//Serial.print("Got back from read command: ");
	//Serial.println(result);
}

void loop(void)
{
	uint8_t * rx_buffer = (uint8_t *)malloc(sizeof(uint8_t) * buffer_Size);

	Serial.println("***** <TEST> *****");
	Serial.println();

	readData();
	selectAddress(address);

	Serial.println("=== Received data array ===");
	Serial.println();

	for (int i = 0; i < number_Size; i++)
	{
		Serial.print(SPI.transfer(0));
		if ((i % 8) != 7)
			Serial.print('\t');
		if ((i % 8) == 7)
			Serial.println();
	}

	if ((number_Size % 8) != 0)
		Serial.println();
	Serial.println();

	writeReadDisable();

	free(rx_buffer);

	Serial.println("*** Finish Reading ***");
	while (1) {};
}