/*****************************************************************************
​ ​* ​ ​Copyright ​ ​(C) ​ ​2017 ​ ​by ​ Jayakrishnan HJ
​ ​*
​ ​* ​ ​Redistribution, ​ ​modification ​ ​or ​ ​use ​ ​of ​ ​this ​ ​software ​ ​in ​ ​source ​ ​or ​ ​binary
​ ​* ​ ​forms ​ ​is ​ ​permitted ​ ​as ​ ​long ​ ​as ​ ​the ​ ​files ​ ​maintain ​ ​this ​ ​copyright. ​ ​Users ​ ​are
​ ​* ​ ​permitted ​ ​to ​ ​modify ​ ​this ​ ​and ​ ​use ​ ​it ​ ​to ​ ​learn ​ ​about ​ ​the ​ ​field ​ ​of ​ ​embedded
​ ​* ​ ​software. ​ ​Jayakrishnan  HJ ​ ​and ​ ​the ​ ​University ​ ​of ​ ​Colorado ​ ​are ​ ​not ​ ​liable ​ ​for
​ ​* ​ ​any ​ ​misuse ​ ​of ​ ​this ​ ​material.
​ ​*
*****************************************************************************/
/**
​ ​* ​ ​@file ​ led_module.c
​ ​* ​ ​@brief ​ ​Linux device driver (character) for onboard Beaglebone LED
​ ​*
​ ​* ​ ​This  ​file ​ ​provides ​ ​an ​ ​abstraction ​ ​of ​ ​reading ​ ​and
​ ​* ​ ​writing ​ ​to ​ various  states  of  an  LED.
​ ​*
​ ​* ​ ​@author ​ Jayakrishnan HJ
​ ​* ​ ​@date ​ ​November ​ 20 ​ ​2017
​ ​* ​ ​@version ​ ​1.0
​ ​*
​ ​*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/kobject.h>
#include <linux/init.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jayakrishnan");
MODULE_DESCRIPTION("A LED LKM for Beaglebone");

static unsigned int gpioLED = 53; //USR0 LED
module_param(gpioLED, uint, S_IRUGO); ///< Param desc. S_IRUGO can be read/not changed
MODULE_PARM_DESC(gpioLED, "GPIO Button Number (default = 53)");

static char gpioName[8] = "gpioXXX";
static unsigned int ledState = 0;   // state of LED
static unsigned int dutycycle = 20; // 20 % dutycycle (default)
static unsigned int period = 2000;  // 2000ms (default)
static struct timer_list my_timer;
static int counter = 0;
static volatile unsigned int mode = 0; // 0- periodic, 1- latch

/* Show mode of operation - periodic or latch */
static ssize_t driverMode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    return sprintf(buf, "%d\n", mode);
}

/* Store mode of operation - periodic or latch */
static ssize_t driverMode_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count){
    unsigned int temp_mode;
    sscanf(buf,"%d",&temp_mode);
    if((temp_mode == 0)||(temp_mode == 1))
        mode = temp_mode;
    return count;
}

/* show state of LED */
static ssize_t ledState_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    return sprintf(buf, "%d\n", ledState);
}

/* store state of LED */
static ssize_t ledState_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count){
    unsigned int temp_ledState;
    sscanf(buf,"%d",&temp_ledState);
    if((temp_ledState == 0)||(temp_ledState == 1)){
        ledState = temp_ledState;
        gpio_set_value(gpioLED, ledState);
    }

    return count;
}

/* show dutycycle of LED */
static ssize_t dutycycle_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    return sprintf(buf, "%d\n", dutycycle);
}

/* store dutycycle of LED */
static ssize_t dutycycle_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count){
    unsigned int temp_dutycycle;
    sscanf(buf,"%d",&temp_dutycycle);
    if((temp_dutycycle > 10) && (temp_dutycycle < 80))
        dutycycle = temp_dutycycle;
    return count;
}

/* show period of LED flashing */
static ssize_t period_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    return sprintf(buf, "%d\n", period);
}

