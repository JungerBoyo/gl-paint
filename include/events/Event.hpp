#ifndef GLPAINT_EVENT_HPP
#define GLPAINT_EVENT_HPP

#include <type_traits>
#include <common.hpp>

namespace glpaint {

struct Event {
	template <typename T>
	requires std::is_base_of_v<Event, T>
	static T* dispatch(Event* ev) {
		return static_cast<T*>(ev);
	}
};

}

#endif