#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stropts.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/soundcard.h>

#include <synth.h>

int afd;
base_module_t *root;

static void
die(const char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

static int
open_audio_device(const char *name, int mode)
{
	int tmp, fd;

	if ((fd = open(name, mode, 0)) == -1)
		die(name);

	tmp = 0x00040009;
	if (ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &tmp) == -1)
		die("SNDCTL_DSP_SETFRAGMENT");

	tmp = AFMT_S16_NE;
	if (ioctl(fd, SNDCTL_DSP_SETFMT, &tmp) == -1)
		die("SNDCTL_DSP_SETFMT");
	if (tmp != AFMT_S16_NE)
		die("16bit sample format not supported");

	tmp = 1;
	if (ioctl(fd, SNDCTL_DSP_CHANNELS, &tmp) == -1)
		die("SNDCTL_DSP_SETFRAGMENT");
	if (tmp != 1)
		die("mono not supported");

	tmp = 48000;
	if (ioctl(fd, SNDCTL_DSP_SPEED, &tmp) == -1)
		die("SNDCTL_DSP_SPEED");

	return fd;
}

static void
handle_audio_output()
{
	short buf[512] = { 0 };

	base_module_t *m = root;
	while (m) {
		m->work(m, buf, 512);
		m = m->next;
	}

	if (write(afd, buf, sizeof(buf)) == -1)
		die("audio write failed\n");
}

void
_add(base_module_t *m)
{
	static base_module_t *c = NULL;

	if (root == NULL) {
		root = m;
		root->next = NULL;
		root->prev = NULL;
		c = root;
	} else {
		c->next = m;
		m->prev = c;
		m->next = NULL;
		c = m;
	}
}

const mod_descript *
find_module(const char *c)
{
	int i;

	for (i = 0; i < sizeof(modules) / sizeof(modules[0]); ++i) {
		if (strcmp(c, modules[i].name) == 0)
			return &modules[i];
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////

base_module_t *
synth_init(const char *device)
{
	afd = open_audio_device(device, O_WRONLY);
	root = NULL;
	return NULL;
}

base_module_t *
synth_add(const char *name, float val)/*base_module_t *c*/
{
	const mod_descript *d = find_module(name);
	if (!d)
		die("failed to match module\n");

	printf("-- add: %s\n", name);
	base_module_t *m = malloc(d->module_size);
	m->volume = 1.0;
	d->init(m, val);
	_add(m);

	return m;
}

void
synth_play()
{
	fd_set fds;
	int n;

	printf("-- play\n");
	while (1) {
		FD_ZERO(&fds);
		FD_SET(afd, &fds);

		if ((n = select(afd + 1, NULL, &fds, NULL, NULL)) == -1)
			die("select");

		if (n > 0) {
			if (FD_ISSET(afd, &fds))
				handle_audio_output();
		}
	}
}
