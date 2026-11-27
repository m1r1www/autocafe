namespace CampusVending;

/// <summary>sealed запрещает наследование от конкретного типа быстрого блюда.</summary>
public sealed class InstantMealProduct : BaseProduct
{
    private readonly int _cookingMinutes;

    public InstantMealProduct(string name, int price, int stock, int cookingMinutes, ProductDetails? details = null)
        : base(name, price, stock, details)
    {
        _cookingMinutes = Math.Max(cookingMinutes, 0);
        Console.WriteLine($"[InstantMealProduct] создан: {name}");
    }

    private InstantMealProduct(InstantMealProduct other) : base(other)
    {
        _cookingMinutes = other._cookingMinutes;
    }

    public override string Category => "быстрое блюдо";

    public override string Describe() => base.Describe() + $", приготовление: {_cookingMinutes} мин.";

    protected override bool HasSameSpecificProperties(BaseProduct other) =>
        other is InstantMealProduct meal && _cookingMinutes == meal._cookingMinutes;

    protected override int GetSpecificHashCode() => _cookingMinutes;

    public override BaseProduct Clone() => new InstantMealProduct(this);
}
