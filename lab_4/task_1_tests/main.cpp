﻿#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include "../task_1/CRectangle.h"
#include "../task_1/CEllipse.h"
#include "../task_1/CTriangle.h"
#include "../task_1/RegularPolygon.h"
#include "../task_1/ShapeFactory.h"
#include "../task_1/PictureDraft.h"
#include "../task_1/IDesigner.h"
#include "../task_1/Painter.h"
#include "../task_1/Client.h"
#include "../task_1/Point.h"
#include "../task_1/CDesigner.h"

class MockCanvas : public gfx::ICanvas
{
public:
	std::vector<std::string> actions;

	void SetColor(uint32_t color) override
	{
		actions.push_back("SetColor: " + std::to_string(color));
	}

	void MoveTo(double x, double y) override
	{
		actions.push_back("MoveTo: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
	}

	void DrawEllipse(double cx, double cy, double rx, double ry) override
	{
		actions.push_back("DrawEllipse: center(" + std::to_string(cx) + ", " + std::to_string(cy) + ") radiusX=" + std::to_string(rx) + ", radiusY=" + std::to_string(ry));
	}

	void DrawText(double left, double top, double fontSize, const std::string& text) override
	{
		actions.push_back("DrawText: position(" + std::to_string(left) + ", " + std::to_string(top) + ") fontSize=" + std::to_string(fontSize) + " text=\"" + text + "\"");
	}

	void DrawLine(double fromX, double fromY, double toX, double toY) override
	{
		actions.push_back("DrawLine: (" + std::to_string(fromX) + ", " + std::to_string(fromY) + ") to (" + std::to_string(toX) + ", " + std::to_string(toY) + ")");
	}
};

void AssertEqualPoint(const Point& expected, const Point& actual)
{
	REQUIRE(expected.x == actual.x);
	REQUIRE(expected.y == actual.y);
}

TEST_CASE("CRectangle: create success") 
{
	Color color = Color::BLACK;
	Point leftTop(10, 20);
	double width = 30;
	double height = 50;

	CRectangle rectangle(color, leftTop, width, height);

	REQUIRE(rectangle.GetColor() == color);
	AssertEqualPoint(leftTop, rectangle.GetLeftTop());
	REQUIRE(rectangle.GetWidth() == width);
	REQUIRE(rectangle.GetHeight() == height);
}

TEST_CASE("CRectangle: draw success") 
{
	Color color = Color::BLACK;
	Point leftTop(10, 20);
	double width = 30;
	double height = 50;

	CRectangle rectangle(color, leftTop, width, height);
	MockCanvas mockCanvas;

	rectangle.Draw(mockCanvas);

	REQUIRE(mockCanvas.actions == std::vector<std::string>
	{
		"SetColor: " + std::to_string(convertColorToHEX(color)),
			"DrawLine: (10.000000, 20.000000) to (40.000000, 20.000000)",
			"DrawLine: (40.000000, 20.000000) to (40.000000, 70.000000)",
			"DrawLine: (40.000000, 70.000000) to (10.000000, 70.000000)",
			"DrawLine: (10.000000, 70.000000) to (10.000000, 20.000000)"
	});
}

TEST_CASE("CEllipse: create success") 
{
	Color color = Color::BLACK;
	Point center(50, 50);
	double horizontalRadius = 30;
	double verticalRadius = 20;

	CEllipse ellipse(color, center, horizontalRadius, verticalRadius);

	REQUIRE(ellipse.GetColor() == color);
	AssertEqualPoint(center, ellipse.GetCenter());
	REQUIRE(ellipse.GetHorizontalRadius() == horizontalRadius);
	REQUIRE(ellipse.GetVerticalRadius() == verticalRadius);
}

TEST_CASE("CEllipse: draw success") 
{
	Color color = Color::BLACK;
	Point center(50, 50);
	double horizontalRadius = 30;
	double verticalRadius = 20;

	CEllipse ellipse(color, center, horizontalRadius, verticalRadius);
	MockCanvas mockCanvas;

	ellipse.Draw(mockCanvas);

	REQUIRE(mockCanvas.actions == std::vector<std::string>
	{
		"SetColor: " + std::to_string(convertColorToHEX(color)),
			"DrawEllipse: center(50.000000, 50.000000) radiusX=30.000000, radiusY=20.000000"
	});
}

TEST_CASE("CTriangle: create success") 
{
	Color color = Color::BLACK;
	Point point1(10, 20);
	Point point2(40, 20);
	Point point3(25, 50);

	CTriangle triangle(color, point1, point2, point3);

	REQUIRE(triangle.GetColor() == color);
	AssertEqualPoint(point1, triangle.GetPoint1());
	AssertEqualPoint(point2, triangle.GetPoint2());
	AssertEqualPoint(point3, triangle.GetPoint3());
}

TEST_CASE("CTriangle: draw success") 
{
	Color color = Color::BLACK;
	Point point1(10, 20);
	Point point2(40, 20);
	Point point3(25, 50);

	CTriangle triangle(color, point1, point2, point3);
	MockCanvas mockCanvas;

	triangle.Draw(mockCanvas);

	REQUIRE(mockCanvas.actions == std::vector<std::string>{
		"SetColor: " + std::to_string(convertColorToHEX(color)),
			"DrawLine: (10.000000, 20.000000) to (40.000000, 20.000000)",
			"DrawLine: (40.000000, 20.000000) to (25.000000, 50.000000)",
			"DrawLine: (25.000000, 50.000000) to (10.000000, 20.000000)"
	});
}

TEST_CASE("RegularPolygon: create success") {
	Color color = Color::BLACK;
	Point center(100, 100);
	int vertexCount = 5;
	double radius = 30;

	RegularPolygon polygon(color, center, vertexCount, radius);

	REQUIRE(polygon.GetColor() == color);
	AssertEqualPoint(center, polygon.GetCenter());
	REQUIRE(polygon.GetRadius() == radius);
}

TEST_CASE("RegularPolygon: draw success") {
	Color color = Color::BLACK;
	Point center(100, 100);
	int vertexCount = 5;
	double radius = 30;

	RegularPolygon polygon(color, center, vertexCount, radius);
	MockCanvas mockCanvas;

	polygon.Draw(mockCanvas);

	REQUIRE(mockCanvas.actions[0] == "SetColor: " + std::to_string(convertColorToHEX(color)));
	REQUIRE(mockCanvas.actions.size() == vertexCount + 2);
}

TEST_CASE("ShapeFactory: create shapes success") 
{
	ShapeFactory factory;

	auto rectangle = factory.CreateShape("blue rectangle 10 20 30 40");
	REQUIRE(dynamic_cast<CRectangle*>(rectangle.get()) != nullptr);

	auto ellipse = factory.CreateShape("red ellipse 50 50 30 20");
	REQUIRE(dynamic_cast<CEllipse*>(ellipse.get()) != nullptr);

	auto triangle = factory.CreateShape("blue triangle 10 20 40 20 25 50");
	REQUIRE(dynamic_cast<CTriangle*>(triangle.get()) != nullptr);

	auto polygon = factory.CreateShape("green regularPolygon 100 100 5 30");
	REQUIRE(dynamic_cast<RegularPolygon*>(polygon.get()) != nullptr);
}

TEST_CASE("PictureDraft: add shapes and get shapes") 
{
	PictureDraft draft;
	ShapeFactory factory;

	auto rectangle = factory.CreateShape("black rectangle 10 20 30 40");
	draft.AddShape(std::move(rectangle));

	auto ellipse = factory.CreateShape("red ellipse 50 50 30 20");
	draft.AddShape(std::move(ellipse));
}

TEST_CASE("Painter: draw picture success") 
{
	MockCanvas mockCanvas;
	PictureDraft draft;
	ShapeFactory factory;
	Painter painter;

	auto rectangle = factory.CreateShape("black rectangle 10 20 30 40");
	draft.AddShape(std::move(rectangle));

	auto ellipse = factory.CreateShape("red ellipse 50 50 30 20");
	draft.AddShape(std::move(ellipse));

	painter.DrawPicture(draft, mockCanvas);

	REQUIRE(mockCanvas.actions.size() > 0);
}

TEST_CASE("Client: create and paint picture") 
{
	MockCanvas mockCanvas;
	ShapeFactory factory;
	Painter painter;
	CDesigner designer(factory);
	Client client(designer);

	std::stringstream input("black rectangle 10 20 30 40\nred ellipse 50 50 30 20\n");
	auto draft = designer.CreateDraft(input);

	painter.DrawPicture(draft, mockCanvas);

	REQUIRE(mockCanvas.actions.size() > 0);
}
