namespace CampusVending;

public class CoffeeProduct : BaseProduct
{
    private readonly int _volumeMl;
    private readonly bool _hasMilk;

    public CoffeeProduct() : base()
    {
        Console.WriteLine("[CoffeeProduct] конструктор по умолчанию");
    }

    public CoffeeProduct(string name, int price, int stock, int volumeMl, bool hasMilk, ProductDetails? details = null)
        : base(name, price, stock, details)
    {
        _volumeMl = Math.Max(volumeMl, 0);
        _hasMilk = hasMilk;
        Console.WriteLine($"[CoffeeProduct] создан: {name}");
    }

    public CoffeeProduct(CoffeeProduct other) : base(other)
    {
        _volumeMl = other._volumeMl;
        _hasMilk = other._hasMilk;
        Console.WriteLine("[CoffeeProduct] копирование");
    }

    public override string Category => "напиток";

    public override string Describe() =>
        base.Describe() + $", объём: {_volumeMl} мл" + (_hasMilk ? ", с молоком" : ", без молока");

    protected override bool HasSameSpecificProperties(BaseProduct other) =>
        other is CoffeeProduct coffee && _volumeMl == coffee._volumeMl && _hasMilk == coffee._hasMilk;

    protected override int GetSpecificHashCode() => HashCode.Combine(_volumeMl, _hasMilk);

    public override BaseProduct Clone() => new CoffeeProduct(this);
}
