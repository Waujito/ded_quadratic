/**
 * @file
 *
 * @brief argparse machine
 */
#include "argparse.h"

int parse_args(int argc, char **argv, 
	       struct option opts[],
	       args_callback callback, void *context) {

	for (int argi = 1; argi < argc; argi++) {
		const char *arg_name = argv[argi];
		int opt_id = -1;
		const char *value = 0; 
		
		for (int opti = 0; opts[opti].name; opti++) {
			struct option opt = opts[opti];
			const char *optname = opt.name;
			const char *argn_cp = arg_name;

			// read dashes
			int nDashes = 0;
			while (*argn_cp == '-') nDashes++, argn_cp++;

			int cmp_len = 0;
			while (*optname && *argn_cp && *optname == *argn_cp)
				optname++, argn_cp++, cmp_len++;

			if (*argn_cp && *argn_cp != '=')
				continue;

			if (	(nDashes == 1 && cmp_len != 1) || 
				nDashes != 2)
				return -argi;
			

			if (*argn_cp == '=') {
				value = ++argn_cp;
			} else if (opt.has_arg == REQUIRED_ARGUMENT) {
				if (argi + 1 < argc) {
					value = argv[++argi];
				} else {
					return -argi;
				}
			}

			if (opt.has_arg == REQUIRED_ARGUMENT && !value) {
				return -argi;
			}

			opt_id = opti;
		}

		if (opt_id != -1) {
			struct args_context ctx = {
				.opt = opts[opt_id],
				.value = value,
				.context = context
			};
			int ret = callback(ctx);
			if (ret) {
				return -argi;
			}
		} else {
			return -argi;
		}

	}

	return 0;
}
