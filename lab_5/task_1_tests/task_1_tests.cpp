#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include "../AbstractCommand.cpp"
#include "../InsertParagraphCommand.h"
#include "../InsertParagraphCommand.cpp"
#include "../ReplaceTextCommand.h"
#include "../ReplaceTextCommand.cpp"
#include "../DeleteCommand.h"
#include "../DeleteCommand.cpp"
#include "../SetTitleCommand.h"
#include "../SetTitleCommand.cpp"
#include "../SaveCommand.h"
#include "../SaveCommand.cpp"
#include "../InsertImageCommand.h"
#include "../InsertImageCommand.cpp"
#include "../ResizeImageCommand.h"
#include "../ResizeImageCommand.cpp"

const std::string IMAGE_PATH = "ezj.png";

class InsertParagraphCommandTestable : public InsertParagraphCommand 
{
public:
    InsertParagraphCommandTestable(std::vector<DocumentItem>& documentItems,
        const std::string& text,
        std::optional<size_t> position)
        : InsertParagraphCommand(documentItems, text, position) {}

    void TestDoExecute() 
    {
        DoExecute();
    }

    void TestDoUnexecute() 
    {
        DoUnexecute();
    }
};

class ReplaceTextCommandTestable : public ReplaceTextCommand 
{
public:
    ReplaceTextCommandTestable(std::vector<DocumentItem>& documentItems,
        const std::string& newText,
        std::optional<size_t> position)
        : ReplaceTextCommand(documentItems, newText, position) {}

    void TestDoExecute() 
    {
        DoExecute();
    }

    void TestDoUnexecute() 
    {
        DoUnexecute();
    }
};

class DeleteItemCommandTestable : public DeleteItemCommand 
{
public:
    DeleteItemCommandTestable(std::vector<DocumentItem>& documentItems, size_t position)
        : DeleteItemCommand(documentItems, position) {}

    void TestDoExecute() 
    {
        DoExecute();
    }

    void TestDoUnexecute() 
    {
        DoUnexecute();
    }
};

class SetTitleCommandTestable : public SetTitleCommand 
{
public:
    SetTitleCommandTestable(std::string& title, const std::string& newTitle)
        : SetTitleCommand(title, newTitle) {}

    void TestDoExecute() 
    {
        DoExecute();
    }

    void TestDoUnexecute() 
    {
        DoUnexecute();
    }
};

class InsertImageCommandTestable : public InsertImageCommand 
{
public:
    InsertImageCommandTestable(
        std::vector<DocumentItem>& documentItems,
        const std::string& path,
        int width,
        int height,
        std::optional<size_t> position)
        : InsertImageCommand(documentItems, path, width, height, position) {}

    void TestDoExecute() 
    {
        DoExecute();
    }

    void TestDoUnexecute() 
    {
        DoUnexecute();
    }
};

class ResizeImageCommandTestable : public ResizeImageCommand {
public:
    ResizeImageCommandTestable(
        std::vector<DocumentItem>& documentItems,
        std::optional<size_t> position,
        int newWidth,
        int newHeight)
        : ResizeImageCommand(documentItems, newWidth, newHeight, position) {}

    void TestDoExecute() 
    {
        DoExecute();
    }

    void TestDoUnexecute() 
    {
        DoUnexecute();
    }
};

class SaveCommandTestable : public SaveCommand 
{
public:
    SaveCommandTestable(const std::vector<DocumentItem>& documentItems, std::string path, std::string title)
        : SaveCommand(documentItems, std::move(path), std::move(title)) {}

    void TestDoExecute() 
    {
        DoExecute();
    }

    void TestDoUnexecute() 
    {
        DoUnexecute();
    }
};

TEST_CASE("InsertParagraphCommand: Insert at the end succeeds", "[InsertParagraphCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string text = "New paragraph text";

    InsertParagraphCommandTestable command(documentItems, text, std::nullopt);

    command.TestDoExecute();

    REQUIRE(documentItems.size() == 1);

    auto paragraph = std::dynamic_pointer_cast<Paragraph>(documentItems[0].GetParagraph());
    REQUIRE(paragraph != nullptr);
    CHECK(paragraph->GetText() == text);

    command.TestDoUnexecute();

    CHECK(documentItems.empty());
}

