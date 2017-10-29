#ifndef FYT_MAE_H
#define FYT_MAE_H

#include <map>

namespace fyt_mae {
	typedef uint32_t state_t;
	typedef uint32_t signal_t;

	class MAE {
		private:
			std::map<state_t, std::map<signal_t, state_t> > transitions;
		public:
			virtual void warn_ignore(state_t st, signal_t sig);
			state_t current_state;
			MAE(state_t init_state);
			void add_transition(state_t ids, state_t ide, signal_t sig);
			bool signal(signal_t sig);
	};
}

#endif
