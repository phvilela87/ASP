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
#include <stdio.h>

void usart_read_callback(struct usart_module *const usart_module);
void usart_write_callback(struct usart_module *const usart_module);

void configure_usart(void);
void configure_usart_callbacks(void);

struct usart_module usart_instance;

#define MAX_RX_BUFFER_LENGTH   5

volatile uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];

void usart_read_callback(struct usart_module *const usart_module)
{
	usart_write_buffer_job(&usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
}

void usart_write_callback(struct usart_module *const usart_module)
{
	port_pin_toggle_output_level(LED_0_PIN);
}

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

	while(usart_init(&usart_instance, SERCOM5, &config_usart) != STATUS_OK);

	usart_enable(&usart_instance);
}

void configure_usart_callbacks(void)
{
	usart_register_callback(&usart_instance, usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_instance, usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);

	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
}


int main(void)
{
	uint8_t string[] = "Hello World!\r\n";
	
	system_init();
	configure_usart();
	configure_usart_callbacks();
	system_interrupt_enable_global();

	usart_write_buffer_wait(&usart_instance, string, sizeof(string));

	while(1) 
	{
		usart_read_buffer_job(&usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
	}
}

