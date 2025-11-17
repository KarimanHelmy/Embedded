// Sends a CAN message using one of the three transmit buffers in a round-robin manner.
void can_message_send(can_message* message){
	static int buffer_number = 0;
	
	buffer_number +=1;
	if (buffer_number > 2) {
		buffer_number = 0;
	}
	
	while (!can_transmit_complete(buffer_number)){
		buffer_number +=1;
		if (buffer_number > 2) {
			buffer_number = 0;
		}
	}
	
	// Standard CAN identifier field (ID)
	unsigned int ID = message->id;
	char id_high = ID / 8;
	char id_low = ID % 8;
	id_low = id_low*0b100000;
	mcp_2515_write(MCP_TXB0SIDH + 16 * buffer_number, id_high);
	mcp_2515_write(MCP_TXB0SIDL + 16 * buffer_number, id_low);
	
	// Define data length of message
	uint8_t data_length = message->length;
	mcp_2515_write(MCP_TXB0DLC + 16 * buffer_number, data_length);
	
	// Write data bytes to transmit buffer
	char* data_bytes = message->data;
	for (uint8_t byte = 0; byte < data_length; byte++) {
		mcp_2515_write(MCP_TXB0Dm + byte + 16 * buffer_number, data_bytes[byte]);
	}
	
	// Request to send message, send if successful
	mcp_2515_request_to_send(MCP_RTS_TX0 + buffer_number);
}
