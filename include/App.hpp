#ifndef GLPAINT_APP_HPP
#define GLPAINT_APP_HPP

#include <thread>

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

	std::thread app_event_polling_thread_;
	EventQueue event_queue_;

	bool app_running_;

	App();

public:
	static App& instance() {
		static App app{};
		return app;
	}
	void operator()(); 
	void run(); 
	void deinit();
};

}

#endif