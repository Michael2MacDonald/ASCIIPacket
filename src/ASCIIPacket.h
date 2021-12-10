#ifndef _ASCIIPACKET_H /* Start of _YOUR_LIBRARY_H */
#define _ASCIIPACKET_H

#define ASCII_NUL 0x00U // Null
#define ASCII_SOH 0x01U // Start of Header
#define ASCII_STX 0x02U // Start of Text
#define ASCII_ETX 0x03U // End of Text
#define ASCII_EOT 0x04U // End of Transmission
#define ASCII_ENQ 0x05U // Enquiry
#define ASCII_ACK 0x06U // Acknowledge
#define ASCII_BEL 0x07U // Bell
#define ASCII_BS  0x08U // Backspace
#define ASCII_HT  0x09U // Horizontal Tab
#define ASCII_LF  0x0AU // Line Feed
#define ASCII_VT  0x0BU // Vertical Tab
#define ASCII_FF  0x0CU // Form Feed
#define ASCII_CR  0x0DU // Carriage Return
#define ASCII_SO  0x0EU // Shift Out
#define ASCII_SI  0x0FU // Shift In
#define ASCII_DLE 0x10U // Data Link Escape
#define ASCII_DC1 0x11U // Device Control 1
#define ASCII_DC2 0x12U // Device Control 2
#define ASCII_DC3 0x13U // Device Control 3
#define ASCII_DC4 0x14U // Device Control 4
#define ASCII_NAK 0x15U // Negative Acknowledge
#define ASCII_SYN 0x16U // Synchronize
#define ASCII_ETB 0x17U // End of Transmission Block
#define ASCII_CAN 0x18U // Cancel
#define ASCII_EM  0x19U // End of Medium
#define ASCII_SUB 0x1AU // Substitute
#define ASCII_ESC 0x1BU // Escape
#define ASCII_FS  0x1CU // File Separator
#define ASCII_GS  0x1DU // Group Separator
#define ASCII_RS  0x1EU // Record Separator
#define ASCII_US  0x1FU // Unit Separator

#include <Stream.h>

class ASCIIPacket {
	Stream stream; // Stream object to hold

	// For Receiving Packets
	String bufInMsgHdr; // Holds message header from incoming packet
	String bufInMsgTxt; // Holds message text from incoming packet
	// bool _overflow = false;
	bool pktAvail = false;

	// For Transmitting Packets
	// String bufOutMsg; // Holds message for outgoing packet
	// String bufOutPkt; // Holds outgoing packet

	bool receivingPkt = false; // Currently receiving packet
	bool pktHdr = false; // Currently receiving packet header
	bool pktTxt = false; // Currently receiving packet text

public:
	// PacketMessage(uint32_t bufInPktAddr, uint32_t bufInMsgAddr) : bufInPkt((uint32_t *)bufInPktAddr), bufInMsg((uint32_t *)bufInMsgAddr), bufOutMsg((uint32_t *)NULL), bufOutPkt((uint32_t *)NULL)) {}
	// PacketMessage(uint32_t bufInPktAddr, uint32_t bufInMsgAddr, uint32_t bufOutMsgAddr, uint32_t bufOutPktAddr, Stream streamInObj) : bufInPkt((uint32_t *)bufInPktAddr), bufInMsg((uint32_t *)bufInMsgAddr), bufOutMsg((uint32_t *)bufOutMsgAddr), bufOutPkt((uint32_t *)bufOutPktAddr), stream(streamInObj) {}
	ASCIIPacket() : stream(NULL) {}
	ASCIIPacket(Stream streamInObj) : stream(streamInObj) {}

	// Receiving Packets
	void update(char ch);             // Read incoming packet from char
	// void update(String pkt);          // Read incoming packet from string
	void update();                    // Read incoming packet from stream
	// bool overflow();                  // Returns true if incoming packet is larger than buffer
	bool available();                 // returns true if full packet has arrived
	void flush();                     // reset all vars and clear buffers
	void get(uint32_t hdrAddr, uint32_t txtAddr);  // Place incoming packet into provided variables and run flush()
	void peek(uint32_t hdrAddr, uint32_t txtAddr); // Place incoming packet into provided variables

	// Transmitting Packets
	void send(String hdr, String txt); // Create and send a packet from String

	// boolean operator==(boolean) { return this->available(); }
}

void decode(String pkt, uint32_t hdrAddr, uint32_t txtAddr); // Decodes 'pkt' and saves the header and text to the Strings pointed to by hdrAddr and txtAddr
String encode(String header, String text); // 

#endif /* End of _ASCIIPACKET_H */