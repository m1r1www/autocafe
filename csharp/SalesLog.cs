namespace CampusVending;

/// <summary>Журнал с IDisposable, сохранённый из предыдущей работы.</summary>
public sealed class SalesLog : IDisposable
{
    private readonly string _title;
    private int _records;

    public SalesLog(string title)
    {
        _title = title;
        Console.WriteLine($"[+] журнал открыт: {_title}");
    }

    public void Record(string line)
    {
        _records++;
        Console.WriteLine($"    запись {_records}: {line}");
    }

    public void Dispose() => Console.WriteLine($"[-] журнал закрыт: {_title} (записей: {_records})");
}
