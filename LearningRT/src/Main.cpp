#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"

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
		ImGui::End();

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		m_ViewportWidth  = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
		m_ViewportHeight = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);
		if (m_Image != nullptr)
			ImGui::Image(m_Image->GetDescriptorSet(), { static_cast<float>(m_Image->GetWidth()), static_cast<float>(m_Image->GetHeight()) });
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render()
	{
		if (m_Image == nullptr
			|| m_Image->GetWidth() != m_ViewportWidth
			|| m_Image->GetHeight() != m_ViewportHeight)
		{
			m_Image = std::make_shared<Walnut::Image>(m_ViewportWidth, m_ViewportHeight, Walnut::ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
		}

		for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++)
			m_ImageData[i] = Walnut::Random::UInt() | 0xFF000000;

		m_Image->SetData(m_ImageData);
	}

private:
	bool m_KeepRendering = false;
	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;

	std::shared_ptr<Walnut::Image> m_Image;
	uint32_t* m_ImageData = nullptr;
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