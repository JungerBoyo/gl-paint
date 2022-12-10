#ifndef GLPAINT_UI_LAYER_HPP
#define GLPAINT_UI_LAYER_HPP

#include <RingQueue.hpp>
#include <Event.hpp>
#include <common.hpp>

namespace glpaint {

using EventQueue = RingQueue<std::unique_ptr<Event>, 128>;

struct UILayer {
private:
	EventQueue& event_queue_;

public:
	UILayer(void* window_handle, EventQueue& event_queue);

	void update(u32 viewport_texture_id); 
	void deinit();
};

}

#endif