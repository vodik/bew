#ifndef SYNTH_MODULE_H
#define SYNTH_MODULE_H

#include <stddef.h>

typedef struct _module {
	void (*work)(struct _module *m, short *buf, size_t nloops);
	struct _module *prev, *next;

	float volume;
} base_module_t;

/* sine.c */
typedef struct _sine_module {
	base_module_t base;
	float phase, step;
} sine_module_t;

void sine_init(base_module_t *m, float val);

/* reverb.c */
typedef struct _reverb_module {
	base_module_t base;
} reverb_module_t;

void reverb_init(base_module_t *m, float val);

#endif
