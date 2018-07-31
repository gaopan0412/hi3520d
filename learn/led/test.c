/*
* file test.c - for test ioctl
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define GPIO_SET_PINMUX                 0x1100
#define GPIO_SET_DATA_OUTPUT    0x1101
#define GPIO_READ_DATA_INPUT    0x1102
#define GPIO_SET_DATA_DIRECT    0x1103


//typedef struct
struct gpio_params
{
      unsigned int group;
      unsigned int pin;
      unsigned int direct;
      unsigned int value;
};
//} gpio_params;

/*
ETH_LED        GPIO_0_4
CAN_LED        GPIO_0_1
CHK_LED        GPIO_0_7
ETH_DOWN_LED   GPIO_0_3
*/

int main(void)
{
    int fd = open("/dev/gpio_test", "rw");
    struct gpio_params gpio;
    struct gpio_params led;

    //beep gpio_8_0
    gpio.group = 8;
    gpio.pin = 0;
    gpio.value = 1;
    gpio.direct=0;   


    //led
    led.group = 0;
    led.pin   = 1;
    led.value = 1;
    led.direct= 0;
 
    if(fd < 0)
    {
    printf("error: open fail\n");
    }

    printf("open success fd:%d\n", fd);
    
    //beep
    ioctl(fd, GPIO_SET_DATA_DIRECT, &gpio);
    ioctl(fd, GPIO_SET_DATA_OUTPUT, &gpio);
 
    //led   
    ioctl(fd, GPIO_SET_DATA_DIRECT, &led);
    ioctl(fd, GPIO_SET_DATA_OUTPUT, &led);

    while(1)
    {
    gpio.value = 0; 
    led.value  = 1;
    printf("value:%d\n", gpio.value); 
    ioctl(fd, GPIO_SET_DATA_OUTPUT, &gpio); 
    ioctl(fd, GPIO_SET_DATA_OUTPUT, &led);
    sleep(1);


    gpio.value = 1;
    led.value = 0;
    printf("value:%d\n", gpio.value); 
    ioctl(fd, GPIO_SET_DATA_OUTPUT, &gpio);
    ioctl(fd, GPIO_SET_DATA_OUTPUT, &led);
    sleep(1);

    }


    close(fd);
    return 0;

}
