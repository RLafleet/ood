#ifndef GRAPHICS_LIB_H
#define GRAPHICS_LIB_H
#include <iostream>
#include <cstdint>
#include <iomanip>

namespace graphics_lib
{
	// Холст для рисования
	class ICanvas
	{
	public:
		virtual void SetColor(uint32_t rgbColor) = 0;

		// Ставит "перо" в точку x, y
		virtual void MoveTo(int x, int y) = 0;

		// Рисует линию с текущей позиции, передвигая перо в точку x,y
		virtual void LineTo(int x, int y) = 0;

		virtual ~ICanvas() = default;
	};

	// Реализация холста для рисования
	class CCanvas : public ICanvas
	{
	public:
		void SetColor(const uint32_t rgbColor) override
		{
			std::cout << "SetColor on #" << std::hex << std::setfill('0') << rgbColor << std::dec << std::endl;
		}

		void MoveTo(const int x, const int y) override
		{
			std::cout << "MoveTo (" << x << ", " << y << ")" << std::endl;
		}

		void LineTo(const int x, const int y) override
		{
			std::cout << "LineTo (" << x << ", " << y << ")" << std::endl;
		}
	};
}

#endif //GRAPHICS_LIB_H