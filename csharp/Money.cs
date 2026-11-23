namespace Cafe;

/// <summary>
/// Сумма денег. Это структура (тип-значение): присваивание копирует значение целиком.
/// Нужна для сравнения со ссылочным типом (классом) в демонстрации.
/// </summary>
public struct Money
{
    public int Rubles { get; set; }
    public int Kopecks { get; set; }

    public override string ToString() => $"{Rubles} руб. {Kopecks} коп.";
}
