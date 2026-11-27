namespace CampusVending;

/// <summary>Оператор агрегирует один автомат и хранит на него слабую ссылку.</summary>
public sealed class MachineOperator
{
    private readonly WeakReference<VendingMachine> _machine;
    public string Name { get; }

    public MachineOperator(string name, VendingMachine machine)
    {
        ArgumentNullException.ThrowIfNull(machine);
        Name = name;
        _machine = new WeakReference<VendingMachine>(machine);
        Console.WriteLine($"[MachineOperator] {Name} закреплён за автоматом «{machine.Name}»");
    }

    // В отличие от C++-версии здесь нет метода, принимающего произвольный автомат:
    // оператор работает только с единственным автоматом, переданным конструктору.
    // Проверка нужна, чтобы убедиться, что слабая ссылка ещё разрешается.
    private bool TryGetMachine(out VendingMachine? machine)
    {
        if (_machine.TryGetTarget(out machine)) return true;
        Console.WriteLine("[MachineOperator] ОТКАЗ: закреплённый автомат больше не существует");
        return false;
    }

    public bool AddCoffee(int amount)
    {
        if (!TryGetMachine(out VendingMachine? machine)) return false;
        return machine.RestockCoffee(amount);
    }

    public bool AddChange(int amount)
    {
        if (!TryGetMachine(out VendingMachine? machine)) return false;
        return machine.AddChange(amount);
    }

    public bool StartMachine()
    {
        if (!TryGetMachine(out VendingMachine? machine)) return false;
        if (!machine.Start()) return false;
        Console.WriteLine("[MachineOperator] автомат запущен");
        return true;
    }

    public bool StopMachine()
    {
        if (!TryGetMachine(out VendingMachine? machine)) return false;
        if (!machine.Stop()) return false;
        Console.WriteLine("[MachineOperator] автомат остановлен");
        return true;
    }
}
