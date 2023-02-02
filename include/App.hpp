#ifndef GLPAINT_APP_HPP
#define GLPAINT_APP_HPP

#include <thread>
#include <stop_token>

#include <RingQueue.hpp>
#include <Event.hpp>
#include <WindowLayer.hpp>
#include <UILayer.hpp>
#include <GLLayer.hpp>
#include <common.hpp>

namespace glpaint {

struct App {
private:
	WindowLayer window_layer_;
	GLLayer gl_layer_;
	UILayer ui_layer_;

	std::stop_source app_stop_source_;

	std::thread app_event_polling_thread_;
	EventQueue event_queue_;

	App();

public:
	static App& instance() {
		static App app{};
		return app;
	}
	void operator()(std::stop_token); 
	void run(); 
	void deinit();
};

}

#endif