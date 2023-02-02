#ifndef GLPAINT_UI_LAYER_HPP
#define GLPAINT_UI_LAYER_HPP

#include <span>

#include <RingQueue.hpp>
#include <Event.hpp>
#include <common.hpp>

namespace glpaint {

using EventQueue = RingQueue<std::unique_ptr<Event>, 128>;

struct UILayer {
	struct InUIViewportDims {
		f32 left_margin_offset{ 0.F };
		f32 top_margin_offset{ 0.F };
		f32 extent_x{ 0.F };
		f32 extent_y{ 0.F };
	};

private:
	EventQueue& event_queue_;

public:
	UILayer(void* window_handle, EventQueue& event_queue);

	InUIViewportDims update(
		u32 viewport_texture_id, 
		i32 viewport_width, 
		i32 viewport_height,
		std::span<const u32> layer_view_ids
	); 

	void deinit();
};

}

#endif