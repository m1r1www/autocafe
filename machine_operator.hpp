#pragma once

#include "vending_machine.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace cafe
{

// Оператор: сотрудник, обслуживающий один или несколько автоматов.
// Связь с автоматами — агрегация: оператор хранит указатели на автоматы, созданные снаружи,
// не создаёт и не уничтожает их; автомат должен пережить закрепление за оператором.
// Следит за правилом: действовать можно только с автоматом, закреплённым за оператором.
class MachineOperator
{
private:
    std::string m_name{};
    std::vector<VendingMachine*> m_machines{};

    [[nodiscard]] bool CheckAssigned(const VendingMachine& machine) const;

public:
    explicit MachineOperator(std::string_view name);
    MachineOperator(std::string_view name, VendingMachine& machine);
    MachineOperator(const MachineOperator&)            = delete;
    MachineOperator& operator=(const MachineOperator&) = delete;
    ~MachineOperator();

    bool Assign(VendingMachine& machine);
    bool Unassign(const VendingMachine& machine);
    [[nodiscard]] bool IsAssigned(const VendingMachine& machine) const;

    bool LoadIngredient(VendingMachine& machine, std::string_view name, int capacity, int amount, int expiryDay);
    bool Restock(VendingMachine& machine, std::string_view name, int amount);
    bool AddCups(VendingMachine& machine, int count);
    bool AddChange(VendingMachine& machine, int amount);
    bool StartMaintenance(VendingMachine& machine);
    bool FinishMaintenance(VendingMachine& machine);

    void PrintInfo() const;

    [[nodiscard]] std::string_view GetName() const
    {
        return m_name;
    }

    [[nodiscard]] std::size_t GetMachineCount() const
    {
        return m_machines.size();
    }
};

} // namespace cafe
