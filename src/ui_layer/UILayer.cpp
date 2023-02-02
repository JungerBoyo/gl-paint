#include <UILayer.hpp>

#include <imgui.h>
#include <imgui_bindings/imgui_impl_glfw.h>
#include <imgui_bindings/imgui_impl_opengl3.h>

#include <config.hpp>

using namespace glpaint;

UILayer::UILayer(void *window_handle, EventQueue &event_queue)
		: event_queue_(event_queue) {
	// initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(window_handle), true);
	ImGui_ImplOpenGL3_Init(cmake::glsl_version.data());
}

UILayer::InUIViewportDims UILayer::update(u32 viewport_texture_id,
																					i32 viewport_width,
																					i32 viewport_height,
																					std::span<const u32> layer_view_ids) {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	InUIViewportDims result{};

	// here goes UI
	// docking window
	const auto *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	static constexpr auto docking_window_flags{
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar};
	ImGui::Begin("##docking-space", nullptr, docking_window_flags);

	auto &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("##Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);
	}
	// render viewport texture
	const auto image_size = ImVec2{static_cast<f32>(viewport_width),
																 static_cast<f32>(viewport_height)};
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0F);
	if (ImGui::Begin("##viewport")) {
		const auto win_padding =
				ImVec2{15.F, 15.F}; // slightly bigger than viewport
		// ImGui::SetWindowSize({
		//     image_size.x + win_padding.x,
		//     image_size.y + win_padding.y
		// });
		// ImGui::SetCursorPos({.5F * win_padding.x, .5F * win_padding.y});
		const auto vp_ui_pos = ImGui::GetWindowPos();
		const auto vp_ui_size = ImGui::GetWindowSize();
		result.left_margin_offset = vp_ui_pos.x;
		result.top_margin_offset = vp_ui_pos.y;
		result.extent_x = vp_ui_size.x;
		result.extent_y = vp_ui_size.y;

		/// DRAWING TEST BEGIN

		static ImVector<ImVec2> points;
		static bool adding_line{false};

		auto p0 = ImGui::GetCursorScreenPos();
		auto size = ImVec2{
				static_cast<f32>(viewport_width),
				static_cast<f32>(viewport_height),
		};
		auto p1 = ImVec2(p0.x + size.x, p0.y + size.y);

		auto &io = ImGui::GetIO();
		auto *draw_list = ImGui::GetWindowDrawList();

		draw_list->AddRectFilled(p0, p1, IM_COL32(50, 50, 50, 255));

		ImGui::InvisibleButton("##canvas", size,
													 ImGuiButtonFlags_MouseButtonLeft |
															 ImGuiButtonFlags_MouseButtonRight);
		const bool is_hovered = ImGui::IsItemHovered();
		const bool is_active = ImGui::IsItemActive();
		const auto mouse_pos_in_canvas =
				ImVec2{io.MousePos.x - p0.x, io.MousePos.y - p0.y};

		if (is_hovered && !adding_line &&
				ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			points.push_back(mouse_pos_in_canvas);
			points.push_back(mouse_pos_in_canvas);
			adding_line = true;
		}
		if (adding_line) {
			points.back() = mouse_pos_in_canvas;
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				adding_line = false;
			}
		}

		draw_list->PushClipRect(p0, p1, true);

		constexpr f32 GRID_STEP{64.F};
		for (f32 x = GRID_STEP; x < size.x; x += GRID_STEP) {
			draw_list->AddLine(ImVec2{p0.x + x, p0.y}, ImVec2{p0.x + x, p1.y},
												 IM_COL32(200, 200, 200, 40));
		}
		for (f32 y = GRID_STEP; y < size.y; y += GRID_STEP) {
			draw_list->AddLine(ImVec2{p0.x, p0.y + y}, ImVec2{p1.x, p0.y + y},
												 IM_COL32(200, 200, 200, 40));
		}

		draw_list->AddImage(
				(void *)(intptr_t)viewport_texture_id, // NOLINT, interfacing with imgui
				p0, p1, {1.F, 0.F}, {0.F, 1.F});

		for (i32 i{0}; i < points.Size; i += 2) {
			draw_list->AddLine({p0.x + points[i].x, p0.y + points[i].y},
												 {p0.x + points[i + 1].x, p0.y + points[i + 1].y},
												 IM_COL32(255, 255, 0, 255), 2.F);
		}
		draw_list->PopClipRect();

		// ImGui::Image(
		//     (void *)(intptr_t)viewport_texture_id, // NOLINT, interfacing with
		//     imgui
		//     {
		//         image_size.x,
		//         image_size.y
		//     },
		//     {1.F, 0.F},
		//     {0.F, 1.F}
		// );

		/// DRAWING TEST END

		ImGui::End();
	}
	ImGui::PopStyleVar();

	if (ImGui::Begin("##layer-tumbnails", nullptr,
									 ImGuiWindowFlags_NoNavFocus |
											 ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::Button("Add new layer")) {
			event_queue_.emplace(std::make_unique<LayerAddEvent>());
		}

		ImGui::Separator();

		constexpr f32 tumbnail_scaling{.20F};
		i32 i{0};
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.F, 0.F, 0.F, 0.F));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.F, 0.F, 0.F, 0.F));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.F, 0.F, 0.F, 0.F));
		for (const auto view_id : layer_view_ids) {
			ImGui::PushID(i);

			ImGui::ImageButton(
					(void *)(intptr_t)view_id, // NOLINT, interfacing with imgui
					{image_size.x * tumbnail_scaling, image_size.y * tumbnail_scaling},
					{1.F, 0.F}, {0.F, 1.F});

			static constexpr std::string_view payload_type = "image_view_tumbnail";

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload(payload_type.data(), &i, sizeof(i32));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload *payload =
								ImGui::AcceptDragDropPayload(payload_type.data());
						payload != nullptr) {
					const auto image_view_index =
							*static_cast<const i32 *>(payload->Data);

					event_queue_.emplace(
							std::make_unique<ImageLayerSwapEvent>(image_view_index, i));
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::PopID();
			++i;
		}
		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	ImGui::End(); // docking-space

	ImGui::Render();

	ImGui::GetDrawData();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return result;
}

void UILayer::deinit() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}