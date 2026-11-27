namespace CampusVending;

/// <summary>Структура-значение из предыдущей работы.</summary>
public struct Money
{
    public int Rubles { get; set; }
    public int Kopecks { get; set; }

    public override string ToString() => $"{Rubles} руб. {Kopecks} коп.";
}
