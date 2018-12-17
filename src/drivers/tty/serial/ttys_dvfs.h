/**
 * @file
 *
 * @date 21.04.2016
 * @author Anton Bondarev
 */

#ifndef TTYS_H_
#define TTYS_H_

#include <fs/idesc.h>
#include <drivers/tty.h>
struct uart;
struct tty_uart {
	struct idesc idesc;
	struct tty tty;
	struct uart *uart;
};

#include <drivers/char_dev.h>
#define TTYS_DEF(name, uart) \
		extern int uart_dev_open(struct dev_module *mode, void *dev_priv); \
		extern const struct idesc_ops idesc_serial_ops; \
		CHAR_DEV_DEF(name, &idesc_serial_ops, uart_dev_open, uart)

#endif /* TTYS_H_ */
