namespace OpenStreamDeck.MonitorService;

public record HardwareMetric(
    string Id, string Name, string Value, string Units);

public interface IHardwareMonitor : IDisposable
{
    void Open();

    IList<HardwareMetric> Get();

    void Close();
}