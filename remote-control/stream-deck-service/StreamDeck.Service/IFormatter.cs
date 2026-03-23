namespace StreamDeck.Service;

public interface IFormatter<T>
{
    string Format(T value);
}

public class HardwareMetricFormatter : IFormatter<HardwareMetric>
{
    public string Format(HardwareMetric value)
    {
        return $"{value.Id}.{value.Name}:{value.Value}{value.Units}";
    }
}

public class HardwareMetricsFormatter(IFormatter<HardwareMetric> itemFormatter) 
    : IFormatter<IList<HardwareMetric>>
{
    public string Format(IList<HardwareMetric> value)
    {
        var results = value
            .Select(itemFormatter.Format)
            .ToList();

        return string.Join("|", results);
    }
}