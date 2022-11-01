#pragma once

#include <Walnut/Image.h>

#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Renderer
{
public:
	Renderer() = default;

	void Render();
	void OnResize(uint32_t width, uint32_t height);
	[[nodiscard]] std::shared_ptr<Walnut::Image> GetFinalImage() const;

	float radius = 0.5f;
	glm::vec3 rayOrigin = glm::vec3(0, 0, 2);
private:
	glm::vec4 PerPixel(glm::vec2 coord);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};

inline std::shared_ptr<Walnut::Image> Renderer::GetFinalImage() const
{
	return m_FinalImage;
}
