/*
 * SPDX-FileCopyrightText: 2020 Efabless Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * SPDX-License-Identifier: Apache-2.0
 */

// This include is relative to $CARAVEL_PATH (see Makefile)
// #include "../verilog/dv/wb_buttons_leds/defs.h"
// #include "stub.c"

/*
	IO Test:
		- Configures MPRJ lower 8-IO pins as outputs
		- Observes counter value through the MPRJ lower 8 IO pins (in the testbench)
*/

// How to make include relative to $CARAVEL_PATH
// #include "verilog/dv/caravel/defs.h"
// #include "verilog/dv/caravel/stub.c"
#include <stdint.h>
#include <stdbool.h>

#define reg_wb_leds      (*(volatile uint32_t*)0x30000000)
#define reg_wb_buttons   (*(volatile uint32_t*)0x30000004)


void main()
{
	/* 
	IO Control Registers
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 3-bits | 1-bit | 1-bit | 1-bit  | 1-bit  | 1-bit | 1-bit   | 1-bit   | 1-bit | 1-bit | 1-bit   |

	Output: 0000_0110_0000_1110  (0x1808) = GPIO_MODE_USER_STD_OUTPUT
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 110    | 0     | 0     | 0      | 0      | 0     | 0       | 1       | 0     | 0     | 0       |
	
	 
	Input: 0000_0001_0000_1111 (0x0402) = GPIO_MODE_USER_STD_INPUT_NOPULL
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 001    | 0     | 0     | 0      | 0      | 0     | 0       | 0       | 0     | 1     | 0       |

	*/

	/* Set up the housekeeping SPI to be connected internally so	*/
	/* that external pin changes don't affect it.			*/

	// reg_spimaster_config = 0xa002;	// Enable, prescaler = 2,
                                        // connect to housekeeping SPI
	(*(volatile uint32_t*)0x24000000) = 0xa002;

	// Connect the housekeeping SPI to the SPI master
	// so that the CSB line is not left floating.  This allows
	// all of the GPIO pins to be used for user functions.

	// Configure lower 8-IOs as user output
	// Observe counter value in the testbench
	(*(volatile uint32_t*)0x26000040) =  0x0402;
	(*(volatile uint32_t*)0x26000044) =  0x0402;
	(*(volatile uint32_t*)0x26000048) =  0x0402;

	(*(volatile uint32_t*)0x2600004c) =  0x1808;
	(*(volatile uint32_t*)0x26000050) =  0x1808;
	(*(volatile uint32_t*)0x26000054) =  0x1808;
	(*(volatile uint32_t*)0x26000058) =  0x1808;
	(*(volatile uint32_t*)0x2600005c) =  0x1808;
	(*(volatile uint32_t*)0x26000060) =  0x1808;
	(*(volatile uint32_t*)0x26000064) =  0x1808;
	(*(volatile uint32_t*)0x26000068) =  0x1808;

	/* Apply configuration */
	(*(volatile uint32_t*)0x26000000) = 1;
	while ((*(volatile uint32_t*)0x26000000) == 1);

    // wait for all 3 buttons to get pressed
    while (reg_wb_buttons != 7);

    // then set all the leds, signalling the end of the test
    reg_wb_leds = 0xFF;

}
