#include <memory>

class ICommand;
typedef std::unique_ptr<ICommand> ICommandPtr;