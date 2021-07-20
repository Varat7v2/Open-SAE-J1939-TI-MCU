/*
 * Main.c
 *
 *  Created on: 16 juli 2021
 *      Author: Daniel Mårtensson
 */

#include "stdlib.h"
#include "stdio.h"

#include "ISO 11783/ISO 11783-7 Application Layer/Application_Layer.h"
#include "Open SAE J1939/Open_SAE_J1939.h"
#include "SAE J1939/SAE J1939-71 Application Layer/Application_Layer.h"
#include "SAE J1939/SAE J1939-73 Diagnostics Layer/Diagnostics_Layer.h"
#include "SAE J1939/SAE J1939-81 Network Management Layer/Network_Management_Layer.h"

int main() {

	/* Create our J1939 structure with two ECU */
	J1939 j1939_1 = {0};
	J1939 j1939_2 = {0};

	/* Set the ECU address */
	j1939_1.this_ECU_address = 0x80;												/* From 0 to 253 because 254 = error address and 255 = broadcast address */
	j1939_2.this_ECU_address = 0x90;

	/* Send an acknowledgement from ECU 1 to ECU 2 where we say that the PGN function is busy because of time out */
	SAE_J1939_Send_Acknowledgement(&j1939_1, 0x90, CONTROL_BYTE_ACKNOWLEDGEMENT_PGN_BUSY, GROUP_FUNCTION_VALUE_ABORT_TIME_OUT, PGN_AUXILIARY_VALVE_ESTIMATED_FLOW_0);

	/* Read the acknowledgement at ECU 2 */
	Open_SAE_J1939_Listen_For_Messages(&j1939_2);

	/* Check what acknowledgement we have */
	printf("Control byte = %i\nGroup function value = %i\nPGN of requested info = %i\nAddress(from where the acknowledgement came from) = 0x%x"
			, j1939_2.from_other_ecu_acknowledgement.control_byte
			, j1939_2.from_other_ecu_acknowledgement.group_function_value
			, j1939_2.from_other_ecu_acknowledgement.PGN_of_requested_info
			, j1939_2.from_other_ecu_acknowledgement.address);

	return 0;
}
