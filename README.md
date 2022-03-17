# ASCIIPacket

ASCIIPacket allows you to encode and decode packets using the ascii control characters. It is great for when you need to send packet messages over serial interfaces like USART/UART.

## Packet Structure

Packets contain two sections of data; the Header and the Text. This can be useful if you need to send multiple types of packets that need to be processed differently for example. In the header you could put information about what data the Text section contains and how to read/process the data.

You can send and receive individual ASCII control characters like `ACK`/`NAK`/`ENQ` using the __INSERT_FUNC__ and __INSERT_FUNC__ functions. This can be useful to add more functionality to your application.

|~~SOH~~|STX|LEN|Command|Value|Length|

**ASCII Control Characters**

These are the ASCII control characters this library makes use of for packets:

|Name|Description|Command|Value|Length|
|----|-----------|-------|-----|------|
|~~Start of Header~~|~~Start Of Packet Header~~|~~SOH `0x01`~~|N/A|N/A|
|Start Of Text|Start Of Packet Data|STX `0x02`|N/A|N/A|
|End Of Text|End Of Packet Data|ETX `0x03`|N/A|N/A|
|End Of Transmission|End Of Packet|EOT `0x04`|N/A|N/A|

## Functions:

`void decode(String pkt, uint32_t hdrAddr, uint32_t txtAddr)` - Decodes `pkt` and saves the decoded Header and Text sections to the Strings pointed to by hdrAddr and txtAddr
`String encode(String header, String text);` - Encodes Strings `header` and `text` into a packet and returns it as a String

**These Functions Are Part Of The `ASCIIPacket` Class:**
`void update(char ch);` - Processes the next char `ch` from the incoming packet
`void update();` - Reads incoming packet from Stream (if provided)
`bool available();` - Returns `true` if a packet has been received
`void flush();` - Clears class buffers and resets class variables
`void get(uint32_t hdrAddr, uint32_t txtAddr);` - Saves the received packet Header and Text sections into the String variables pointed to by `hdrAddr` and `txtAddr`, then runs `flush()`
`void peek(uint32_t hdrAddr, uint32_t txtAddr);` - Saves the received packet Header and Text sections into the String variables pointed to by `hdrAddr` and `txtAddr`
`void send(String hdr, String txt);` - Encodes `hdr` and `txt` into a packet and sends it on the Stream (if provided)
**Creating An Instance Of The `ASCIIPacket` Class:**
```c++
// Create an instance of the ASCIIPacket class for decoding/encoding packets
ASCIIPacket myPacket();

// Create an instance of the ASCIIPacket class for decoding/encoding packets directly from/to a Stream object
ASCIIPacket myPacket(Stream myStream);

// Example of using the Arduino serial port as the stream:
ASCIIPacket myPacket(Serial);
```

## Usage

The Header and Text sections of each packet can be used for any data with some restrictions. The Header and Text sections must only use printable ascii characters (`0x20` through `0x7F`). The two sections are saved as separate Strings so that they can be used independently by the application.

You can decode a packet by providing the class with one char at a time using the `update()` or `update(char ch)` function. As it reads the packet it saves the packet Header and Text sections into the class buffers.

Each instance of the `ASCIIPacket` class can only decode one packet at a time. Once the end of a packet is reached, the class will no longer accept new chars using `update()` or `update(char ch)` until the class buffers are cleared using `flush()`. If you attempt to use these functions without running `flush()` they will immediately return with no error.

You can check if a packet has been received using `available()`. Once a packet has been received, you can get the decoded Header and Text data using `get()` or `peek()`.

You privide `get()` with the memory addresses of two String variables and it will use pointers to save the Header and Text buffers into the variables and then run `flush()` to allow the next packet to be received. `peek()` acts the same way but does not run `flush()` so you will have to run `flush()` or `get()` before you can receive another packet.

By supplying the ASCIIPacket class with an object of the Stream class, you can directly receive and send packets over the Stream using the `update()` and `send()` functions. This is useful for sending/receiving packets over arduino serial for example.

## Example:

Here is a simple example for Arduino using the Stream class:

```c++
#include "PacketMessage.h"

ASCIIPacket SerialPackets(Serial2);
String Header;
String Text;

void setup() {
	Serial.begin(9600);
	Serial2.begin(9600);
}

void loop() {
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
```
