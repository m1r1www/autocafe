namespace CampusVending;

/// <summary>Ингредиент из предыдущей версии проекта; автомат создаёт его сам (композиция).</summary>
public sealed class Ingredient
{
    public string Name { get; }
    public int Capacity { get; }
    public int Amount { get; private set; }

    public Ingredient(string name, int amount, int capacity)
    {
        Name = name;
        Capacity = Math.Max(capacity, 0);
        Amount = Math.Clamp(amount, 0, Capacity);
        Console.WriteLine($"[Ingredient] создан: {Name}");
    }

    public bool Load(int amount)
    {
        if (amount <= 0 || Amount + amount > Capacity) return false;
        Amount += amount;
        return true;
    }

    public bool Consume(int amount)
    {
        if (amount <= 0 || amount > Amount) return false;
        Amount -= amount;
        return true;
    }

    public override string ToString() => $"{Name}: {Amount}/{Capacity}";
}
