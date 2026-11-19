namespace Cafe;

/// <summary>
/// Журнал продаж. Нужен для демонстрации освобождения ресурса без деструктора:
/// при выходе из блока using автоматически вызывается Dispose().
/// </summary>
public sealed class SalesLog : IDisposable
{
    private readonly string _title;
    private int _records;

    public SalesLog(string title)
    {
        _title = title;
        Console.WriteLine($"[+] журнал открыт: {title}");
    }

    public void Record(string drink, bool success)
    {
        _records++;
        Console.WriteLine($"    запись {_records}: {drink} - {(success ? "выдан" : "отказ")}");
    }

    public void Dispose()
    {
        Console.WriteLine($"[-] журнал закрыт: {_title} (записей: {_records})");
    }
}
