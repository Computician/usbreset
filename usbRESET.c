/**
 * The MIT License (MIT)
 *
 * Copyright (c) <year> <copyright holders>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * Very simple usb device reset program. put the vendor and and product hex ids that you gather from lsusb in the strcmp and it will reset that device on debian.
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>
#include <usb.h>
#include <string.h>

int main()
{
        char fileName[21];
        struct usb_bus *bus;
        struct usb_device *dev;
        usb_init();
        usb_find_busses();
        usb_find_devices();
        for (bus = usb_busses; bus; bus = bus->next)
                for (dev = bus->devices; dev; dev = dev->next){
                        char buffer1[5];
                        char buffer2[5];
                sprintf(buffer1, "%04x", dev->descriptor.idVendor);
                sprintf(buffer2, "%04x", dev->descriptor.idProduct);
                if(strcmp(buffer1, "045e") == 0 && strcmp(buffer2, "0772") == 0)
                {
                        sprintf(fileName, "/dev/bus/usb/%s/%s", bus->dirname, dev->filename);
                }
        }
    int fd;
    int rc;

    fd = open(fileName, O_WRONLY);
    if (fd < 0) {
        perror("Error opening output file");
        return 1;
    }

    printf("Resetting USB device %s\n", fileName);
    rc = ioctl(fd, USBDEVFS_RESET, 0);
    if (rc < 0) {
        perror("Error in ioctl");
        return 1;
    }
    printf("Reset successful\n");

    close(fd);
    return 1;
}
