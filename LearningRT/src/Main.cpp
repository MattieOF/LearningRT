#include "Renderer.h"

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h" // EntryPoint must be included after Application
#include "Walnut/Timer.h"
#include "Walnut/Input/Input.h"

class RaytracingLayer : public Walnut::Layer
{
public:
	virtual void OnUpdate(float ts) override
	{
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S))
			m_Renderer.rayOrigin.y += m_MoveSpeed * ts;
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W))
			m_Renderer.rayOrigin.y -= m_MoveSpeed * ts;
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A))
			m_Renderer.rayOrigin.x += m_MoveSpeed * ts;
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D))
			m_Renderer.rayOrigin.x -= m_MoveSpeed * ts;
	}

	virtual void OnUIRender() override
	{
		// Settings window
		ImGui::Begin("Settings");
		if (ImGui::Button("Render Once") || m_KeepRendering)
			Render();
		ImGui::Checkbox("Keep Rendering", &m_KeepRendering);
		ImGui::DragFloat3("Camera Position", &m_Renderer.rayOrigin[0], 0.1f);
		ImGui::DragFloat("Sphere Radius", &m_Renderer.radius, 0.05f);
		if (ImGui::Button("Reset"))
		{
			m_Renderer.rayOrigin = glm::vec3(0, 0, 2);
			m_Renderer.radius = 0.5f;
		}
		ImGui::DragFloat("Move Speed", &m_MoveSpeed, 0.05f);
		ImGui::Text("Viewport size is %ix%i (total pixels: %i)", m_ViewportWidth, m_ViewportHeight, m_ViewportWidth * m_ViewportHeight);
		ImGui::Text("Last render took %.3fms", m_LastRenderTime);
		ImGui::End();

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		m_ViewportWidth  = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
		m_ViewportHeight = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

		const auto image = m_Renderer.GetFinalImage();
		if (image != nullptr)
			ImGui::Image(image->GetDescriptorSet(), { static_cast<float>(image->GetWidth()), static_cast<float>(image->GetHeight()) },
				ImVec2(0, 1), ImVec2(1, 0));

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

	// Controls
	float m_MoveSpeed = 2;

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
