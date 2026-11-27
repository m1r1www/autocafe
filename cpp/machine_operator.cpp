#include "machine_operator.hpp"

#include <algorithm>
#include <iostream>

namespace campus_vending
{

MachineOperator::MachineOperator(std::string_view name)
    : m_name{ name }
{
    std::cout << "  [MachineOperator] создан: " << m_name << " (автоматов нет)\n";
}

MachineOperator::MachineOperator(std::string_view name, const std::shared_ptr<VendingMachine>& machine)
    : m_name{ name }
{
    Assign(machine);
    std::cout << "  [MachineOperator] создан: " << m_name << '\n';
}

MachineOperator::~MachineOperator()
{
    std::cout << "  [MachineOperator] уничтожается: " << m_name << " (автоматы не удаляются)\n";
}

bool MachineOperator::IsAssigned(const std::shared_ptr<VendingMachine>& machine) const
{
    if (!machine) return false;

    for (const auto& weakMachine : m_machines)
    {
        const auto current = weakMachine.lock();
        if (current && current == machine) return true;
    }
    return false;
}

bool MachineOperator::CheckAssigned(const std::shared_ptr<VendingMachine>& machine) const
{
    if (!IsAssigned(machine))
    {
        std::cout << "  [MachineOperator] ОТКАЗ: автомат не закреплён за оператором " << m_name << '\n';
        return false;
    }
    return true;
}

bool MachineOperator::Assign(const std::shared_ptr<VendingMachine>& machine)
{
    if (!machine || IsAssigned(machine)) return false;

    m_machines.push_back(machine);
    std::cout << "  [MachineOperator] " << m_name << " закреплён за автоматом '" << machine->GetName() << "'\n";
    return true;
}

bool MachineOperator::Unassign(const std::shared_ptr<VendingMachine>& machine)
{
    if (!machine) return false;

    const auto oldSize = m_machines.size();
    m_machines.erase(std::remove_if(m_machines.begin(), m_machines.end(), [&](const auto& weakMachine) {
        const auto current = weakMachine.lock();
        return !current || current == machine;
    }), m_machines.end());
    return m_machines.size() != oldSize;
}

bool MachineOperator::LoadIngredient(const std::shared_ptr<VendingMachine>& machine, std::string_view name,
                                     int capacity, int amount, int expiryDay)
{
    return CheckAssigned(machine) && machine->LoadIngredient(name, capacity, amount, expiryDay);
}

bool MachineOperator::Restock(const std::shared_ptr<VendingMachine>& machine, std::string_view name, int amount)
{
    return CheckAssigned(machine) && machine->Restock(name, amount);
}

bool MachineOperator::AddCups(const std::shared_ptr<VendingMachine>& machine, int count)
{
    return CheckAssigned(machine) && machine->AddCups(count);
}

bool MachineOperator::AddChange(const std::shared_ptr<VendingMachine>& machine, int amount)
{
    return CheckAssigned(machine) && machine->AddChange(amount);
}

bool MachineOperator::StartMaintenance(const std::shared_ptr<VendingMachine>& machine)
{
    return CheckAssigned(machine) && machine->SwitchToMaintenance();
}

bool MachineOperator::FinishMaintenance(const std::shared_ptr<VendingMachine>& machine)
{
    return CheckAssigned(machine) && machine->SwitchToWorking();
}

void MachineOperator::PrintInfo() const
{
    std::size_t count = 0;
    std::cout << "  Оператор " << m_name << ", автоматы:\n";
    for (const auto& weakMachine : m_machines)
    {
        if (const auto machine = weakMachine.lock())
        {
            ++count;
            std::cout << "    - " << machine->GetName() << '\n';
        }
    }
    std::cout << "    всего живых автоматов: " << count << '\n';
}

} // namespace campus_vending
