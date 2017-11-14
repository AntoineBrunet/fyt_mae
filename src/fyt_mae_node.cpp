#include "ros/ros.h"
#include "cmg_msgs/State.h"
#include "cmg_msgs/Signal.h"
#include "fyt_mae/fyt_mae.h"
#include "fyt_mae/fyt_commons.h"
#include "fyt_mae/CheckState.h"

namespace fyt_mae {
	class ROSMAE : public MAE {
		private:
			ros::NodeHandle n;
			ros::Publisher state_pub;
			ros::Subscriber signal_sub;
			ros::ServiceServer state_srv;
		public:
			virtual void warn_ignore(state_t st, signal_t sig) override {
				ROS_WARN("Ignoring signal %ud from state %ud", sig, st);
			}
			void sig_cb(const cmg_msgs::Signal::ConstPtr & msg) {
				if (signal(msg->signal)) {
					cmg_msgs::State msg;
					msg.state = current_state;
					state_pub.publish(msg);
				}
			}

			bool check_state(fyt_mae::CheckState::Request &req, fyt_mae::CheckState::Response &res) {
				res.state = current_state;
				return true;
			}

			ROSMAE() : MAE(FYT_INIT_STATE), n("~") {
				signal_sub = n.subscribe("signal", 1, &ROSMAE::sig_cb, this);
				state_pub = n.advertise<cmg_msgs::State>("state", 1);
				state_srv = n.advertiseService("check_state", &ROSMAE::check_state, this);
				
				add_transition(STATE_SAFE,	STATE_SAFE,	SIG_ALARM);
				add_transition(STATE_READY,	STATE_SAFE,	SIG_ALARM);
				add_transition(STATE_MISS,	STATE_SAFE,	SIG_ALARM);
				add_transition(STATE_POST,	STATE_SAFE,	SIG_ALARM);
				
				add_transition(STATE_SAFE,	STATE_READY,	SIG_GOOD);
				add_transition(STATE_POST,	STATE_READY,	SIG_GOOD);
				
				add_transition(STATE_READY,	STATE_MISS,	SIG_START);
				add_transition(STATE_MISS,	STATE_POST,	SIG_END);
			}
	};
}	

int main(int argc, char * argv[]) {
	ros::init(argc, argv, "mae");
	fyt_mae::ROSMAE mae;
	ros::spin();
	return 0;
}
