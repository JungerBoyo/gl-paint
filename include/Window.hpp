#ifndef TEMPLATE_PROJECT_WINDOW_HPP
#define TEMPLATE_PROJECT_WINDOW_HPP

#include <memory>
#include <string_view>

namespace template_project {

struct Window {
	struct WinNative;
	std::shared_ptr<WinNative> win_handle_;

	Window(std::string_view title, int w, int h,
		   void (*win_error_callback)(int, const char *),
		   void (*gl_error_callback)(std::uint32_t, std::uint32_t,
									 std::uint32_t, std::uint32_t, int,
									 const char *, const void *));
	
	[[nodiscard]] void* native();
	[[nodiscard]] std::pair<int, int> size() const;
	[[nodiscard]] float time() const;

	void setViewport(int w, int h) const;
	void setWinUserDataPointer(void* ptr);

	template<typename R, typename ...Args>
	void setKeyCallback(R(*key_callback)(Args...));

	template<typename R, typename ...Args>
	void setMousePositionCallback(R(*mouse_position_callback)(Args...));

	void swapBuffers() const;
	bool shouldClose() const;
	void pollEvents() const;

	void deinit();
};

} // namespace rw_cube

#include "Window.tpp"

#endif