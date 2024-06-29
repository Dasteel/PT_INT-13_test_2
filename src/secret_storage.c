#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/uaccess.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dasteel");
MODULE_DESCRIPTION("A simple Linux kernel module for storing secrets in procfs");
MODULE_VERSION("v0.1.0-alpha");

struct secret {
    int id;
    char *data;
    struct list_head list;
};

static LIST_HEAD(secret_list);
static int next_id = 1;

tatic ssize_t write_secret(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
    struct secret *new_secret;
    new_secret = kmalloc(sizeof(*new_secret), GFP_KERNEL);
    if (!new_secret)
        return -ENOMEM;

    new_secret->data = kmalloc(count + 1, GFP_KERNEL);
    if (!new_secret->data) {
        kfree(new_secret);
        return -ENOMEM;
    }

    if (copy_from_user(new_secret->data, buffer, count)) {
        kfree(new_secret->data);
        kfree(new_secret);
        return -EFAULT;
    }
    new_secret->data[count] = '\0';
    new_secret->id = next_id++;
    list_add(&new_secret->list, &secret_list);
    return count;
}

static int __init secret_init(void) {
    printk(KERN_INFO "Secret storage module loaded\n");
    return 0;
}

static void __exit secret_exit(void) {
    printk(KERN_INFO "Secret storage module unloaded\n");
}

module_init(secret_init);
module_exit(secret_exit);

