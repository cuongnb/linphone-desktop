#include "ispell.h"

extern "C" {
	int bc_spell_checker (const char * dicts_folder, const char * locale, int read_fd, int write_fd);
}
