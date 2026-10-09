#include "machine_operator.hpp"
#include <iostream>
namespace cafe {
MachineOperator::MachineOperator(std::string_view name, VendingMachine& machine)
    : m_name{name}, m_machine{&machine} {
    std::cout << "[MachineOperator] создан: " << m_name << ", закреплён за '" << machine.GetName() << "'\n";
}
MachineOperator::~MachineOperator() {
    std::cout << "[MachineOperator] уничтожается: " << m_name << " (автомат не удаляется)\n";
}
bool MachineOperator::AddCoffee(int amount) { return m_machine != nullptr && m_machine->RestockCoffee(amount); }
bool MachineOperator::StartMachine() {
    if (m_machine == nullptr) return false;
    m_machine->Start();
    std::cout << "[MachineOperator] автомат запущен\n";
    return true;
}
bool MachineOperator::StopMachine() {
    if (m_machine == nullptr) return false;
    m_machine->Stop();
    std::cout << "[MachineOperator] автомат остановлен\n";
    return true;
}
}
