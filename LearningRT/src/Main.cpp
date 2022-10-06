#include "Renderer.h"

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h" // EntryPoint must be included after Application
#include "Walnut/Timer.h"

class RaytracingLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		// Settings window
		ImGui::Begin("Settings");
		if (ImGui::Button("Render Once") || m_KeepRendering)
			Render();
		ImGui::Checkbox("Keep Rendering", &m_KeepRendering);
		ImGui::Text("Last render took %.3fms", m_LastRenderTime);
		ImGui::End();

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		m_ViewportWidth  = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
		m_ViewportHeight = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

		const auto image = m_Renderer.GetFinalImage();
		if (image != nullptr)
			ImGui::Image(image->GetDescriptorSet(), { static_cast<float>(image->GetWidth()), static_cast<float>(image->GetHeight()) });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render()
	{
		Walnut::Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render();

		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	// UI State
	bool m_KeepRendering = false;
	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;

	// Renderer
	Renderer m_Renderer;

	// Timing
	float m_LastRenderTime = 0;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Raytracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<RaytracingLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}
