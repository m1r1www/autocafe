namespace Cafe;

/// <summary>
/// Автомат: хранит ингредиент «Кофе» и состояние, готовит кофе.
/// Правила: заказ только в рабочем состоянии; нет кофе - отказ без списания;
/// пополнять запасы и менять состояние должен только оператор.
/// </summary>
public class VendingMachine
{
    private const int CoffeeCapacity = 100;
    private const int CoffeePortion = 20;

    // Композиция: ингредиент создаётся самим автоматом и наружу не отдаётся.
    private readonly Ingredient _coffee;

    public string Name { get; }
    public bool IsWorking { get; private set; } = true;

    public VendingMachine(string name, int coffeeAmount)
    {
        Name = name;
        _coffee = new Ingredient("Кофе", coffeeAmount, CoffeeCapacity);
        Console.WriteLine($"[VendingMachine] создан: {name}");
    }

    /// <summary>Заказ кофе: проверяет состояние и запас, списывает порцию.</summary>
    public bool MakeCoffee()
    {
        Console.WriteLine("[VendingMachine] заказ: кофе");

        if (!IsWorking)
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: автомат выключен");
            return false;
        }

        if (!_coffee.Consume(CoffeePortion))
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: кофе приготовить нельзя");
            return false;
        }

        Console.WriteLine("[VendingMachine] кофе приготовлен");
        return true;
    }

    // Три метода ниже internal: видны только внутри проекта. Вызывать их должен
    // только MachineOperator (в C# нет friend, поэтому это ещё и договорённость).
    internal bool RestockCoffee(int amount) => _coffee.Load(amount);

    internal void Start() => IsWorking = true;

    internal void Stop() => IsWorking = false;

    public override string ToString() => $"Автомат '{Name}': {(IsWorking ? "работает" : "выключен")}, {_coffee}";
}
