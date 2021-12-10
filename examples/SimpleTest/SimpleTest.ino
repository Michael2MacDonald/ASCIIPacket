#include "ASCIIPacket.h"

ASCIIPacket SerialPackets();
String Header;
String Text;

void setup() {
	Serial.begin(9600);
	Serial2.begin(9600);
}

void loop() {

	// Code Here...

	// For every char available from Serial2, run update() to process it
	for (unsigned int i; i<Serial2.available(); i++) {
		SerialPackets.update(Serial2.read()); // Send next char from Serial2 to update()
		// After each update() check if packet has need received
		if (SerialPackets.available()) {
			receivePacket(); // Get packet data and process/save it if needed
		}
	}

	// Code Here...

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