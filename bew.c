#include <stdlib.h>
#include <stdio.h>

#include <synth.h>

int
main(int argc, char *argv[])
{
	base_module_t *mod = synth_init("/dev/dsp");
	synth_add("sine", 440.0);
	synth_add("sine", 540.0);
	//synth_add("reverb", 0.0);

	synth_play();
	return EXIT_SUCCESS;
}
