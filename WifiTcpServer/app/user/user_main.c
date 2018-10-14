#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "driver/gpio16.h"
#include "gpio.h"
#include "driver/uart.h"
#include "user_interface.h"
#include "spi_flash.h"
#include "espconn.h"
#include "user_config.h"
#include "smartconfig.h"


void user_init()
{
	////TCP_ip = 192.168.4.1
	uart_init( BIT_RATE_115200, BIT_RATE_115200 );//UART 波特率初始化
	tcp_service_init();
}
