/**
 * @file
 * @brief
 *
 * @author  Anton Kozlov
 * @date    09.08.2013
 */
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <util/err.h>
#include <util/indexator.h>

#include <mem/misc/pool.h>
#include <drivers/char_dev.h>
#include <drivers/device.h>
#include <drivers/serial/uart_device.h>
#include "idesc_serial.h"
#include <fs/dvfs.h>
extern int idesc_uart_bind(struct uart *uart);

int uart_dev_open(struct dev_module *mod, void *dev_priv) {
	int res;

	idesc_uart_bind(dev_priv);

	if ((res = uart_open(dev_priv))) {
		return res;
	}

//	node->flags |= DVFS_NO_LSEEK;

	return 0;
}

#define SERIAL_POOL_SIZE OPTION_GET(NUMBER, serial_quantity)
POOL_DEF(cdev_serials_pool, struct dev_module, SERIAL_POOL_SIZE);

extern const struct idesc_ops idesc_serial_ops;

int ttys_register(const char *name, void *dev_info) {
	struct dev_module *cdev;

	cdev = pool_alloc(&cdev_serials_pool);
	if (!cdev) {
		return -ENOMEM;
	}
	memset(cdev, 0, sizeof(*cdev));
	memcpy(cdev->name, name, sizeof(cdev->name));
	cdev->dev_priv = dev_info;
	cdev->dev_file.f_ops = &char_dev_fops;

	return char_dev_register(cdev);
}
