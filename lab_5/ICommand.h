#ifndef LAB5_ICOMMAND_H
#define LAB5_ICOMMAND_H

// tryMerge ???? ????????
class ICommand
{
public:
    virtual void Execute() = 0;
    virtual void Unexecute() = 0;

    virtual ~ICommand() = default;
};

#endif //LAB5_ICOMMAND_H