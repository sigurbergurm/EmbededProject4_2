#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define HELLO_MAJOR 0
#define DEVICE_NAME "hello1"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sigurbergur Magnusson");
MODULE_DESCRIPTION("A simple Linux LKM that accepts a single parameter.");
MODULE_VERSION("0.1");

static int majorNumber;
static int numberOpens = 0;
static char *name = "world";

module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

// static int hello_open(struct inode *, struct file *);
// static int hello_release(struct inode *, struct file *);
static ssize_t hello_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
    // .open = hello_open,
    .write = hello_write,
    // .release = hello_release,
};

static ssize_t hello_write(struct file * file, const char *buf, size_t count, loff_t *ppos)
{
    printk("hello write: acceptin %zu bytes from the user \n", count);
    return count;
}

static int __init hello_init(void) {

    printk(KERN_INFO "dev: Hello %s from the RPi LKM!\n", name);

    majorNumber = register_chrdev(HELLO_MAJOR, DEVICE_NAME, &fops);

    if (majorNumber<0){
        printk(KERN_ALERT "hello failed to register a major number\n");
        return majorNumber;
    }

    printk(KERN_INFO "hello: registered correctly with major number %d\n", majorNumber);
    return 0;
}

static void __exit hello_exit(void) {
    numberOpens++;
    printk(KERN_INFO "hello: Device has been opened %d time(s)\n", numberOpens);
    return 0;
}

module_init(hello_init);
module_exit(hello_exit);

//*************RESULTS

//sudo mknod /dev/hello1 c 238 0 <- to make the device node with major number 238 and minor 0.

//Runing:   echo "Open the pod bay doors, HAL." > /dev/hello1
//yields:   Oct  8 16:37:11 sigurbergur kernel: [  859.511642] hello write: acceptin 29 bytes from the user 