namespace CampusVending;

/// <summary>Вложенная карточка товара: используется для демонстрации shallow/deep copy.</summary>
public sealed class ProductDetails
{
    public string Description { get; set; }

    public ProductDetails(string description)
    {
        Description = description;
    }
}
