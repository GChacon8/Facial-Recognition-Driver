#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>  // Para el acceso a memoria de espacio de usuario
#include <linux/device.h>    // Para device_create

#define DEVICE_NAME "rpi_camera"

static dev_t dev_num;
static struct cdev rpi_camera_cdev;
static struct class *rpi_camera_class;

// Declaración de las funciones de operaciones del archivo
static int rpi_camera_open(struct inode *inode, struct file *file);
static int rpi_camera_release(struct inode *inode, struct file *file);
static ssize_t rpi_camera_read(struct file *file, char __user *buf, size_t len, loff_t *offset);
static ssize_t rpi_camera_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);
static long rpi_camera_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

// Estructura file_operations
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = rpi_camera_open,
    .read = rpi_camera_read,
    .write = rpi_camera_write,
    .release = rpi_camera_release,
    .unlocked_ioctl = rpi_camera_ioctl,
};

static int __init rpi_camera_init(void) {
    int ret;

    // Asignar número mayor y menor
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ALERT "Failed to allocate device number\n");
        return ret;
    }

    printk(KERN_INFO "Allocated device number: %d\n", dev_num);

    // Crear la clase para el dispositivo
    rpi_camera_class = class_create(DEVICE_NAME);
    if (IS_ERR(rpi_camera_class)) {
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "Failed to create class\n");
        return PTR_ERR(rpi_camera_class);
    }

    // Inicializar cdev
    cdev_init(&rpi_camera_cdev, &fops);
    rpi_camera_cdev.owner = THIS_MODULE;

    ret = cdev_add(&rpi_camera_cdev, dev_num, 1);
    if (ret < 0) {
        printk(KERN_ALERT "Failed to add cdev\n");
        class_destroy(rpi_camera_class);
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    // Crear el dispositivo en /dev
    if (IS_ERR(device_create(rpi_camera_class, NULL, dev_num, NULL, DEVICE_NAME))) {
        printk(KERN_ALERT "Failed to create device\n");
        class_destroy(rpi_camera_class);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(rpi_camera_class);
    }

    printk(KERN_INFO "Device registered: /dev/%s\n", DEVICE_NAME);
    return 0;
}

static void __exit rpi_camera_exit(void) {
    device_destroy(rpi_camera_class, dev_num); // Destruir el dispositivo
    class_destroy(rpi_camera_class);           // Destruir la clase
    cdev_del(&rpi_camera_cdev);                // Eliminar el cdev
    unregister_chrdev_region(dev_num, 1);      // Liberar el número mayor
    printk(KERN_INFO "Device unregistered\n");
}

module_init(rpi_camera_init);
module_exit(rpi_camera_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tu nombre");
MODULE_DESCRIPTION("Raspberry Pi Camera Driver");

// Implementación de las funciones de operaciones
static int rpi_camera_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Camera opened\n");
    return 0;
}

static int rpi_camera_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Camera released\n");
    return 0;
}

static ssize_t rpi_camera_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    printk(KERN_INFO "Camera read\n");
    return 0;
}

static ssize_t rpi_camera_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    printk(KERN_INFO "Camera write\n");
    return len;
}

static long rpi_camera_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    printk(KERN_INFO "Camera ioctl\n");
    return 0;
}
