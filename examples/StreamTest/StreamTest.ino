/**
 * ASCIIPacket test with Steam
 * 
 * Output of this sketch:
 * 	Once on startup:
 * 	- Sends a packet with the header and text, "My Header 1" and "My Text 1" over Serial2
 * 	- Sends a packet with the header and text, "My Header 2" and "My Text 2" over Serial2
 * 	- Sends a packet with the header and text, "My Header 3" and "My Text 3" over Serial2
 * 	Loops forever:
 * 	- Prints message to the arduino serial monitor (Serial) whenever it receives a packet over Serial2
 * 
 */

#include "PacketMessage.h"
/**
 * All ASCII Character Definitions Are Provided, Including:
 * 	#define ASCII_SOH 0x01U // Start of Header
 * 	#define ASCII_STX 0x02U // Start of Text
 * 	#define ASCII_ETX 0x03U // End of Text
 * 	#define ASCII_EOT 0x04U // End of Transmission
 */

ASCIIPacket SerialPackets(Serial2);
String Header;
String Text;

String SendPacket = ASCII_SOH + "My Header 1" + ASCII_STX + "My Text 1" + ASCII_ETX + ASCII_EOT;
String SendHeader = "My Header 2";
String SendText = "My Text 1";

void setup() {
	Serial.begin(9600);
	Serial2.begin(9600);

	// Send a premade packet through Serial2 manually
	for (unsigned int i = 0; i < SendPacket.length(); i++) {
		Serial2.write(SendPacket.charAt(i));
	}

	// Encode a new packet and send it through Serial2 manually
	SendPacket = SerialPackets.encode(SendHeader, SendText);
	for (unsigned int i = 0; i < SendPacket.length(); i++) {
		Serial2.write(SendPacket.charAt(i));
	}

	// Send a packet through Serial2 using send()
	SendHeader = "My Header 3";
	SendText = "My Text 3";
	SerialPackets.send(SendHeader, SendText);
}

void loop() {

	// Code Here...

	loopPackets(); // Run update() and process new packets.

	// Code Here...

}

void loopPackets() {
	// For every char available from Stream, run update() to process it
	for (unsigned int i; i<Serial2.available(); i++) {
		SerialPackets.update(); // Run update() to fetch char from Stream
		// After each update() check if packet has need received
		if (SerialPackets.available()) {
			receivePacket(); // Get packet data and process/save it if needed
		}
	}
}

void receivePacket() {
	SerialPackets.get(&Header, &Text); // Get packet data
	// Note:
	// get() Will overwrite the data in the variables you give it so
	// make sure you process/save/use the data from the previous
	// packet before getting the next packet. Or use different variables
	// for each get() like an array of strings you increment through
	Serial.println("A New Packet Has Arrived!");
	Serial.println("Packet Header:");
	Serial.println(Header);
	Serial.println("Packet Text");
	Serial.println(Text);
}