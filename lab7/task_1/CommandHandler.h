#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <sstream>
#include <iostream>
#include "CMenu.h"
#include "ISlide.h"
#include "ICanvas.h"
#include "IShapeFactory.h"

class CommandHandler
{
public:
	CommandHandler(std::unique_ptr<ISlide> slide, std::unique_ptr<ICanvas> canvas,
		std::unique_ptr<IShapeFactory> factory)
		: m_slide(std::move(slide)), m_canvas(std::move(canvas)), m_factory(std::move(factory))
	{
		m_menu.AddItem("InsertShape", "Usage: InsertShape <type> <left> <top> <width> <height> <outlineColor> <fillColor>. Adds a shape.",
			[&](std::istringstream& params) { InsertShape(params); });
		m_menu.AddItem("RemoveShape", "Usage: RemoveShape <index>. Removes a shape by index.",
			[&](std::istringstream& params) { RemoveShape(params); });
		m_menu.AddItem("ListShapes", "Lists all shapes on the slide.",
			[&](std::istringstream& params) { ListShapes(); });
		m_menu.AddItem("SetOutlineStyle",
			"Usage: SetOutlineStyle <index> <color> <enabled>. Sets outline style for a shape.",
			[&](std::istringstream& params) { SetOutlineStyle(params); });
		m_menu.AddItem("SetFillStyle", "Usage: SetFillStyle <index> <color> <enabled>. Sets fill style for a shape.",
			[&](std::istringstream& params) { SetFillStyle(params); });
		m_menu.AddItem("ResizeShape", "Usage: ResizeShape <index> <new_width> <new_height>. Resizes a shape.",
			[&](std::istringstream& params) { ResizeShape(params); });
		m_menu.AddItem("Draw", "Draws the slide on the canvas.", [&](std::istringstream& params) { Draw(); });
		m_menu.AddItem("GroupShapes", "Usage: GroupShapes <index1> <index2> ... Groups selected shapes.",
			[&](std::istringstream& params) { GroupShapes(params); });
		m_menu.AddItem("Help", "Shows command list.", [&](std::istringstream& params) { Help(); });
		m_menu.AddItem("Exit", "Exits the program.", [&](std::istringstream& params) { Exit(); });
	}

	void Run()
	{
		m_menu.Run();
	}

private:
	void InsertShape(std::istringstream& params) const
	{
		std::string shapeDescription;
		std::getline(params, shapeDescription);
		try
		{
			auto shape = m_factory->CreateShape(shapeDescription);
			m_slide->InsertShape(std::move(shape), m_slide->GetShapesCount());
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error inserting shape: " << e.what() << std::endl;
		}
	}

	void RemoveShape(std::istringstream& params) const
	{
		size_t index;
		params >> index;
		if (index < m_slide->GetShapesCount())
		{
			m_slide->RemoveShapeAtIndex(index);
		}
		else
		{
			std::cerr << "Invalid shape index." << std::endl;
		}
	}

	void ListShapes() const
	{
		for (size_t i = 0; i < m_slide->GetShapesCount(); ++i)
		{
			const auto shape = m_slide->GetShapeAtIndex(i);

			auto frame = shape->GetFrame();
			std::cout << "Shape " << i << ":" << std::endl;
			std::cout << "  Position: (" << frame.left << ", " << frame.top << ")" << std::endl;
			std::cout << "  Size: Width = " << frame.width << ", Height = " << frame.height << std::endl;

			IStyle& outlineStyle = shape->GetOutlineStyle();
			if (outlineStyle.IsEnabled() && outlineStyle.GetColor().has_value())
			{
				std::cout << "  Outline: Enabled" << std::endl;
				std::cout << "    Color: #" << std::hex << outlineStyle.GetColor().value() << std::dec << std::endl;
			}
			else
			{
				std::cout << "  Outline: Disabled" << std::endl;
			}

			IStyle& fillStyle = shape->GetFillStyle();
			if (fillStyle.IsEnabled() && fillStyle.GetColor().has_value())
			{
				std::cout << "  Fill: Enabled" << std::endl;
				std::cout << "    Color: #" << std::hex << fillStyle.GetColor().value() << std::dec << std::endl;
			}
			else
			{
				std::cout << "  Fill: Disabled" << std::endl;
			}

			std::cout << std::endl;
		}
	}


	void SetOutlineStyle(std::istringstream& params) const
	{
		size_t index;
		std::string colorStr;
		bool enabled;
		params >> index >> colorStr >> enabled;

		if (index < m_slide->GetShapesCount())
		{
			auto shape = m_slide->GetShapeAtIndex(index);
			IStyle& outlineStyle = shape->GetOutlineStyle();
			RGBAColor color;
			if (ParseColor(colorStr, color))
			{
				outlineStyle.SetColor(color);
				outlineStyle.SetIsEnabled(enabled);
			}
			else
			{
				std::cerr << "Invalid color format." << std::endl;
			}
		}
		else
		{
			std::cerr << "Invalid shape index." << std::endl;
		}
	}

	void SetFillStyle(std::istringstream& params) const
	{
		size_t index;
		std::string colorStr;
		bool enabled;
		params >> index >> colorStr >> enabled;

		if (index < m_slide->GetShapesCount())
		{
			auto shape = m_slide->GetShapeAtIndex(index);
			IStyle& fillStyle = shape->GetFillStyle();
			RGBAColor color;
			if (ParseColor(colorStr, color))
			{
				fillStyle.SetColor(color);
				fillStyle.SetIsEnabled(enabled);
			}
			else
			{
				std::cerr << "Invalid color format." << std::endl;
			}
		}
		else
		{
			std::cerr << "Invalid shape index." << std::endl;
		}
	}

	void ResizeShape(std::istringstream& params) const
	{
		size_t index;
		double width, height;
		params >> index >> width >> height;

		if (index < m_slide->GetShapesCount())
		{
			auto shape = m_slide->GetShapeAtIndex(index);
			auto frame = shape->GetFrame();
			frame.width = width;
			frame.height = height;
			shape->SetFrame(frame);
		}
		else
		{
			std::cerr << "Invalid shape index." << std::endl;
		}
	}

	void Draw() const
	{
		m_slide->Draw(*m_canvas);
	}

	void GroupShapes(std::istringstream& params) const
	{
		std::vector<size_t> indexes;
		size_t index;
		while (params >> index)
		{
			if (index < m_slide->GetShapesCount())
			{
				indexes.push_back(index);
			}
			else
			{
				std::cerr << "Invalid shape index." << std::endl;
				return;
			}
		}

		if (!indexes.empty())
		{
			m_slide->GroupShapesAtIndexex(indexes);
		}
	}

	void Help()
	{
		m_menu.ShowInstructions();
	}

	void Exit()
	{
		m_menu.Exit();
	}

	static bool ParseColor(std::string colorStr, RGBAColor& color)
	{
		if (colorStr.size() != 7 || colorStr[0] != '#')
		{
			return false;
		}
		std::stringstream ss;
		ss << std::hex << colorStr.append("FF").substr(1);

		if (!(ss >> color))
		{
			return false;
		}

		return ss.eof();
	}

	CMenu m_menu;
	std::unique_ptr<ISlide> m_slide;
	std::unique_ptr<ICanvas> m_canvas;
	std::unique_ptr<IShapeFactory> m_factory;
};

#endif //COMMANDHANDLER_H_