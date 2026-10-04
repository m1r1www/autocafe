#include "machine_operator.hpp"

#include <algorithm>
#include <iostream>

namespace cafe
{

MachineOperator::MachineOperator(std::string_view name)
    : m_name{ name }
{
    std::cout << "  [MachineOperator] создан: " << m_name << " (автоматов нет)\n";
}

MachineOperator::MachineOperator(std::string_view name, VendingMachine& machine)
    : m_name{ name }
    , m_machines{ &machine }
{
    std::cout << "  [MachineOperator] создан: " << m_name << ", закреплён автомат '" << machine.GetName() << "'\n";
}

MachineOperator::~MachineOperator()
{
    std::cout << "  [MachineOperator] уничтожается: " << m_name << " (автоматы не удаляются)\n";
}

bool MachineOperator::IsAssigned(const VendingMachine& machine) const
{
    return std::find(m_machines.begin(), m_machines.end(), &machine) != m_machines.end();
}

bool MachineOperator::CheckAssigned(const VendingMachine& machine) const
{
    if (!IsAssigned(machine))
    {
        std::cout << "  [MachineOperator] ОТКАЗ: " << m_name << " не закреплён за автоматом '" << machine.GetName()
                  << "'\n";
        return false;
    }
    return true;
}

bool MachineOperator::Assign(VendingMachine& machine)
{
    if (IsAssigned(machine))
    {
        std::cout << "  [MachineOperator] " << m_name << " уже закреплён за автоматом '" << machine.GetName() << "'\n";
        return false;
    }

    m_machines.push_back(&machine);
    std::cout << "  [MachineOperator] " << m_name << " закреплён за автоматом '" << machine.GetName() << "'\n";
    return true;
}

bool MachineOperator::Unassign(const VendingMachine& machine)
{
    const auto it = std::find(m_machines.begin(), m_machines.end(), &machine);
    if (it == m_machines.end())
    {
        return false;
    }

    m_machines.erase(it);
    std::cout << "  [MachineOperator] " << m_name << " откреплён от автомата '" << machine.GetName() << "'\n";
    return true;
}

bool MachineOperator::LoadIngredient(VendingMachine& machine, std::string_view name, int capacity, int amount,
                                     int expiryDay)
{
    return CheckAssigned(machine) && machine.LoadIngredient(name, capacity, amount, expiryDay);
}

bool MachineOperator::Restock(VendingMachine& machine, std::string_view name, int amount)
{
    return CheckAssigned(machine) && machine.Restock(name, amount);
}

bool MachineOperator::AddCups(VendingMachine& machine, int count)
{
    return CheckAssigned(machine) && machine.AddCups(count);
}

bool MachineOperator::AddChange(VendingMachine& machine, int amount)
{
    return CheckAssigned(machine) && machine.AddChange(amount);
}

bool MachineOperator::StartMaintenance(VendingMachine& machine)
{
    return CheckAssigned(machine) && machine.SwitchToMaintenance();
}

bool MachineOperator::FinishMaintenance(VendingMachine& machine)
{
    return CheckAssigned(machine) && machine.SwitchToWorking();
}

void MachineOperator::PrintInfo() const
{
    std::cout << "  Оператор " << m_name << ", автоматов: " << m_machines.size() << '\n';
    for (const VendingMachine* machine : m_machines)
    {
        std::cout << "    - " << machine->GetName() << '\n';
    }
}

} // namespace cafe
