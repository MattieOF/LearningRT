#include "Renderer.h"

#include "Walnut/Random.h"


void Renderer::Render()
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { static_cast<float>(x) / static_cast<float>(m_FinalImage->GetWidth()), static_cast<float>(y) / static_cast<float>(m_FinalImage->GetHeight()) };
			coord = coord * 2.0f - 1.0f; // Remap from 0->1 to -1->1
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	float radius = 0.5f;
	glm::vec3 rayOrigin(0, 0, 2);
	glm::vec3 rayDirection(coord.x, coord.y, 1.0f);

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;
	
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0)
	{
		uint8_t r = static_cast<uint8_t>((coord.x + 1) / 2 * 255.0f);
		uint8_t g = static_cast<uint8_t>((coord.y + 1) / 2 * 255.0f);
		return 0xFF000000 | g << 8 | r;
	}
	return 0xFF000000;
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
