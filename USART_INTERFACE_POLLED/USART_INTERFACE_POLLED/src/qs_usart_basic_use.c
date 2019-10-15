/**
 * \file
 *
 * \brief SAM USART Quick Start
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>

void configure_usart(void);

struct usart_module usart_instance;

void configure_usart(void)
{
	struct usart_config config_usart;

	usart_get_config_defaults(&config_usart);

	/* Configures PB22 and PB23 as usart pins on SAMD20J18 */
	config_usart.baudrate    = 9600;
	config_usart.mux_setting = USART_RX_3_TX_2_XCK_3;
	config_usart.pinmux_pad0 = PINMUX_UNUSED;
	config_usart.pinmux_pad1 = PINMUX_UNUSED;
	config_usart.pinmux_pad2 = PINMUX_PB22D_SERCOM5_PAD2;
	config_usart.pinmux_pad3 = PINMUX_PB23D_SERCOM5_PAD3;
	
	/* Set this configuration if you want to test the code on SAMD20E15 pins PA06 and PA07 */
	//config_usart.mux_setting = USART_RX_1_TX_2_XCK_3;
	//config_usart.pinmux_pad0 = PINMUX_UNUSED;
	//config_usart.pinmux_pad1 = PINMUX_UNUSED;
	//config_usart.pinmux_pad2 = PINMUX_PA06D_SERCOM0_PAD2;
	//config_usart.pinmux_pad3 = PINMUX_PA07D_SERCOM0_PAD3;

	while (usart_init(&usart_instance, SERCOM5, &config_usart) != STATUS_OK);
	
	/* Set this configuration if you want to test the code on SAMD20E15 pins PA06 and PA07 */
	//while(usart_init(&usart_instance, SERCOM0, &config_usart) != STATUS_OK);

	usart_enable(&usart_instance);
}

int main(void)
{
	uint8_t string[] = "Hello World!\r\n";
	uint16_t temp;
	
	system_init();
	configure_usart();

	usart_write_buffer_wait(&usart_instance, string, sizeof(string));

	while(1) 
	{
		if(usart_read_wait(&usart_instance, &temp) == STATUS_OK) 
		{
			while(usart_write_wait(&usart_instance, temp) != STATUS_OK);
		}
	}
}