/* store LED flash period */
static ssize_t period_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count){
    unsigned int temp_period;
    sscanf(buf,"%d",&temp_period);
    if((temp_period > 100) && (temp_period < 10000))
        period = temp_period;
    return count;
}

/**  Use these helper macros to define the name and access levels of the kobj_attributes
 *  The kobj_attribute has an attribute attr (name and mode), show and store function pointers
 *  The various states of the LED is to be exposed
 *  with mode 0644 using the functions above
 */

static struct kobj_attribute ledState_attr = __ATTR(ledState, 0644, ledState_show, ledState_store);
static struct kobj_attribute dutycycle_attr = __ATTR(dutycycle, 0644, dutycycle_show, dutycycle_store);
static struct kobj_attribute period_attr = __ATTR(period, 0644, period_show, period_store);
static struct kobj_attribute mode_attr = __ATTR(mode, 0644, driverMode_show, driverMode_store);

/* This is if I had just a read only attribute */
//static struct kobj_attribute ledState_attr = __ATTR_RO(ledState);

/**  The ebb_attrs[] is an array of attributes that is used to create the attribute group below.
 *  The attr property of the kobj_attribute is used to extract the attribute struct
 */
static struct attribute *ebb_attrs[] = {

  &ledState_attr.attr,
  &dutycycle_attr.attr,
  &period_attr.attr,
  &mode_attr.attr,
  NULL,
};


/**  The attribute group uses the attribute array and a name, which is exposed on sysfs -- in this
 *  case it is gpio53.
 */
static struct attribute_group attr_group = {
    .name = gpioName,
    .attrs = ebb_attrs,
};

static struct kobject *ebb_kobj;

void timer_callback(unsigned long data){

    ++counter;
    ledState = !ledState;
    if(mode == 0)
        gpio_set_value(gpioLED, ledState);

    if (counter == 1){
        printk(KERN_INFO "timer callback: counter 1");
        mod_timer(&my_timer, jiffies + msecs_to_jiffies((period * dutycycle)/100));
    }
    else if(counter == 2){
        printk(KERN_INFO "timer callback: counter 2");
        mod_timer(&my_timer, jiffies + msecs_to_jiffies((period*(100-dutycycle)/100)));
        counter = 0;
    }

}

static int __init test_init(void){
    int result = 0;
    int ret;
    printk(KERN_INFO "Initializing custom module");
    sprintf(gpioName, "gpio%d", gpioLED);

    ebb_kobj = kobject_create_and_add("ebb", kernel_kobj->parent);
    if(!ebb_kobj){
        printk(KERN_ALERT "EBB: LED failed to create mapping\n");
        return -ENOMEM;
    }

    result = sysfs_create_group(ebb_kobj, &attr_group);
    if(result){
        printk(KERN_ALERT"EBB: LED failed to create sysfs group\n");
        return result;
    }

    if(!gpio_is_valid(gpioLED)){
        printk(KERN_INFO "Invalid GPIO pin");
        return -ENODEV;
    }
    gpio_request(gpioLED,"sysfs");
    gpio_direction_output(gpioLED,0);
    gpio_export(gpioLED,false);
    gpio_set_value(gpioLED, ledState);

    //Timer Initializations

    setup_timer(&my_timer,timer_callback,0);
    ret =  mod_timer(&my_timer, jiffies + msecs_to_jiffies((period * dutycycle)/100));
    if (ret){
        printk(KERN_ALERT "error mod_timer");
        return ret;
    }


    return 0;
}

static void __exit test_cleanup(void){

    printk(KERN_INFO "LED test: current state of LED: %d ", gpio_get_value(gpioLED));

    kobject_put(ebb_kobj);
    gpio_set_value(gpioLED,0);
    gpio_unexport(gpioLED);
    gpio_free(gpioLED);
    del_timer(&my_timer);

    printk(KERN_INFO "Exiting module");

}

module_init(test_init);
module_exit(test_cleanup);