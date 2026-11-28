namespace CampusVending;

/// <summary>Абстрактный базовый класс любого товара университетского вендинга.</summary>
public abstract class BaseProduct : IEquatable<BaseProduct>
{
    private static int _createdCount;

    private readonly int _inventoryNumber;
    private readonly string _name;
    private int _price;
    private int _stock;
    private ProductDetails _details;

    protected BaseProduct()
        : this("Без названия", 0, 0, new ProductDetails(""))
    {
        Console.WriteLine("[BaseProduct] конструктор по умолчанию");
    }

    protected BaseProduct(string name, int price, int stock, ProductDetails? details = null)
    {
        _inventoryNumber = ++_createdCount;
        _name = name;
        _price = Math.Max(price, 0);
        _stock = Math.Max(stock, 0);
        _details = details ?? new ProductDetails("");
        Console.WriteLine($"[BaseProduct] создан: {_name} №{_inventoryNumber}");
    }

    /// <summary>Копирующий конструктор создаёт независимую карточку деталей.</summary>
    protected BaseProduct(BaseProduct other)
    {
        _inventoryNumber = ++_createdCount;
        _name = other._name;
        _price = other._price;
        _stock = other._stock;
        _details = new ProductDetails(other._details.Description);
        Console.WriteLine($"[BaseProduct] глубокая копия: {_name} №{_inventoryNumber}");
    }

    public string Name => _name;
    public int Price => _price;
    public int Stock => _stock;
    public int InventoryNumber => _inventoryNumber;
    public static int CreatedCount => _createdCount;

    protected string Header() => $"Товар №{_inventoryNumber} «{_name}»";

    public abstract string Category { get; }

    public virtual string Describe() =>
        $"{Header()}, категория: {Category}, цена: {_price} руб., остаток: {_stock}, описание: {_details.Description}";

    public abstract BaseProduct Clone();

    /// <summary>MemberwiseClone оставляет вложенную ссылку общей — поверхностная копия.</summary>
    public BaseProduct ShallowCopy() => (BaseProduct)MemberwiseClone();

    public void SetDetailsDescription(string description) => _details.Description = description;

    public BaseProduct SetPrice(int price)
    {
        if (price >= 0) _price = price;
        return this;
    }

    public bool SellOne()
    {
        if (_stock <= 0) return false;
        _stock--;
        return true;
    }

    public bool Restock(int amount)
    {
        if (amount <= 0) return false;
        _stock += amount;
        return true;
    }

    public void RegisterIn(VendingMachine machine)
    {
        machine.AddProduct(this); // передаём текущий объект через this
    }

    // Наследники добавляют в сравнение собственные характеристики товара.
    protected virtual bool HasSameSpecificProperties(BaseProduct other) => true;
    protected virtual int GetSpecificHashCode() => 0;

    public bool Equals(BaseProduct? other)
    {
        return other is not null
            && GetType() == other.GetType()
            && StringComparer.Ordinal.Equals(Name, other.Name)
            && Price == other.Price
            && StringComparer.Ordinal.Equals(Category, other.Category)
            && HasSameSpecificProperties(other);
    }

    public override bool Equals(object? obj) => obj is BaseProduct product && Equals(product);

    public override int GetHashCode() => HashCode.Combine(GetType(), Name, Price, Category, GetSpecificHashCode());
}
