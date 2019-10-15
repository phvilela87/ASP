#include <atmel_start.h>

static uint8_t example_USART_0[12] = "Hello World!";

volatile static uint32_t data_arrived = 0;

static void tx_cb_USART_0(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
	gpio_toggle_pin_level(LED0);
}

static void rx_cb_USART_0(const struct usart_async_descriptor *const io_descr)
{
	/* Receive completed */
	data_arrived = 1;
}

static void err_cb_USART_COM(const struct usart_async_descriptor *const io_descr)
{
	/* error handle */
	io_write(&USART_0.io, example_USART_0, 12);
}

int main(void)
{
	uint8_t recv_char;
	
	struct io_descriptor *io;
	
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	usart_async_register_callback(&USART_0, USART_ASYNC_TXC_CB, tx_cb_USART_0);
	usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, rx_cb_USART_0);
	usart_async_register_callback(&USART_0, USART_ASYNC_ERROR_CB, err_cb_USART_COM);
	usart_async_get_io_descriptor(&USART_0, &io);
	usart_async_enable(&USART_0);

	io_write(io, example_USART_0, 12);

	while(1)
	{
		if (data_arrived == 0)
		{
			continue;
		}

		while(io_read(&USART_0.io, &recv_char, 1) == 1)
		{
			while(io_write(&USART_0.io, &recv_char, 1) != 1);
		}
		
		data_arrived = 0;
	}
}
