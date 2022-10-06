#include "Renderer.h"

#include "Walnut/Random.h"


void Renderer::Render()
{
	for (uint32_t i = 0; i < m_FinalImage->GetWidth() * m_FinalImage->GetHeight(); i++)
		m_ImageData[i] = Walnut::Random::UInt() | 0xFF000000;

	m_FinalImage->SetData(m_ImageData);
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;
		m_FinalImage->Resize(width, height); // This function checks if it even needs resizing for us
	}
	else
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);

	delete[] m_ImageData;
	m_ImageData = new uint32_t[static_cast<uint64_t>(width) * height];
}