#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sigurbergur Magnusson");
MODULE_DESCRIPTION("A simple Linux LKM that accepts a single parameter.");
MODULE_VERSION("0.1");

static char *name = "world";

module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

static int __init hello_init(void) {
    printk(KERN_INFO "dev: Hello %s from the RPi LKM!\n", name);
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "dev: Goodbye %s from the RPi LKM!\n", name);

}

module_init(hello_init);
module_exit(hello_exit);


/*MODINFO OUTPUT
pi@sigurbergur:~/Embeded/Assignment4_2/Part1 $ modinfo hello.ko
filename:       /home/pi/Embeded/Assignment4_2/Part1/hello.ko
version:        0.1
description:    A simple Linux LKM that accepts a single parameter.
author:         Sigurbergur Magnusson
license:        GPL
srcversion:     F2BEBCCB490F507DB4AAA81
depends:
name:           hello
vermagic:       5.4.51+ mod_unload modversions ARMv6 p2v8
parm:           name:The name to display in /var/log/kern.log (charp)

kernal log:
Oct  8 21:48:01 sigurbergur kernel: [  978.219751] dev: Hello world from the RPi LKM!
Oct  8 22:16:46 sigurbergur kernel: [ 1503.916551] dev: Goodbye world from the RPi LKM!
Oct  8 23:17:03 sigurbergur kernel: [ 1520.400242] dev: Hello Sigurbergur from the RPi LKM!

Lokking inside the parameters folder in the /sys/module/hello there is a file "name" there.
if we call the hello.ko is called with name = sigurbergur the name parameter in there changes
otherwise it is written out as world as defined in line 12
*/