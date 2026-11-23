namespace Cafe;

/// <summary>
/// Ингредиент: название, текущий запас и вместимость.
/// Правило: запас не бывает отрицательным и не превышает вместимость.
/// </summary>
public class Ingredient
{
    public string Name { get; }
    public int Capacity { get; }
    public int Amount { get; private set; }

    public Ingredient(string name, int amount, int capacity)
    {
        Name = name;
        Capacity = Math.Max(capacity, 0);
        // запас приводим к границам 0..вместимость
        Amount = Math.Min(Math.Max(amount, 0), Capacity);

        if (Amount != amount || Capacity != capacity)
        {
            Console.WriteLine($"[Ingredient] ОТКАЗ: запас '{name}' должен быть в пределах 0..{Capacity}, установлено {Amount}");
        }
        Console.WriteLine($"[Ingredient] создан: {name}");
    }

    /// <summary>Пополнить запас.</summary>
    public bool Load(int value)
    {
        if (value <= 0 || Amount + value > Capacity)
        {
            Console.WriteLine($"[Ingredient] ОТКАЗ: нельзя добавить {value} ед. ингредиента '{Name}'");
            return false;
        }
        Amount += value;
        return true;
    }

    /// <summary>Списать часть запаса.</summary>
    public bool Consume(int value)
    {
        if (value <= 0 || value > Amount)
        {
            Console.WriteLine($"[Ingredient] ОТКАЗ: недостаточно '{Name}'");
            return false;
        }
        Amount -= value;
        return true;
    }

    public override string ToString() => $"{Name}: {Amount}/{Capacity}";
}
