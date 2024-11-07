
#include "graphics_lib.h"
#include "modern_graphics_lib.h"
#include "shape_drawing_lib.h"

#include "ModernGraphicsAdapter.h"
#include "ModernGraphicsClassAdapter.h"

// Пространство имен приложения (доступно для модификации)

#pragma once
namespace app
{
	void PaintPicture(shape_drawing_lib::CCanvasPainter& painter)
	{
		using namespace shape_drawing_lib;

		const CTriangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 }, 0xFFBBCC);
		const CRectangle rectangle({ 30, 40 }, 18, 24);

		painter.Draw(triangle);
		painter.Draw(rectangle);
	}

	void PaintPictureOnCanvas()
	{
		graphics_lib::CCanvas simpleCanvas;
		shape_drawing_lib::CCanvasPainter painter(simpleCanvas);
			
		PaintPicture(painter);
	}

	void PaintPictureOnModernGraphicsRenderer()
	{
		modern_graphics_lib::CModernGraphicsRenderer renderer(std::cout);
		ModernGraphicsAdapter modernGraphicsAdapter(renderer);
		shape_drawing_lib::CCanvasPainter painter(modernGraphicsAdapter);

		renderer.BeginDraw();

		PaintPicture(painter);

		renderer.EndDraw(); // Имеет ли смысл вызывать внутри адаптера
	}

	void PaintPictureOnModernGraphicsRenderer2()
	{
		ModernGraphicsClassAdapter modernGraphicsClassAdapter(std::cout);
		shape_drawing_lib::CCanvasPainter painter(modernGraphicsClassAdapter);

		modernGraphicsClassAdapter.BeginDraw();

		PaintPicture(painter);

		modernGraphicsClassAdapter.EndDraw();
	}
}