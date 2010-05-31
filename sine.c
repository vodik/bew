#include <synth.h>
#include <math.h>

void
sine_work(base_module_t *m, short *buf, size_t nloops)
{
	sine_module_t *sm = (sine_module_t *)m;
	size_t i;

	for (i = 0; i < nloops; ++i) {
		float val = sin(sm->phase) * 1024.0 * m->volume;
		buf[i] += (short)val;

		sm->phase += sm->step;
		if (sm->phase >= 2.0 * M_PI)
			sm->phase = 0;
	}
}

void
sine_init(base_module_t *m, float val)
{
	sine_module_t *sm = (sine_module_t *)m;

	m->work = sine_work;
	sm->phase = 0;
	sm->step = 2.0 * M_PI * val / 48000.0f;
}