TEST_CASE("InsertParagraphCommand: Insert at position succeeds", "[InsertParagraphCommand]") 
{
    std::string text = "Existing paragraph";
    auto paragraph = std::make_shared<Paragraph>(text);
    DocumentItem documentItem(paragraph);
    std::vector<DocumentItem> documentItems = { documentItem };

    std::string textInserted = "Inserted paragraph";
    size_t position = 0;

    InsertParagraphCommandTestable command(documentItems, textInserted, position);

    command.TestDoExecute();

    REQUIRE(documentItems.size() == 2);

    auto paragraph1 = std::dynamic_pointer_cast<Paragraph>(documentItems[position].GetParagraph());
    REQUIRE(paragraph1 != nullptr);
    CHECK(paragraph1->GetText() == textInserted);

    command.TestDoUnexecute();

    REQUIRE(documentItems.size() == 1);
    CHECK(documentItems[0].GetParagraph()->GetText() == "Existing paragraph");
}

TEST_CASE("InsertParagraphCommand: Insert at invalid position throws error", "[InsertParagraphCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string text = "Text for invalid position";
    size_t invalidPosition = 10;

    InsertParagraphCommandTestable command(documentItems, text, invalidPosition);

    REQUIRE_THROWS_AS(command.TestDoExecute(), std::invalid_argument);
    CHECK(documentItems.empty());
}

TEST_CASE("ReplaceTextCommand: DoExecute replaces text at position", "[ReplaceTextCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string oldText = "Original text";
    documentItems.emplace_back(std::make_shared<Paragraph>(oldText));

    std::string newText = "Replaced text";
    size_t position = 0;

    ReplaceTextCommandTestable command(documentItems, newText, position);

    command.TestDoExecute();

    auto paragraph = std::dynamic_pointer_cast<Paragraph>(documentItems[position].GetParagraph());
    REQUIRE(paragraph != nullptr);
    CHECK(paragraph->GetText() == newText);

    command.TestDoUnexecute();

    CHECK(paragraph->GetText() == "Original text");
}

TEST_CASE("ReplaceTextCommand: Replace with invalid position throws error", "[ReplaceTextCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string newText = "Text for invalid position";
    size_t invalidPosition = 10;

    ReplaceTextCommandTestable command(documentItems, newText, invalidPosition);

    REQUIRE_THROWS_AS(command.TestDoExecute(), std::invalid_argument);
}

TEST_CASE("ReplaceTextCommand: Replace text without position throws error", "[ReplaceTextCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string newText = "Text for no position";

    ReplaceTextCommandTestable command(documentItems, newText, std::nullopt);

    REQUIRE_THROWS_AS(command.TestDoExecute(), std::invalid_argument);
}

TEST_CASE("DeleteItemCommand: Execute delete item at specified position", "[DeleteItemCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string text1 = "Paragraph 1";
    std::string text2 = "Paragraph 2";
    documentItems.emplace_back(std::make_shared<Paragraph>(text1));
    documentItems.emplace_back(std::make_shared<Paragraph>(text2));

    size_t position = 1;
    DeleteItemCommandTestable command(documentItems, position);

    command.TestDoExecute();

    REQUIRE(documentItems.size() == 1);
    CHECK(documentItems[0].GetParagraph()->GetText() == "Paragraph 1");

    command.TestDoUnexecute();

    REQUIRE(documentItems.size() == 2);
    CHECK(documentItems[1].GetParagraph()->GetText() == "Paragraph 2");
}

TEST_CASE("SetTitleCommand: Execute sets new title", "[SetTitleCommand]") 
{
    std::string title = "Original Title";
    std::string newTitle = "New Title";

    SetTitleCommandTestable command(title, newTitle);

    command.TestDoExecute();

    CHECK(title == "New Title");

    command.TestDoUnexecute();

    CHECK(title == "Original Title");
}

TEST_CASE("SetTitleCommandTest: DoUnexecute reverts to old title success", "[SetTitleCommand]") 
{
    std::string title = "Initial Title";
    std::string newTitle = "Updated Title";

    SetTitleCommandTestable command(title, newTitle);

    command.TestDoExecute();
    REQUIRE(title == "Updated Title");

    command.TestDoUnexecute();
    REQUIRE(title == "Initial Title");
}

