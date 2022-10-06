#pragma once

#include <Walnut/Image.h>

#include <memory>

class Renderer
{
public:
	Renderer() = default;

	void Render();
	void OnResize(uint32_t width, uint32_t height);
	[[nodiscard]] std::shared_ptr<Walnut::Image> GetFinalImage() const;
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};

inline std::shared_ptr<Walnut::Image> Renderer::GetFinalImage() const
{
	return m_FinalImage;
}
