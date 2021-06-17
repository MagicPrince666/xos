/**
 * @file am335x_mac.c
 * @author 黄李全 (hlq@ldrobot.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-15
 * @copyright Copyright (c) {2021} 深圳乐动机器人版权所有
 */

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/clk.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>


#define DRIVER_NAME "am335x_mac"
#define MODULE_VERS "1.0"
#define MODULE_NAME "am335x_serial"

#define AM33XX_CONTROL_PADCONF_MUX_PBASE            0x44E10000LU

#define MAC_ID0_LO AM33XX_CONTROL_PADCONF_MUX_PBASE+0x630
#define MAC_ID0_HI AM33XX_CONTROL_PADCONF_MUX_PBASE+0x634

#define MAC_ID1_LO AM33XX_CONTROL_PADCONF_MUX_PBASE+0x638
#define MAC_ID1_HI AM33XX_CONTROL_PADCONF_MUX_PBASE+0x63c

unsigned int *mac_config; 

static struct proc_dir_entry *am335x_dir,*am335x_proc_file;

static ssize_t proc_read(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
	int ret;
	//printk(KERN_ALERT "proc_read (/proc/%s) called\n", MODULE_NAME);
	if(*ppos > 0)
		return 0;
	
	uint32_t mac0[4] = {0};
	uint8_t* art_mmap_reg;

	mac_config = ioremap(MAC_ID0_LO, 4);
	mac0[0] = readl(mac_config);
	mac_config = ioremap(MAC_ID0_HI, 4);
	mac0[1] = readl(mac_config);
	mac_config = ioremap(MAC_ID1_LO, 4);
	mac0[2] = readl(mac_config);
	mac_config = ioremap(MAC_ID1_HI, 4);
	mac0[3] = readl(mac_config);

	art_mmap_reg = (uint8_t*)mac0;
	/* am335x mac1 6 byrtes + mac0 2 bytes*/
	ret = sprintf(ubuf ,"%02x%02x%02x%02x%02x%02x\n",
		art_mmap_reg[12],
		art_mmap_reg[13],
		art_mmap_reg[14],
		art_mmap_reg[15],
		art_mmap_reg[8],
		art_mmap_reg[9]
		);
	*ppos = ret;
/*
	mac_config = ioremap(MAC_ID1_LO, 4);
	mac0[0] = readl(mac_config);
	mac_config = ioremap(MAC_ID1_HI, 4);
	mac0[1] = readl(mac_config);

	printk("ETH1 MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
		art_mmap_reg[4],
		art_mmap_reg[5],
		art_mmap_reg[6],
		art_mmap_reg[7],
		art_mmap_reg[0],
		art_mmap_reg[1]
		);

	printk("am335x get mac address\n");
*/
	return ret;
}


static struct file_operations dev_proc_ops = {
    .owner = THIS_MODULE,
	.read = proc_read
};

static int __init am335x_serial_init(void)
{
	//uint32_t data;
	printk("am335x mac address read\n");

	am335x_dir = proc_mkdir("am335x", NULL);
	if(am335x_dir == NULL) {
		proc_remove(am335x_dir);
		am335x_dir = proc_mkdir("am335x", NULL);
	}

	am335x_proc_file = proc_create(MODULE_NAME, 0666, am335x_dir, &dev_proc_ops);
	if(am335x_proc_file == NULL) {
       //proc_remove(am335x_proc_file);
	   printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",MODULE_NAME);
		return -ENOMEM;
    }

	return 0;
}
 
static void __exit am335x_serial_exit(void)
{
	if(am335x_proc_file != NULL) {
		proc_remove(am335x_proc_file);
	}
	if(am335x_dir != NULL) {
		proc_remove(am335x_dir);
	}
	printk("am335x remove get mac address\n");
}

module_init(am335x_serial_init);
module_exit(am335x_serial_exit);

MODULE_DESCRIPTION("AM335X GET MAC driver");
MODULE_AUTHOR("huang liquan <846863428@qq.com>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" DRIVER_NAME);