TEST_CASE("SetTitleCommandTest: Repeated execute and unexecute success", "[SetTitleCommand]") 
{
    std::string title = "Start Title";
    std::string newTitle = "Changed Title";

    SetTitleCommandTestable command(title, newTitle);

    command.TestDoExecute();
    REQUIRE(title == "Changed Title");

    command.TestDoUnexecute();
    REQUIRE(title == "Start Title");

    command.TestDoExecute();
    REQUIRE(title == "Changed Title");

    command.TestDoUnexecute();
    REQUIRE(title == "Start Title");
}

TEST_CASE("SetTitleCommandTest: DoExecute with same title success", "[SetTitleCommand]") 
{
    std::string title = "Same Title";

    SetTitleCommandTestable command(title, title);

    command.TestDoExecute();
    REQUIRE(title == "Same Title");

    command.TestDoUnexecute();
    REQUIRE(title == "Same Title");
}

TEST_CASE("SaveCommandTest: DoExecute throws exception when file cannot be opened", "[SaveCommand]") 
{
    std::string invalidPath = "/invalid_path/test_output.html";
    std::vector<DocumentItem> documentItems;

    SaveCommandTestable command(documentItems, invalidPath, "Test Document");

    REQUIRE_THROWS_AS(command.TestDoExecute(), std::runtime_error);
}

TEST_CASE("InsertImageCommandTests: Execute inserts image at end success", "[InsertImageCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string imagePath = IMAGE_PATH;
    int width = 100;
    int height = 200;

    InsertImageCommandTestable command(documentItems, imagePath, width, height, std::nullopt);

    REQUIRE_NOTHROW(command.TestDoExecute());
    REQUIRE(documentItems.size() == 1);
    REQUIRE(std::filesystem::exists(imagePath));
}

TEST_CASE("InsertImageCommandTests: Execute inserts image at position success", "[InsertImageCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string imagePath = IMAGE_PATH;
    int width = 100;
    int height = 200;

    InsertImageCommandTestable command(documentItems, imagePath, width, height, 0);

    REQUIRE_NOTHROW(command.TestDoExecute());
    REQUIRE(documentItems.size() == 1);
    REQUIRE(std::filesystem::exists(imagePath));
}

TEST_CASE("InsertImageCommandTests: Execute throws on invalid position", "[InsertImageCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string imagePath = IMAGE_PATH;
    int width = 100;
    int height = 200;

    InsertImageCommandTestable command(documentItems, imagePath, width, height, 2);

    REQUIRE_THROWS_AS(command.TestDoExecute(), std::invalid_argument);
}

TEST_CASE("InsertImageCommandTests: Unexecute removes last inserted success", "[InsertImageCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string imagePath = IMAGE_PATH;
    int width = 100;
    int height = 200;

    InsertImageCommandTestable command(documentItems, imagePath, width, height, std::nullopt);

    command.TestDoExecute();
    REQUIRE(documentItems.size() == 1);
    REQUIRE(std::filesystem::exists(imagePath));

    command.TestDoUnexecute();
    REQUIRE(documentItems.size() == 0);
}

TEST_CASE("InsertImageCommandTests: Unexecute removes inserted at position success", "[InsertImageCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string imagePath = IMAGE_PATH;
    int width = 100;
    int height = 200;

    InsertImageCommandTestable command(documentItems, imagePath, width, height, 0);

    command.TestDoExecute();
    REQUIRE(documentItems.size() == 1);
    REQUIRE(std::filesystem::exists(imagePath));

    command.TestDoUnexecute();
    REQUIRE(documentItems.size() == 0);
}

TEST_CASE("InsertImageCommandTests: Execute throws on non-existent image", "[InsertImageCommand]") 
{
    std::vector<DocumentItem> documentItems;
    std::string nonExistentImagePath = "1/" + IMAGE_PATH;
    int width = 100;
    int height = 200;

    InsertImageCommandTestable command(documentItems, nonExistentImagePath, width, height, std::nullopt);

    REQUIRE_THROWS_AS(command.TestDoExecute(), std::invalid_argument);
    REQUIRE(!std::filesystem::exists(nonExistentImagePath));
}

