namespace Cafe;

public class Ingredient
{
    public string Name { get; }
    public int Amount { get; private set; }
    public int Capacity { get; }

    public Ingredient(string name, int amount, int capacity)
    {
        Name = name;
        Amount = amount;
        Capacity = capacity;
    }

    public bool Load(int amount)
    {
        if (amount <= 0 || Amount + amount > Capacity)
        {
            Console.WriteLine($"[Ingredient] ОТКАЗ: нельзя добавить {amount} ед. ингредиента '{Name}'");
            return false;
        }
        Amount += amount;
        return true;
    }

    public bool Consume(int amount)
    {
        if (amount <= 0 || amount > Amount)
        {
            Console.WriteLine($"[Ingredient] ОТКАЗ: недостаточно '{Name}'");
            return false;
        }
        Amount -= amount;
        return true;
    }

    public override string ToString() => $"{Name}: {Amount}/{Capacity}";
}
