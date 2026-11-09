# How To Use
## SPI Modification :
assume SPIB is used :  
1- spi initialization :  
~~~    
#define SPIB_BAUD_RATE 8e6  

static void spi_cfg_spib_master_init(void)
    {
    spi_cfg_gpio_spib_init();
    SPI_disableModule(SPIB_BASE);
    SPI_setConfig(SPIB_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA1,
                    SPI_MODE_MASTER, SPIB_BAUD_RATE, 8);
    SPI_setTxFifoTransmitDelay(SPIB_BASE, 0);
    SPI_disableLoopback(SPIB_BASE);
    SPI_setEmulationMode(SPIB_BASE, SPI_EMULATION_FREE_RUN);
    SPI_enableFIFO(SPIB_BASE);
    SPI_clearInterruptStatus(SPIB_BASE, SPI_INT_TXFF);
    SPI_clearInterruptStatus(SPIB_BASE, SPI_INT_RXFF);
    SPI_setFIFOInterruptLevel(SPIB_BASE, SPI_FIFO_TX7, SPI_FIFO_RX7);
    SPI_disableInterrupt(SPIB_BASE, SPI_INT_TXFF);
    SPI_disableInterrupt(SPIB_BASE, SPI_INT_RXFF);
    SPI_enableModule(SPIB_BASE);
     }
~~~
2- add below functions to spi_cfg.c    
~~~
void spi_cfg_cs_control(uint32_t base, bool cs_control)
    {
    switch (base)
        {
        case SPIA_BASE:
        GPIO_writePin(SPIA_STE, cs_control);
        break;
        case SPIB_BASE:

        GPIO_writePin(SPIB_STE, cs_control);

        break;
        case SPIC_BASE:
        GPIO_writePin(SPIC_STE, cs_control);
        break;
        default:
        break;
    }
 }
~~~         
~~~
uint16_t spi_cfg_read(uint32_t base)
    {
    uint16_t data;
    data = SPI_readDataNonBlocking(base);
    return data;
    }
~~~
~~~
void spi_cfg_write(uint32_t base,uint16_t data)
    {
      SPI_writeDataNonBlocking(base, data);
    }
~~~    
## Application Modification
copy files in the app folder (copy content only) and paste in the proper directory
 in your project.(for example make a directory inside source_app folder and name it canfd_app)    
 uncomment function and included headers    
the initialize function needs some data explained below    

1- sys_clk   	    &emsp;&emsp;&emsp; 		the actual cpu clock    
2- rx_gpio_pin      &emsp;&ensp;   		    the MCU pin that is connected to interrupt1 of MCP2518 module    
3- iso_mode 	    &emsp;&emsp;		    canfd ISO standard mode    
4- osc_clk		    &emsp;&emsp;		    input clock of MCP2518 module    
5- spi_read		    &emsp;&emsp;		    mapped spi read function    
6- spi_write 	    &emsp;&emsp;		    mapped spi write function    
7- spi_cs_control   &emsp; 		            mapped spi chip select control function    
8- spi_base		    &emsp;&emsp;		    spi channel used for communication with MCP2518 ( for example SPIB_BASE)    
9- gpio_read_f	    &emsp;		            mapped gpio read function    
## Function Calls
call canfd_app_init in your initialize function.
by including interface folder, you can :    
1- call canfd_interface_receive_message whenever you want to get one received packet.this function returns boolean status.if the status is true, packet is received.    
2- call canfd_interface_send_message to add your packets to tx queue    
3- call canfd_interface_tx_task in your idle task to send queued packets    
## Considerations
1- make sure you pass correct functions(spi and gpio funtions that are declared as function pointers) in canfd_app_init    
2- in canfd_app_init you should pass a pin for rx interrupt(rx_gpio_pin).make sure you configured that pin as input before pass it to the function.
