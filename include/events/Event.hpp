#ifndef GLPAINT_EVENT_HPP
#define GLPAINT_EVENT_HPP

#include <type_traits>
#include <common.hpp>

namespace glpaint {

struct Event {
	/**
	 * @brief event tag signifies which layers should
	 * care about this event
	 */
	enum class Tag : u64 {
		APPLICATION = 0x00'00'00'00'00'00'00'01,
		GL_LAYER 	= 0x00'00'00'00'00'00'00'02
	};
	/**
	 * @brief concrete types of events with tags
	 */
	enum class Type : u64 {
		/// Application events
		CANCELLATION  		= 0x00'00'00'00'00'00'00'01,
		WINDOW_RESIZE 		= 0x00'00'00'00'00'00'00'02,
		IMAGE_LAYER_SWAP 	= 0x00'00'00'00'00'00'00'04,
		LAYER_ADD			= 0x00'00'00'00'00'00'00'08,
		SCROLL				= 0x00'00'00'00'00'00'00'10,
	};

	[[nodiscard]] virtual Tag tag() const = 0;
	[[nodiscard]] virtual Type type() const = 0;

	template <typename T>
	requires std::is_base_of_v<Event, T>
	static const T* dispatch(const Event* ev) {
		return static_cast<const T*>(ev);
	}

	virtual ~Event() = default;
};

inline constexpr auto operator|(Event::Type lhs, Event::Type rhs) {
    return static_cast<u64>(lhs) | static_cast<u64>(rhs);
}
inline constexpr auto operator&(Event::Type lhs, Event::Type rhs) {
    return static_cast<u64>(lhs) & static_cast<u64>(rhs);
}
inline constexpr auto operator|(Event::Tag lhs, Event::Tag rhs) {
    return static_cast<u64>(lhs) | static_cast<u64>(rhs);
}
inline constexpr auto operator&(Event::Tag lhs, Event::Tag rhs) {
    return static_cast<u64>(lhs) & static_cast<u64>(rhs);
}

struct CancellationEvent : public Event {
	[[nodiscard]] Tag tag() const override { return Tag::APPLICATION; }
	[[nodiscard]] Type type() const override { return Type::CANCELLATION; }
};

struct WindowResizeEvent : public Event {
	i32 width;
	i32 height;

	WindowResizeEvent(i32 new_width, i32 new_height) : 
		width(new_width), height(new_height) {}

	[[nodiscard]] Tag tag() const override { return Tag::GL_LAYER; }
	[[nodiscard]] Type type() const override { return Type::WINDOW_RESIZE; }
};

struct ImageLayerSwapEvent : public Event {
	i32 src_layer_index;
	i32 dst_layer_index;

	ImageLayerSwapEvent(i32 src_layer_index, i32 dst_layer_index) :
		src_layer_index(src_layer_index), dst_layer_index(dst_layer_index) {}
	
	[[nodiscard]] Tag tag() const override { return Tag::GL_LAYER; }
	[[nodiscard]] Type type() const override { return Type::IMAGE_LAYER_SWAP; }
};

struct LayerAddEvent : public Event {
	[[nodiscard]] Tag tag() const override { return Tag::GL_LAYER; }
	[[nodiscard]] Type type() const override { return Type::LAYER_ADD; }
};

struct ScrollEvent : public Event {
	f64 x;
	f64 y;

	ScrollEvent(f64 x, f64 y) : x(x), y(y) {}

	[[nodiscard]] Tag tag() const override { return Tag::GL_LAYER; }
	[[nodiscard]] Type type() const override { return Type::SCROLL; }
};

}

#endif