TEST_CASE("ResizeImageCommandTests: Execute resizes image success", "[ResizeImageCommand]") 
{
    std::vector<DocumentItem> documentItems;
    auto image = std::make_shared<Image>(IMAGE_PATH, 100, 200);
    documentItems.emplace_back(image);

    ResizeImageCommandTestable command(documentItems, 0, 150, 300);

    REQUIRE_NOTHROW(command.TestDoExecute());
    REQUIRE(image->GetHeight() == 300);
    REQUIRE(image->GetWidth() == 150);
}

TEST_CASE("ResizeImageCommandTests: Execute throws on invalid position", "[ResizeImageCommand]") 
{
    std::vector<DocumentItem> documentItems;
    auto image = std::make_shared<Image>(IMAGE_PATH, 100, 200);
    documentItems.emplace_back(image);

    ResizeImageCommandTestable command(documentItems, 1, 300, 150);

    REQUIRE_THROWS_AS(command.TestDoExecute(), std::invalid_argument);
}

TEST_CASE("ResizeImageCommandTests: Execute throws when no image at position", "[ResizeImageCommand]") 
{
    std::vector<DocumentItem> documentItems;

    ResizeImageCommandTestable command(documentItems, 0, 150, 300);

    REQUIRE_THROWS_AS(command.TestDoExecute(), std::invalid_argument);
}

TEST_CASE("ResizeImageCommandTests: Unexecute resizes back to original size success", "[ResizeImageCommand]") 
{
    std::vector<DocumentItem> documentItems;
    auto image = std::make_shared<Image>(IMAGE_PATH, 100, 200);
    documentItems.emplace_back(image);

    ResizeImageCommandTestable command(documentItems, 0, 150, 300);

    command.TestDoExecute();
    REQUIRE(image->GetWidth() == 150);
    REQUIRE(image->GetHeight() == 300);

    command.TestDoUnexecute();
    REQUIRE(image->GetWidth() == 100);
    REQUIRE(image->GetHeight() == 200);
}

TEST_CASE("ResizeImageCommandTests: Unexecute throws on invalid position", "[ResizeImageCommand]") 
{
    std::vector<DocumentItem> documentItems;
    auto image = std::make_shared<Image>(IMAGE_PATH, 100, 200);
    documentItems.emplace_back(image);

    ResizeImageCommandTestable command(documentItems, 1, 150, 300);

    REQUIRE_THROWS_AS(command.TestDoUnexecute(), std::invalid_argument);
}

TEST_CASE("SaveCommandTest: DoExecute creates HTML file with images success", "[SaveCommand]")
{
    std::vector<DocumentItem> documentItems;
    std::string title = "Test Document";
    std::string filePath = "test_output_with_images.html";

    documentItems.emplace_back(std::make_shared<Paragraph>("Paragraph 1"));
    documentItems.emplace_back(std::make_shared<Paragraph>("Paragraph 2"));

    std::string imagePath = IMAGE_PATH;
    documentItems.emplace_back(std::make_shared<Image>(imagePath, 100, 200));

    SaveCommandTestable command(documentItems, filePath, title);

    command.TestDoExecute();

    std::ifstream inFile(filePath);
    REQUIRE(inFile.is_open());

    std::string line;
    std::getline(inFile, line);
    REQUIRE(line == "<!DOCTYPE html>");
    std::getline(inFile, line);
    REQUIRE(line == "<html>");
    std::getline(inFile, line);
    REQUIRE(line == "<head>");
    std::getline(inFile, line);
    REQUIRE(line == "<title>Test Document</title>");
    std::getline(inFile, line);
    REQUIRE(line == "</head>");
    std::getline(inFile, line);
    REQUIRE(line == "<body>");
    std::getline(inFile, line);
    REQUIRE(line == "<p>Paragraph 1</p>");
    std::getline(inFile, line);
    REQUIRE(line == "<p>Paragraph 2</p>");
    std::getline(inFile, line);
    REQUIRE(line == "<img src=\"images/ezj.png\" alt=\"ezj.png\" width=\"100\" height=\"200\" />");
    std::getline(inFile, line);
    REQUIRE(line == "</body>");
    std::getline(inFile, line);
    REQUIRE(line == "</html>");

    inFile.close();
    std::filesystem::remove(filePath);
}