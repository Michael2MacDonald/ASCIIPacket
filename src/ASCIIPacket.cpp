#include "ASCIIPacket.h" // Inlcude library header file
#include <Arduino.h>
#include <Stream.h>


void ASCIIPacket::update(char ch) {
	if ( !receivingPkt && (bufInMsgHdr!='\0'||bufInMsgTxt!='\0') ) { return; } // Must clear Buffers before getting new packet

	switch (ch) {
	case ASCII_SOH: // Start of header
		if (!receivingPkt) {
			receivingPkt = true; // Start receiving packet
			pktHdr = true; // Receiving packet header
			pktTxt = false; // Receiving packet text
			/** TODO: Clear buffers, etc, in case of error with previos packet */
		} else {
			/** TODO: Error handling */
		}
		break;

	case ASCII_STX: // Start of text
		if (receivingPkt && pktHdr) { // Check if we are receiving a packet and packet header currently
			if (!pktTxt) {
				pktHdr = false; // No longer receiving packet header
				pktTxt = true; // Receiving packet text
			} else { // There was a transmition error or the text contains `0x02`
				/** TODO: ??? We are already reading the text so there was a transmition error or the text contains `0x02` */
			}
		} else {
			/** TODO: Error handling */
		}
		break;

	case ASCII_ETX: // End of text
		if (receivingPkt) { // Check if we are receiving a packet
			pktTxt = false; // No longer receiving packet text
			pktHdr = false; // No longer receiving packet header
		} else {
			/** TODO: Error handling */
		}
		break;

	case ASCII_EOT: // End of transmition
		pktAvail = true;
		receivingPkt = false; // No longer receiving packet
		pktTxt = false; // No longer receiving packet text
		pktHdr = false; // No longer receiving packet header
		break;
	
	default:
		if (receivingPkt) {
			if (pktHdr) {
				bufInMsgHdr += ch;
			} else if (pktTxt) {
				bufInMsgTxt += ch;
			}
		}
		break;
	}
	
}

void ASCIIPacket::update() {
	if (stream == NULL) { return; }

	if (stream.available()) {
		int r = stream.read();
		if (r != -1) {
			update(r);
		}
		
	}

	// for (unsigned int i; i<stream.available(); i++) {
	// 	int r = stream.read();
	// 	if (r != -1) {
	// 		update(r);
	// 	}
	// }
}

bool ASCIIPacket::available() {
	return pktAvail;
}

void ASCIIPacket::flush() {
	// bufInPkt = '\0';
	bufInMsgHdr = '\0';
	bufInMsgTxt = '\0';

	// _overflow = false;
	pktAvail = false;

	receivingPkt = false;
	pktHdr = false;
	pktTxt = false;
}

void ASCIIPacket::get(uint32_t hdrAddr, uint32_t txtAddr) {
	String *hdr = hdrAddr;
	String *txt = txtAddr;
	hdr += bufInMsgHdr;
	txt += bufInMsgTxt;
	flush();
	return;
}

void ASCIIPacket::peek(uint32_t hdrAddr, uint32_t txtAddr) {
	String *hdr = hdrAddr;
	String *txt = txtAddr;
	hdr += bufInMsgHdr;
	txt += bufInMsgTxt;
	return;
}

void send(String hdr, String txt) {
	String _pkt = encode(hdr, txt);
	for (unsigned int i = 0; i < _pkt.length(); i++) {
		stream.write(_pkt.charAt(i));
	}
}

// https://www.rapidtables.com/code/text/ascii-table.html

String encode(String hdr, String txt) {

	for (unsigned int i = 0; i < hdr.length(); i++) { // Header must be printable ascii
		if (hdr.charAt(i) <= 0x1FU) {
			return 0;
		}
	}
	for (unsigned int i = 0; i < txt.length(); i++) { // Text must be printable ascii
		if (txt.charAt(i) <= 0x1FU) {
			return 0;
		}
	}

	String result;

	result = ASCII_SOH; // Start of header
	result += hdr; // Header
	result += ASCII_STX; // Start of text
	result += txt; // Text
	result += ASCII_ETX; // End of text
	result += ASCII_EOT; // End of transmition

	return result;
}

// void ASCIIPacket::encode(uint32_t msgAddr, uint32_t pktAddr) {
// 	char* msg = (char *)msgAddr;
// 	String message = msg;

// 	String packet = encode(message);

// 	char* pkt = (char *)pktAddr;
// 	strncpy(pkt, packet, sizeof(pkt) - 1);
// }

void decode(String pkt, uint32_t hdrAddr, uint32_t txtAddr) {
	String *hdr = hdrAddr;
	String *txt = txtAddr;
	
	uint16_t size = pkt.length();

	bool readingTxt = false;
	bool readingHdr = false;

	for (uint16_t i=0; i<size; i++) {
		uint8_t _char = pkt.charAt(i);

		switch (_char) {
			case SOH: // Start of header
				readingHdr = true;
				readingTxt = false;
				break;
			case STX: // Start of text
				readingHdr = false;
				readingTxt = true;
				break;
			case STX: // End of text
				readingHdr = false;
				readingTxt = false;
				break;
			case EOT: // End of transmition
				return;
				break;
			
			default:
				if (readingTxt) {
					txt += _char;
				} else if (readingHdr) {
					hdr += _char;
				}
				break;

		}

	}

	return;
}