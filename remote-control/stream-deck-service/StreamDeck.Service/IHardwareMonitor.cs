using OpenHardwareMonitor.Hardware;

namespace StreamDeck.Service;

public record HardwareMetric(string Id, string Name, string Value, string Units);

public interface IHardwareMonitor : IDisposable
{
    void Open();

    IList<HardwareMetric> Get();

    void Close();
}

public class WindowsHardwareMonitor(ILogger<WindowsHardwareMonitor> logger) : IHardwareMonitor
{
    private readonly Computer _pc = new();

    public void Open()
    {
        _pc.IsCpuEnabled = true;
        _pc.IsGpuEnabled = true;
        _pc.IsMemoryEnabled = true;

        _pc.Open(true);
    }

    public IList<HardwareMetric> Get()
    {
        var cpus = _pc.Hardware
            .Where(h => h.HardwareType == HardwareType.Cpu)
            .ToList();

        var gpus = _pc.Hardware
            .Where(h => h.HardwareType == HardwareType.GpuNvidia || h.HardwareType == HardwareType.GpuAmd)
            .ToList();

        var rams = _pc.Hardware
            .Where(h => h.HardwareType == HardwareType.Memory)
            .Where(x => x.Name == "Physical Memory")
            .ToList();

        var resutls = new List<HardwareMetric>();

        // CPU
        foreach (var cpu in cpus)
        {
            cpu.Update();
            cpu.GetReport();

            var id = cpu.HardwareType.ToString().ToUpper() + cpu.Identifier.ToString().Split('/').Last();

            var temp = cpu.Sensors
                .Where(x => x.SensorType == SensorType.Temperature)
                .Where(x => x.Name == "Core Average")
                .Max(x => x.Value);

            var load = cpu.Sensors
                .Where(x => x.SensorType == SensorType.Load)
                .First(x => x.Name == "Total")
                .Value;

            resutls.Add(new HardwareMetric(id, "Load", ((int)load!).ToString(), "%"));
            resutls.Add(new HardwareMetric(id, "Temperature", ((int)temp!).ToString(), "°C"));
        }

        //RAM
        foreach (var ram in rams)
        {
            ram.Update();
            ram.GetReport();

            var total = ram.Sensors
                .Where(x => x.SensorType == SensorType.Data)
                .First(x => x.Name == "Total")
                .Value;

            var used = ram.Sensors
                .Where(x => x.SensorType == SensorType.Data)
                .First(x => x.Name == "Used")
                .Value;

            resutls.Add(new HardwareMetric("RAM", "Used", ((int)used!).ToString(), "GB"));
        }

        //GPU
        foreach (var gpu in gpus)
        {
            gpu.Update();
            gpu.GetReport();

            var index = gpu.Identifier.ToString().Split('/').Last();
            var id = "GPU" + index;
            var units = "MB";

            var memory = gpu.Sensors
                .Where(x => x.SensorType == SensorType.SmallData)
                .First(x => x.Name == "GPU Memory Used")
                .Value;

            if (memory > 1024)
            {
                memory /= 1024;
                units = "GB";
            }

            var temp = gpu.Sensors
                .Where(x => x.SensorType == SensorType.Temperature)
                .Where(x => x.Name == "GPU Hot Spot")
                .Max(x => x.Value);

            resutls.Add(new HardwareMetric(id, "Memory", ((int)memory!).ToString(), units));
            resutls.Add(new HardwareMetric(id, "Temperature", ((int)temp!).ToString(), "°C"));
        }


        return resutls;
    }

    public void Close()
    {
        _pc.Close();
    }

    public void Dispose()
    {
        Close();
    }
}