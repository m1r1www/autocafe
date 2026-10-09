#pragma once
#include "vending_machine.hpp"
#include <string>
#include <string_view>
namespace cafe {
class MachineOperator {
private:
    std::string m_name{};
    VendingMachine* m_machine{nullptr};
public:
    MachineOperator(std::string_view name, VendingMachine& machine);
    ~MachineOperator();
    bool AddCoffee(int amount);
    bool StartMachine();
    bool StopMachine();
};
}
