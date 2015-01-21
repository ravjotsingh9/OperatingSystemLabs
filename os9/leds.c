/*
 * leds.c
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <asm/uaccess.h> /* for put_user */
#include <asm/io.h>
/* 
 * Prototypes - this would normally go in a .h file
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
#define SUCCESS 0
#define DEVICE_NAME "leds" /* Dev name as it appears in /proc/devices */
#define BUF_LEN 80 /* Max length of the message from the device */
/* 
 * Global variables are declared as static, so are global within the file. 
 */
static int Major; /* Major number assigned to our device driver */
static int Device_Open = 0; /* Is device open? 
 * Used to prevent multiple access to device */
static char msg[BUF_LEN]; /* The msg the device will give when asked */
static char *msg_Ptr;
static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};
/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if (Major < 0) {
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}
	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");
	return SUCCESS;
}
/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	/* 
	* Unregister the device 
	*/
	unregister_chrdev(Major, DEVICE_NAME);
}
/*
 * Methods
 */
/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
/*
	static int counter = 0;
	if (Device_Open)
	return -EBUSY;
	Device_Open++;
	sprintf(msg, "I already told you %d times Hello world!\n", counter++);
	msg_Ptr = msg;
*/
	try_module_get(THIS_MODULE);

	return SUCCESS;
}
/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--; /* We're now ready for our next caller */
	/* 
	* Decrement the usage count, or else once you opened the file, you'll
	* never get get rid of the module. 
	 */
	module_put(THIS_MODULE);
	return 1;
}
/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h */
	char *buffer, /* buffer to fill with data */
	 size_t length, /* length of the buffer */
	 loff_t * offset)
{
	return 1;
}
/* 
 * Called when a process writes to dev file: echo "hi" > /dev/hello 
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	int retries = 5;
	int timeout = 1000;
		int state =0;

	int count =0;
	while(count< len)
	{
		if(buff[count]=='1')
		{
			printk("numlock\n");
			state= 0x02;
		}
		else if(buff[count]=='2')
		{
			printk("capslock\n");
			state= 0x04;
		}
		else if(buff[count]=='3')
		{
			printk("scrolllock\n");
			state= 0x01;
		}
		else
		{
			state= 0x00;
		}
		count++;
	}
	

	outb(0xed,0x60);// tell the keyboard we want to set the leds 		configuration
	udelay(timeout);
	
	while (retries!=0 && inb(0x60)!=0xfa) { // wait for the keyboard 	controller
		retries--;
		udelay(timeout);
	}
	
	if (retries!=0) { //check is keyboard is ready to accept command
		outb(state,0x60);
	}

	return 1;
}
