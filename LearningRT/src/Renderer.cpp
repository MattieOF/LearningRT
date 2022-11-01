#include "Renderer.h"

#include "Walnut/Input/Input.h"

#include "Utils.h"

void Renderer::Render()
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { static_cast<float>(x) / static_cast<float>(m_FinalImage->GetWidth()), static_cast<float>(y) / static_cast<float>(m_FinalImage->GetHeight()) };
			coord = coord * 2.0f - 1.0f; // Remap from 0->1 to -1->1
			glm::vec4 color = PerPixel(coord);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::Vec4ToABGRInt(color);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
	glm::vec3 rayDirection(coord.x, coord.y, 1.0f);

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;
	
	// >= 0 is a hit, < 0 is a miss.
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0)
	{
		// We hit the circle, color with a basic UV representation so it looks cool
		float r = (coord.x + 1) / 2;
		float g = (coord.y + 1) / 2;
		return {r, g, 0, 1};
	}

	return {0, 0, 0, 1};
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return; // Resize is unnessesary, return early so we don't bother resizing or recreating image data. 
		m_FinalImage->Resize(width, height);
	}
	else
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);

	// If the image resized, or if it never existed in the first place, we need to reallocate the CPU buffer for the image.
	delete[] m_ImageData;
	m_ImageData = new uint32_t[static_cast<uint64_t>(width) * height];
}
