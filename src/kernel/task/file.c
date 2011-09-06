/**
 * @file
 * @brief File descriptor (fd) abstraction over FILE *
 *
 * @date 06.09.11
 * @author Anton Kozlov
 */

#include <types.h>
#include <errno.h>
#include <kernel/task.h>
#include <lib/list.h>

static int alloc_fd(struct task *tsk) {
	if (list_empty(&tsk->free_fds)) {
		return -ENOMEM;
	}

	list_move(tsk->free_fds.next, &tsk->opened_fds);

	return ENOERR;
}

static void free_fd(struct task *tsk, int fd) {
	struct __fd_list *fd_lst = &tsk->fds[fd];
	list_move(&fd_lst->list_hnd, &tsk->free_fds);
}

static int file_opened(FILE *file, struct task *tsk) {
	int fd;

	fd = alloc_fd(tsk);

	tsk->fds[fd].file = file;

	return fd;
}

static void file_closed(int fd, struct task *tsk) {
	tsk->fds[fd].file = NULL;
	free_fd(tsk, fd);
}
#if 0
static int reopen_fd(int fd, FILE *file, struct task *tsk) {
	if (tsk->fds[fd].file != NULL) {
		fclose(tsk->fds[fd].file);
		file_closed(fd, tsk);
	}
	file_opened(file, tsk);
	return 0;
}
#endif
int open(const char *path, const char *mode) {
	FILE *file = fopen(path, mode);

	return file_opened(file, task_self());
}

//XXX should be just close, interference with socket's close
int file_close(int fd) {
	struct task *tsk = task_self();

	FILE *file = tsk->fds[fd].file;

	if (file == NULL) {
		return -EBADF;
	}

	file_closed(fd, tsk);

	return ENOERR;
}

ssize_t write(int fd, const void *buf, size_t nbyte) {
	struct task *tsk = task_self();

	return fwrite(buf, 1, nbyte, tsk->fds[fd].file);
}

//FIXME KILL-ME
int task_create_with_io(struct task **new, struct task *parent, FILE *stdin, FILE *stdout, FILE *stderr) {

	int res = task_create(new, parent);
	if (res != 0) {
		return res;
	}

	file_opened(stdin, *new);
	file_opened(stdout, *new);
	file_opened(stderr, *new);

	return ENOERR;
}

