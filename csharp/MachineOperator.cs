namespace Cafe;

/// <summary>
/// Оператор: сотрудник, обслуживающий автомат.
/// Агрегация: автомат создаётся снаружи и передаётся оператору уже готовым,
/// оператор хранит только ссылку на него.
/// </summary>
public class MachineOperator
{
    private readonly VendingMachine _machine;

    public string Name { get; }

    public MachineOperator(string name, VendingMachine machine)
    {
        ArgumentNullException.ThrowIfNull(machine);
        Name = name;
        _machine = machine;
        Console.WriteLine($"[MachineOperator] создан: {name}, закреплён за '{machine.Name}'");
    }

    public bool AddCoffee(int amount) => _machine.RestockCoffee(amount);

    public void StartMachine()
    {
        _machine.Start();
        Console.WriteLine("[MachineOperator] автомат запущен");
    }

    public void StopMachine()
    {
        _machine.Stop();
        Console.WriteLine("[MachineOperator] автомат остановлен");
    }

    public override string ToString() => $"Оператор {Name} (обслуживает '{_machine.Name}')";
}
