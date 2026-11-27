namespace CampusVending;

public class SnackProduct : BaseProduct
{
    private readonly int _weightGrams;

    public SnackProduct(string name, int price, int stock, int weightGrams, ProductDetails? details = null)
        : base(name, price, stock, details)
    {
        _weightGrams = Math.Max(weightGrams, 0);
        Console.WriteLine($"[SnackProduct] создан: {name}");
    }

    public SnackProduct(SnackProduct other) : base(other)
    {
        _weightGrams = other._weightGrams;
        Console.WriteLine("[SnackProduct] копирование");
    }

    public override string Category => "перекус";

    // В отличие от CoffeeProduct здесь базовое Describe() намеренно не вызывается.
    public override string Describe() =>
        $"{Header()}, категория: перекус, вес: {_weightGrams} г, цена: {Price} руб., остаток: {Stock}";

    protected override bool HasSameSpecificProperties(BaseProduct other) =>
        other is SnackProduct snack && _weightGrams == snack._weightGrams;

    protected override int GetSpecificHashCode() => _weightGrams;

    public override BaseProduct Clone() => new SnackProduct(this);
}
