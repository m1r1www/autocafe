#pragma once

#include "vending_machine.hpp"

#include <string>
#include <string_view>

namespace cafe
{

// Оператор: сотрудник, обслуживающий автомат.
// Связь с автоматом — агрегация: оператор хранит указатель на автомат, созданный снаружи,
// не создаёт и не уничтожает его; автомат должен пережить оператора.
// Правило 16: пополнять запасы и менять состояние автомата можно только через оператора.
class MachineOperator
{
private:
    std::string m_name{};
    VendingMachine* m_machine{ nullptr }; // агрегация: не владеем, не удаляем

public:
    MachineOperator(std::string_view name, VendingMachine& machine);
    MachineOperator(const MachineOperator&)            = delete;
    MachineOperator& operator=(const MachineOperator&) = delete;
    ~MachineOperator();

    bool AddCoffee(int amount);
    void StartMachine();
    void StopMachine();

    [[nodiscard]] std::string_view GetName() const
    {
        return m_name;
    }
};

} // namespace cafe
