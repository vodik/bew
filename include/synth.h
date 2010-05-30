#ifndef SYNTH_H
#define SYNTH_H

#include <module.h>

/*#include <mod/sine.h>
#include <mod/reverb.h>*/

typedef struct {
	const char *name;
	void (*init)(base_module_t *m, float val);
	size_t module_size;
} mod_descript;

static const mod_descript modules[] = {
	 { "sine",   sine_init,   sizeof(sine_module_t)   },
	 { "reverb", reverb_init, sizeof(reverb_module_t) },
};

base_module_t *synth_init(const char *device);
base_module_t *synth_add(const char *module, float val);
void synth_play();

#endif
