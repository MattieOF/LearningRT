#pragma once

#include <glm/glm.hpp>

namespace Utils
{
	static uint32_t Vec4ToABGRInt(const glm::vec4& vec)
	{
		uint8_t r = static_cast<uint8_t>(vec.r * 255.0f);
		uint8_t g = static_cast<uint8_t>(vec.g * 255.0f);
		uint8_t b = static_cast<uint8_t>(vec.b * 255.0f);
		uint8_t a = static_cast<uint8_t>(vec.a * 255.0f);

		return (a << 24) | (b << 16) | (g << 8) | r;
	}
}
