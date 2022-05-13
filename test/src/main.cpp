#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include <euler_spiral.hpp>


int main(int argc, char const *argv[]) {

	int w = 1280;
	int h = 720;
	std::string title = "Euler Spiral demo";
	initImgui(w, h, title);

	// Our state
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4 edge_color = ImVec4(255.0/255.0, 255.0/255.0f, 255.0f/255.0, 1.00f);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static float length = 300.0;
		static float initCurv = 1;
		static float dCurv = 1;
		static float thetadeg = 0;
		std::vector<es::SpiralPoint> pos;
		for (double s = 0.0; s < length; s += 1.0) {
			es::SpiralPoint p;
			p = es::getEndPoint(s, dCurv*0.0001, initCurv*0.001, w/2.0, h/2.0, deg2rad(thetadeg));
			pos.push_back(p);
			// std::cout << p.x << " " << p.y << std::endl;
		}


		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoBackground;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);

			ImGui::Begin("Euler Spiral", nullptr, window_flags);
			ImGui::SliderFloat("Length", &length, 1, 1000);
			ImGui::SliderFloat("Init curvature", &initCurv, -10, 10);
			ImGui::SliderFloat("d curvature (x1000)", &dCurv, -10, 10);
			ImGui::SliderFloat("Initial Theta (deg)", &thetadeg, 0, 360);

			ImDrawList *draw_list = ImGui::GetWindowDrawList();

			for (size_t i = 0; i < pos.size()-1; i++) {
				draw_list->AddLine(
						ImVec2(pos[i].x, pos[i].y), 
						ImVec2(pos[i+1].x, pos[i+1].y), 
						ImColor(edge_color)
				);
			}

			ImGui::End();
		}


		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}
