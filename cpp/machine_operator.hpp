#pragma once

#include "vending_machine.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace campus_vending
{

// Оператор агрегирует автоматы: хранит слабые ссылки и не владеет ими.
class MachineOperator
{
private:
    std::string m_name{};
    std::vector<std::weak_ptr<VendingMachine>> m_machines{};

    [[nodiscard]] bool CheckAssigned(const std::shared_ptr<VendingMachine>& machine) const;

public:
    explicit MachineOperator(std::string_view name);
    MachineOperator(std::string_view name, const std::shared_ptr<VendingMachine>& machine);
    ~MachineOperator();

    bool Assign(const std::shared_ptr<VendingMachine>& machine);
    bool Unassign(const std::shared_ptr<VendingMachine>& machine);
    [[nodiscard]] bool IsAssigned(const std::shared_ptr<VendingMachine>& machine) const;

    bool LoadIngredient(const std::shared_ptr<VendingMachine>& machine, std::string_view name,
                        int capacity, int amount, int expiryDay);
    bool Restock(const std::shared_ptr<VendingMachine>& machine, std::string_view name, int amount);
    bool AddCups(const std::shared_ptr<VendingMachine>& machine, int count);
    bool AddChange(const std::shared_ptr<VendingMachine>& machine, int amount);
    bool StartMaintenance(const std::shared_ptr<VendingMachine>& machine);
    bool FinishMaintenance(const std::shared_ptr<VendingMachine>& machine);

    void PrintInfo() const;
};

} // namespace campus_vending
