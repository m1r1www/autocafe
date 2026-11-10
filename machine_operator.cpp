#include "machine_operator.hpp"

#include <iostream>

namespace cafe
{

MachineOperator::MachineOperator(std::string_view name, VendingMachine& machine)
    : m_name{ name }
    , m_machine{ &machine }
{
    std::cout << "[MachineOperator] создан: " << m_name << ", закреплён за '" << machine.GetName() << "'\n";
}

MachineOperator::~MachineOperator()
{
    std::cout << "[MachineOperator] уничтожается: " << m_name << " (автомат не удаляется)\n";
}

bool MachineOperator::AddCoffee(int amount)
{
    return m_machine->RestockCoffee(amount);
}

void MachineOperator::StartMachine()
{
    m_machine->Start();
    std::cout << "[MachineOperator] автомат запущен\n";
}

void MachineOperator::StopMachine()
{
    m_machine->Stop();
    std::cout << "[MachineOperator] автомат остановлен\n";
}

} // namespace cafe
