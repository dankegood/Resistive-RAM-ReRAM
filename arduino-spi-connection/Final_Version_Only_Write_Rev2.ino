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

	Serial.println("How many byte do you want to input? ");

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

	Serial.println("***** Start Writing on ReRAM *****");

	userInputStart();
}

void writeEnable(void)
{
	//byte result;

	// Set SS as LOW to be activated
	digitalWrite(SLAVESELECT, LOW);
	delay(10); // Originally 3000ms == 3s

	// Send Write Enable Latch command
	//result = SPI.transfer(WREN);
	SPI.transfer(WREN);
	delay(100);

	// Print to check whether the input value is properly stored
	//Serial.print("Got back from write enable command: ");
	//Serial.println(result);

	// Set SS as HIGH to be deactivated
	digitalWrite(SLAVESELECT, HIGH);
	delay(10); // Originally 3000ms == 3s

	// Set SS as LOW to be activated
	digitalWrite(SLAVESELECT, LOW);
	delay(10); // Originally 3000ms == 3s

	// Send Write Memory Code command
	//result = SPI.transfer(WRITE);
	SPI.transfer(WRITE);
	delay(100);

	// Print to check whether the input value is properly stored
	//Serial.print("Got back from write command: ");
	//Serial.println(result);
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

void loop(void)
{
	uint8_t * tx_buffer = (uint8_t *)malloc(sizeof(uint8_t) * buffer_Size);

	Serial.println("***** <TEST> *****");
	Serial.println();
	Serial.println("=== Generated Number ===");
	Serial.println();

	// Send data if the number_Size is less than buffer_Size
	if (number_Size < buffer_Size)
	{
		for (int i = 0; i < number_Size; i++)
		{
			tx_buffer[i] = random(255);
			Serial.print(tx_buffer[i]);
			if ((i % 8) != 7)
				Serial.print('\t');
			if ((i % 8) == 7)
				Serial.println();
		}

		if ((number_Size % 8) != 0)
			Serial.println();
		Serial.println();

		writeEnable();
		selectAddress(address);

		for (int i = 0; i < number_Size; i++)
			SPI.transfer(tx_buffer[i]);

		writeReadDisable();
	}

	// Send data if the number_Size is greater than or equal to buffer_Size
	else
	{
		int quotient = number_Size / buffer_Size;
		int remainder = number_Size % buffer_Size;

		for (int i = 0; i < quotient; i++)
		{
			for (int j = 0; j < buffer_Size; j++)
			{
				tx_buffer[j] = random(255);
				Serial.print(tx_buffer[j]);
				if ((j % 8) != 7)
					Serial.print('\t');
				if ((j % 8) == 7)
					Serial.println();
			}

			writeEnable();
			selectAddress(address + buffer_Size * i);

			for (int i = 0; i < buffer_Size; i++)
				SPI.transfer(tx_buffer[i]);

			writeReadDisable();
		}

		if (remainder != 0)
		{
			for (int i = 0; i < remainder; i++)
			{
				tx_buffer[i] = random(255);
				Serial.print(tx_buffer[i]);
				if ((i % 8) != 7)
					Serial.print('\t');
				if ((i % 8) == 7)
					Serial.println();
			}

			if ((remainder % 8) != 0)
				Serial.println();
			Serial.println();

			writeEnable();
			selectAddress(address + buffer_Size * quotient);

			for (int i = 0; i < remainder; i++)
				SPI.transfer(tx_buffer[i]);

			writeReadDisable();
		}
	}

	free(tx_buffer);

	Serial.println("*** Finish Writing ***");
	while (1) {};
}