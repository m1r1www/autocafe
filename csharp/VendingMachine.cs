namespace Cafe;

public class VendingMachine
{
    private readonly Ingredient _coffee;
    private bool _working = true;

    public string Name { get; }
    public bool IsWorking => _working;

    public VendingMachine(string name, int coffeeAmount)
    {
        Name = name;
        // Композиция: автомат сам создаёт свой ингредиент.
        _coffee = new Ingredient("Кофе", coffeeAmount, 100);
        Console.WriteLine($"[VendingMachine] создан: {Name}");
    }

    public bool MakeCoffee()
    {
        Console.WriteLine("[VendingMachine] заказ: кофе");
        if (!_working)
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: автомат выключен");
            return false;
        }
        if (!_coffee.Consume(20))
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: кофе приготовить нельзя");
            return false;
        }
        Console.WriteLine("[VendingMachine] кофе приготовлен");
        return true;
    }

    public bool RestockCoffee(int amount) => _coffee.Load(amount);
    public void Stop() => _working = false;
    public void Start() => _working = true;

    public override string ToString()
        => $"Автомат '{Name}': {(_working ? "работает" : "выключен")}, {_coffee}";
}
