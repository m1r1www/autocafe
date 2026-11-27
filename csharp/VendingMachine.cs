namespace CampusVending;

/// <summary>Автомат с каталогом напитков, снеков и готовых блюд.</summary>
public sealed class VendingMachine
{
    private readonly List<BaseProduct> _products = new();
    private readonly Ingredient _coffee;
    private readonly List<string> _journal = new();

    public string Name { get; }
    public bool IsWorking { get; private set; }
    public int Revenue { get; private set; }
    public int ChangeBox { get; private set; }

    public VendingMachine(string name)
    {
        Name = name;
        IsWorking = false;
        _coffee = new Ingredient("Кофе", 60, 100);
        Console.WriteLine($"[VendingMachine] создан: {Name}");
    }

    public void AddProduct(BaseProduct product)
    {
        ArgumentNullException.ThrowIfNull(product);
        if (_products.Any(item => item.InventoryNumber == product.InventoryNumber))
        {
            Console.WriteLine($"[VendingMachine] товар уже добавлен: {product.Name}");
            return;
        }
        _products.Add(product);
        Console.WriteLine($"[VendingMachine] в каталог добавлен: {product.Name}");
    }

    public bool SellProduct(string name, int paid)
    {
        if (!IsWorking)
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: автомат выключен");
            return false;
        }

        BaseProduct? product = _products.FirstOrDefault(item => item.Name == name);
        if (product is null || product.Stock <= 0)
        {
            Console.WriteLine($"[VendingMachine] ОТКАЗ: товар отсутствует или закончился: {name}");
            return false;
        }
        if (paid < product.Price)
        {
            Console.WriteLine($"[VendingMachine] ОТКАЗ: недостаточно денег, цена {product.Price}, внесено {paid}");
            return false;
        }

        int change = paid - product.Price;
        if (change > ChangeBox)
        {
            Console.WriteLine($"[VendingMachine] ОТКАЗ: недостаточно размена, требуется {change}, в кассете {ChangeBox}");
            return false;
        }

        // Списываем товар и деньги только после всех проверок.
        if (!product.SellOne()) return false;
        ChangeBox -= change;
        Revenue += product.Price;
        _journal.Add($"{product.Name} — {product.Price} руб., сдача {change} руб.");
        Console.WriteLine($"[VendingMachine] выдан товар: {product.Name}, сдача {change} руб.");
        return true;
    }

    // Запасы и размен пополняются только при выключенном автомате.
    internal bool AddChange(int amount)
    {
        if (IsWorking)
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: размен можно пополнять только при выключенном автомате");
            return false;
        }
        if (amount <= 0) return false;
        ChangeBox += amount;
        return true;
    }

    public void PrintProducts()
    {
        Console.WriteLine($"Каталог автомата «{Name}»:");
        foreach (BaseProduct product in _products)
            Console.WriteLine("  - " + product.Describe());
    }

    public void PrintJournal()
    {
        Console.WriteLine("Журнал продаж:");
        foreach (string entry in _journal) Console.WriteLine("  " + entry);
        Console.WriteLine($"Выручка: {Revenue} руб.");
    }

    public bool MakeCoffee()
    {
        if (!IsWorking)
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: автомат выключен");
            return false;
        }
        if (!_coffee.Consume(15))
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: недостаточно кофе");
            return false;
        }
        Console.WriteLine("[VendingMachine] кофе приготовлен");
        return true;
    }

    internal bool RestockCoffee(int amount)
    {
        if (IsWorking)
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: кофе можно пополнять только при выключенном автомате");
            return false;
        }
        return _coffee.Load(amount);
    }

    internal bool Start()
    {
        if (IsWorking)
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: автомат уже работает");
            return false;
        }
        IsWorking = true;
        return true;
    }

    internal bool Stop()
    {
        if (!IsWorking)
        {
            Console.WriteLine("[VendingMachine] ОТКАЗ: автомат уже выключен");
            return false;
        }
        IsWorking = false;
        return true;
    }

    public override string ToString() =>
        $"Автомат «{Name}»: {(IsWorking ? "работает" : "выключен")}, товаров: {_products.Count}, {_coffee}, размен: {ChangeBox} руб., выручка: {Revenue} руб.";
}
