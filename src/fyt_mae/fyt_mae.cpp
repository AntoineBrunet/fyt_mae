#include "fyt_mae/fyt_mae.h"
#include <iostream>

namespace fyt_mae {
	MAE::MAE(state_t init_state) : current_state(init_state) { }

	void MAE::add_transition(state_t ids, state_t ide, signal_t sig) {
		transitions[ids][sig] = ide;
	}

	bool MAE::signal(signal_t sig) {
		std::map<signal_t, state_t>::iterator nxt = transitions[current_state].find(sig);
		if (nxt != transitions[current_state].end()) {
			current_state = nxt->second;
			return true;
		} else {
			warn_ignore(current_state, sig);
			return false;
		}
	}

	void MAE::warn_ignore(state_t st, signal_t sig) {
		std::cout << "Ignoring signal" << sig << " from state " << st << std::endl;
	}
}
