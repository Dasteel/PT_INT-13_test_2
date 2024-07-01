#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dasteel");
MODULE_DESCRIPTION("A simple Linux kernel module for storing secrets in procfs");
MODULE_VERSION("0.1.0-alpha1");

struct secret {
    int id;
    char *data;
    struct list_head list;
};

static LIST_HEAD(secret_list);
static int next_id = 1;

static ssize_t write_secret(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
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

static ssize_t read_secret(struct file *file, char __user *buffer, size_t count, loff_t *ppos) {
    struct secret *secret;
    char *output;
    size_t len = 0;

    list_for_each_entry(secret, &secret_list, list) {
        len += snprintf(NULL, 0, "ID: %d, Secret: %s\n", secret->id, secret->data);
    }

    if (*ppos >= len)
        return 0;

    output = kmalloc(len + 1, GFP_KERNEL);
    if (!output)
        return -ENOMEM;

    len = 0;
    list_for_each_entry(secret, &secret_list, list) {
        len += sprintf(output + len, "ID: %d, Secret: %s\n", secret->id, secret->data);
    }

    if (copy_to_user(buffer, output, len)) {
        kfree(output);
        return -EFAULT;
    }

    *ppos += len;
    kfree(output);
    return len;
}

static ssize_t delete_secret(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
    int id;
    struct secret *secret, *tmp;

    if (kstrtoint_from_user(buffer, count, 10, &id))
        return -EFAULT;

    list_for_each_entry_safe(secret, tmp, &secret_list, list) {
        if (secret->id == id) {
            list_del(&secret->list);
            kfree(secret->data);
            kfree(secret);
            return count;
        }
    }
    return -ENOENT;
}

static struct proc_ops fops_write_read = {
    .proc_write = write_secret,
    .proc_read = read_secret,
};

static struct proc_ops fops_delete = {
    .proc_write = delete_secret,
};

static int __init secret_init(void) {
    proc_create("secret_storage", 0666, NULL, &fops_write_read);
    proc_create("secret_storage_delete", 0666, NULL, &fops_delete);
    printk(KERN_INFO "Secret storage module loaded\n");
    return 0;
}

static void __exit secret_exit(void) {
    struct secret *secret, *tmp;
    remove_proc_entry("secret_storage", NULL);
    remove_proc_entry("secret_storage_delete", NULL);
    list_for_each_entry_safe(secret, tmp, &secret_list, list) {
        kfree(secret->data);
        kfree(secret);
    }
    printk(KERN_INFO "Secret storage module unloaded\n");
}

module_init(secret_init);
module_exit(secret_exit);

