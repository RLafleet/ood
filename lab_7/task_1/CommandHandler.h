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
		m_menu.AddItem(
			"InsertShape",
			"Usage: InsertShape <type> <left> <top> <width> <height> <outlineColor> <fillColor>. Adds a new shape to the slide.",
			[&](std::istringstream& params) { InsertShape(params); });

		m_menu.AddItem(
			"RemoveShape",
			"Usage: RemoveShape <index>. Deletes the shape at the specified index from the slide.",
			[&](std::istringstream& params) { RemoveShape(params); });

		m_menu.AddItem(
			"ListShapes",
			"Displays a list of all shapes currently on the slide.",
			[&](std::istringstream& params) { ListShapes(); });

		m_menu.AddItem(
			"SetOutlineStyle",
			"Usage: SetOutlineStyle <index> <color> <enabled>. Updates the outline style of the shape at the specified index.",
			[&](std::istringstream& params) { SetOutlineStyle(params); });

		m_menu.AddItem(
			"SetFillStyle",
			"Usage: SetFillStyle <index> <color> <enabled>. Updates the fill style of the shape at the specified index.",
			[&](std::istringstream& params) { SetFillStyle(params); });

		m_menu.AddItem(
			"ResizeShape",
			"Usage: ResizeShape <index> <new_width> <new_height>. Changes the size of the shape at the specified index.",
			[&](std::istringstream& params) { ResizeShape(params); });

		m_menu.AddItem(
			"Draw",
			"Renders the current slide onto the canvas.",
			[&](std::istringstream& params) { Draw(); });

		m_menu.AddItem("GroupShapes",
			"Usage: GroupShapes <index1> <index2> ... Combines selected shapes into a single group.",
			[&](std::istringstream& params) { GroupShapes(params); });

		m_menu.AddItem(
			"Help",
			"Displays a list of available commands and their usage.",
			[&](std::istringstream& params) { Help(); });

		m_menu.AddItem(
			"Exit",
			"Terminates the program.",
			[&](std::istringstream& params) { Exit(); });
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
		std::cout << "==============================\n";
		std::cout << "         List of Shapes       \n";
		std::cout << "==============================\n";

		for (size_t i = 0; i < m_slide->GetShapesCount(); ++i)
		{
			const auto shape = m_slide->GetShapeAtIndex(i);
			auto frame = shape->GetFrame();

			std::cout << "Shape " << i + 1 << ":\n";  
			std::cout << "  Position: (" << frame.left << ", " << frame.top << ")\n";
			std::cout << "  Size: Width = " << frame.width << ", Height = " << frame.height << "\n";

			IStyle& outlineStyle = shape->GetOutlineStyle();
			if (outlineStyle.IsEnabled() && outlineStyle.GetColor().has_value())
			{
				std::cout << "  Outline: Enabled\n";
				std::cout << "    Color: #" << std::hex << outlineStyle.GetColor().value() << std::dec << "\n";
			}
			else
			{
				std::cout << "  Outline: Disabled\n";
			}

			IStyle& fillStyle = shape->GetFillStyle();
			if (fillStyle.IsEnabled() && fillStyle.GetColor().has_value())
			{
				std::cout << "  Fill: Enabled\n";
				std::cout << "    Color: #" << std::hex << fillStyle.GetColor().value() << std::dec << "\n";
			}
			else
			{
				std::cout << "  Fill: Disabled\n";
			}

			std::cout << "------------------------------\n";
		}

		std::cout << "==============================\n";
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


	CMenu m_menu;
	std::unique_ptr<ISlide> m_slide;
	std::unique_ptr<ICanvas> m_canvas;
	std::unique_ptr<IShapeFactory> m_factory;
};

#endif //COMMANDHANDLER_H_