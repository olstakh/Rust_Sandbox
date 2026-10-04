using System.Diagnostics.CodeAnalysis;

internal class LocalCache
{
    private readonly Dictionary<string, string> _cache = new();

    public void Add(string key, string value)
    {
        _cache[key] = value;
    }
    
    public bool TryGetValue(string key, [NotNullWhen(true)] out string? value)
    {
        return _cache.TryGetValue(key, out value);
    }

    public IEnumerable<KeyValuePair<string, string>> GetAll()
    {
        return _cache;
    }

    public bool Remove(string key)
    {
        return _cache.Remove(key);
    }

    public void Clear()
    {
        _cache.Clear();
    }
}