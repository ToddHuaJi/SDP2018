/*
  simple test of UART interfaces
 */

#include <AP_HAL/AP_HAL.h>
#include <AP_HAL_Linux/AP_HAL_Linux.h>
#include <AP_HAL_Linux/UARTDriver.h>
#include <AP_SerialManager/AP_SerialManager.h>
#include <AP_HAL_Linux/HAL_Linux_Class.h>
#include <AP_HAL_Linux/UARTDevice.h>
#if HAL_OS_POSIX_IO
#include <stdio.h>

#endif


void setup();
void loop();


const HAL_Linux::HAL& hal = AP_HAL::get_HAL();
// const AP_HAL::HAL &hal = HAL_Linux::g
UARTDevice* uartL;
Linux::UARTDriver *uartDriverTest;

/*
  setup one UART at 57600
 */
static void setup_uart(AP_HAL::UARTDriver *uart, const char *name)
{
    if (uart == nullptr) {
         hal.console->println(name);
        // that UART doesn't exist on this platform
        return;
    }
    
    uart->begin(115200, 0, 0);
    uart->printf("Hello on UART %s \n",
                 name );

    
}


void setup(void)
{
    /*
      start all UARTs at 115200 with default buffer sizes
    */
    hal.scheduler->delay(5000); //Ensure that the uartA can be initialized
    
    setup_uart(hal.uartA, "uartA");  // console
    setup_uart(hal.uartB, "uartB");  // 1st GPS
    setup_uart(hal.uartC, "uartC");  // telemetry 1
    setup_uart(hal.uartD, "uartD");  // telemetry 2
    setup_uart(hal.uartE, "uartE");  // 2nd GPS
    
    uartL = new UARTDevice("/dev/ttyAMA0");
    // uartL->UARTDevice("/dev/ttyAMA0");
    uartL->set_speed((uint32_t)115200);
    uartL->open();
    uartL->set_blocking(true);

    // uartDriverTest = Linux::UARTDriver::from(hal.uartA);
    // uartDriverTest = new Linux::UARTDriver(true);
    // uartDriverTest->set_device_path("/dev/ttyAMA0");
    // uartDriverTest->begin(115200);
    // uartL = new Linux::UARTDriver(true);
    // uartL->set_device_path("/dev/ttyAMA0");
    // uartL->begin(115200);
}

static void test_uart(AP_HAL::UARTDriver *uart, const char *name)
{
    
    uart->printf("Hello on UART %s at %.3f seconds\n",
                 name, (double)(AP_HAL::millis() * 0.001f));
    
    
    // // char readout = 'a';
    uint8_t* output = new uint8_t[10];
    
    uartL->read(output,(uint16_t)9);
    // uint32_t available = uartDriverTest->available();
    // uartL->
    
    uart->printf("Header Byte 1:                    0x%x\n", output[0]);
    uart->printf("Header Byte 2:                    0x%x\n", output[1]);
    uart->printf("Distance lower Byte:              0x%x\n", output[2]);
    uart->printf("Distance higher Byte:             0x%x\n", output[3]);
    uart->printf("Low Signal Strength Byte:         0x%x\n", output[4]);
    uart->printf("High Signal Strength Byte:        0x%x\n", output[5]);
    uart->printf("Reservered Byte:                  0x%x\n", output[6]);
    uart->printf("Original signal quality degree:   0x%x\n", output[7]);
    uart->printf("Checksum:                         0x%x\n", output[8]);
    // uart->printf("Read out on UARTDrive at %x\n", available);
    // uart->printf("Read out on UART %s \n",uartDriverTest->get_device_path());


}

void loop(void)
{
    test_uart(hal.uartA, "uartA");
    test_uart(hal.uartB, "uartB");
    test_uart(hal.uartC, "uartC");
    test_uart(hal.uartD, "uartD");
    test_uart(hal.uartE, "uartE");

    // also do a raw printf() on some platforms, which prints to the
    // debug console
#if HAL_OS_POSIX_IO
    ::printf("Hello on debug console at %.3f seconds\n", (double)(AP_HAL::millis() * 0.001f));
#endif

// #if  CONFIG_HAL_BOARD_SUBTYPE == HAL_BOARD_SUBTYPE_LINUX_NAVIO2
//     hal.uartA->set_device_path("/dev/ttyAMA0");
// #endif

    hal.scheduler->delay(1000);



}

AP_HAL_MAIN();
