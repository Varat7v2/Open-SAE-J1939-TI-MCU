/*
 * Main.c
 *
 *  Created on: 16 juli 2021
 *      Author: Daniel Mårtensson
 */

#include "stdlib.h"
#include "stdio.h"

#include "ISO 11783/ISO 11783-7 Application Layer/ISO_11783-7_Application_Layer.h"
#include "Open SAE J1939/Open_SAE_J1939.h"
#include "SAE J1939-71 Application Layer/SAE_J1939-71_Application_Layer.h"
#include "SAE J1939-73 Diagnostics Layer/SAE_J1939-73_Diagnostics_Layer.h"
#include "SAE J1939-81 Network Management Layer/SAE_J1939-81_Network_Management_Layer.h"

int main() {

	/* Create our J1939 structure with two ECU */
	J1939 j1939_1 = {0};
	J1939 j1939_2 = {0};

	/* Set the ECU address */
	j1939_1.this_ECU_address = 0xA;												/* From 0 to 253 because 254 = error address and 255 = broadcast address */
	j1939_2.this_ECU_address = 0x90;

	/* Set the Software Identification */
	j1939_1.this_software_identification.number_of_fields = 15;
	char text[15] = "SAE J1939!!!";
	for(uint8_t i = 0; i < 15; i++)
		j1939_1.this_software_identification.identifications[i] = (uint8_t) text[i];

	/* Request Software Identification from ECU 2 to ECU 1 */
	SAE_J1939_Send_Request(&j1939_2, 0xA, PGN_SOFTWARE_IDENTIFICATION);

	/* Response request from ECU 1 perspective */
	Open_SAE_J1939_Listen_For_Messages(&j1939_1);

	/* Read response request from ECU 1 to ECU 2 */
	for(uint8_t i = 0; i < 5; i++)
		Open_SAE_J1939_Listen_For_Messages(&j1939_2);

	/* Display what ECU 2 got */
	printf("Number of fields = %i\nIdentifications = %s\nFrom ECU address = 0x%x", j1939_2.from_other_ecu_software_identification.number_of_fields, j1939_2.from_other_ecu_software_identification.identifications, j1939_2.from_other_ecu_software_identification.from_ecu_address);

	return 0;
}
