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


int main(void)
{
    int fd = open("/dev/gpio_test", "rw");
    struct gpio_params gpio;
    
    gpio.group = 8;
    gpio.pin = 0;
    gpio.value = 1;
    gpio.direct=0;   
 
    if(fd < 0)
    {
    printf("error: open fail\n");
    }

    printf("open success fd:%d\n", fd);
 
    ioctl(fd, GPIO_SET_PINMUX, &gpio);
    ioctl(fd, GPIO_SET_DATA_DIRECT, &gpio);
    ioctl(fd, GPIO_SET_DATA_OUTPUT, &gpio);

    while(1)
    {
    gpio.value = 0; 
    printf("value:%d\n", gpio.value); 
    ioctl(fd, GPIO_SET_DATA_OUTPUT, &gpio);
     sleep(1);
    gpio.value = 1;
    printf("value:%d\n", gpio.value); 
    ioctl(fd, GPIO_SET_DATA_OUTPUT, &gpio);
    sleep(1);

    }



    close(fd);
    return 0;

}
