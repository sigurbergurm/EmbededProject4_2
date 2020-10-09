#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define  DEVICE_MAJOR 0         ///< Requested device node major number or 0 for dynamic allocation
#define  DEVICE_NAME "hello1"   ///< In this implementation, the device name has nothing to do with the name of the device in /dev. You must use mknod to create the device node in /dev

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Torfi Thorhallsson");
MODULE_DESCRIPTION("A simple Linux LKM that accepts characters (bytes) from the user.");
MODULE_VERSION("0.1");

static int   majorNumber;        ///< Stores the device number -- determined automatically
static int   numberOpens = 0;    ///< Counts the number of times the device is opened
static char *name = "world";

module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

// The prototype functions for the character driver -- must come before the struct definition

static int     hello_open(struct inode *, struct file *);
static int     hello_release(struct inode *, struct file *);
static ssize_t hello_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = hello_open,
   .write = hello_write,
   .release = hello_release,
};

static ssize_t hello_write(struct file * file, const char *buf, size_t count, loff_t *ppos)
{
   printk("hello write: accepting %zu bytes from the user\n", count);
   return count;
}

static int hello_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "hello: Device successfully closed\n");
   return 0;
}

static int __init hello_init(void) {
   printk(KERN_INFO "hello: Hello %s from the RPi LKM!\n", name);

   majorNumber = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "hello failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "hello: registered correctly with major number %d\n", majorNumber);

   return 0;
}

static void __exit hello_exit(void) {
   unregister_chrdev(majorNumber, DEVICE_NAME);
   printk(KERN_INFO "hello: Goodbye %s from the RPi LKM!\n", name);
}

static int hello_open(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "hello: Device has been opened %d time(s)\n", numberOpens);

   return 0;
}

module_init(hello_init);
module_exit(hello_exit);

//Results from log - main.cpp inside Part3 folder is the application calling this driver, yielding this result.

// Oct  8 17:17:09 sigurbergur kernel: [ 3257.776959] hello: Device has been opened 21 time(s)
// Oct  8 17:17:09 sigurbergur kernel: [ 3257.777037] hello write: accepting 16 bytes from the user
// Oct  8 17:17:09 sigurbergur kernel: [ 3257.777083] hello: Device successfully closed
// Oct  8 17:17:10 sigurbergur kernel: [ 3258.777456] hello: Device has been opened 22 time(s)
// Oct  8 17:17:10 sigurbergur kernel: [ 3258.777508] hello write: accepting 16 bytes from the user
// Oct  8 17:17:10 sigurbergur kernel: [ 3258.777542] hello: Device successfully closed
// Oct  8 17:17:11 sigurbergur kernel: [ 3259.777784] hello: Device has been opened 23 time(s)
// Oct  8 17:17:11 sigurbergur kernel: [ 3259.777836] hello write: accepting 16 bytes from the user
// Oct  8 17:17:11 sigurbergur kernel: [ 3259.777871] hello: Device successfully closed
// Oct  8 17:17:12 sigurbergur kernel: [ 3260.778126] hello: Device has been opened 24 time(s)
// Oct  8 17:17:12 sigurbergur kernel: [ 3260.778179] hello write: accepting 16 bytes from the user
// Oct  8 17:17:12 sigurbergur kernel: [ 3260.778212] hello: Device successfully closed
// Oct  8 17:17:13 sigurbergur kernel: [ 3261.778467] hello: Device has been opened 25 time(s)
// Oct  8 17:17:13 sigurbergur kernel: [ 3261.778518] hello write: accepting 16 bytes from the user
// Oct  8 17:17:13 sigurbergur kernel: [ 3261.778553] hello: Device successfully closed
// Oct  8 17:17:14 sigurbergur kernel: [ 3262.778805] hello: Device has been opened 26 time(s)
// Oct  8 17:17:14 sigurbergur kernel: [ 3262.778856] hello write: accepting 16 bytes from the user
// Oct  8 17:17:14 sigurbergur kernel: [ 3262.778890] hello: Device successfully closed
// Oct  8 17:17:15 sigurbergur kernel: [ 3263.779138] hello: Device has been opened 27 time(s)
// Oct  8 17:17:15 sigurbergur kernel: [ 3263.779277] hello write: accepting 16 bytes from the user
// Oct  8 17:17:15 sigurbergur kernel: [ 3263.779312] hello: Device successfully closed
// Oct  8 17:17:16 sigurbergur kernel: [ 3264.779566] hello: Device has been opened 28 time(s)
// Oct  8 17:17:16 sigurbergur kernel: [ 3264.779618] hello write: accepting 16 bytes from the user
// Oct  8 17:17:16 sigurbergur kernel: [ 3264.779654] hello: Device successfully closed
// Oct  8 17:17:17 sigurbergur kernel: [ 3265.779908] hello: Device has been opened 29 time(s)
// Oct  8 17:17:17 sigurbergur kernel: [ 3265.779961] hello write: accepting 16 bytes from the user
// Oct  8 17:17:17 sigurbergur kernel: [ 3265.779995] hello: Device successfully closed
// Oct  8 17:17:18 sigurbergur kernel: [ 3266.780247] hello: Device has been opened 30 time(s)
// Oct  8 17:17:18 sigurbergur kernel: [ 3266.780299] hello write: accepting 16 bytes from the user
// Oct  8 17:17:18 sigurbergur kernel: [ 3266.780333] hello: Device successfully closed