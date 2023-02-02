#ifndef GLPAINT_WINDOW_HPP
#define GLPAINT_WINDOW_HPP

#include <memory>
#include <string_view>

#include <common.hpp>


namespace glpaint {

struct Window {
	struct WinNative;
	std::shared_ptr<WinNative> win_handle_;

	Window(std::string_view title, i32 w, i32 h, void (*win_error_callback)(i32 err_code, const char* message));
	
	[[nodiscard]] void* native();
	[[nodiscard]] std::pair<i32, i32> size() const;
	[[nodiscard]] f32 time() const;

	void setViewport(i32 w, i32 h) const;
	void setWinUserDataPointer(void* ptr);

	template<typename R, typename ...Args>
	void setKeyCallback(R(*key_callback)(Args...));

	template<typename R, typename ...Args>
	void setMousePositionCallback(R(*mouse_position_callback)(Args...));

	template<typename R, typename ...Args>
	void setWindowResizeCallback(R(*window_resize_callback)(Args...));

	template<typename R, typename ...Args>
	void setMouseScrollCallback(R(*mouse_scroll_callback)(Args...));

	void swapBuffers() const;
	bool shouldClose() const;
	void pollEvents() const;

	void deinit();
};

} // namespace rw_cube

#include "Window.tpp"

#endif