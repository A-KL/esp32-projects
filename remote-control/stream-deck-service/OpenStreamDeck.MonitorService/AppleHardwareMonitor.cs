using System.Diagnostics;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;

namespace OpenStreamDeck.MonitorService;

public partial class AppleHardwareMonitor(ILogger<AppleHardwareMonitor> logger) : IHardwareMonitor
{
    private const int HostCpuLoadInfo = 3;
    private const int HostVmInfo64 = 4;
    private const string DegreeCelsius = "°C";

    private CpuLoadInfo _previousCpuLoad;
    private bool _hasPreviousCpuLoad;

    public void Open()
    {
        _previousCpuLoad = ReadCpuLoadInfo();
        _hasPreviousCpuLoad = true;
    }

    public IList<HardwareMetric> Get()
    {
        var results = new List<HardwareMetric>();

        if (TryGetCpuLoad(out var cpuLoad))
        {
            results.Add(new HardwareMetric("CPU0", "Load", ((int)cpuLoad).ToString(CultureInfo.InvariantCulture), "%"));
        }

        if (TryGetCpuTemperature(out var cpuTemperature))
        {
            results.Add(new HardwareMetric("CPU0", "Temperature", ((int)cpuTemperature).ToString(CultureInfo.InvariantCulture), DegreeCelsius));
        }

        if (TryGetMemoryUsed(out var memoryUsedGb))
        {
            results.Add(new HardwareMetric("RAM", "Used", ((int)memoryUsedGb).ToString(CultureInfo.InvariantCulture), "GB"));
        }

        return results;
    }
    
    public void Close()
    {
    }
    
    public void Dispose()
    {
        Close();
    }

    private bool TryGetCpuLoad(out double load)
    {
        load = 0;

        try
        {
            var currentCpuLoad = ReadCpuLoadInfo();

            if (!_hasPreviousCpuLoad)
            {
                _previousCpuLoad = currentCpuLoad;
                _hasPreviousCpuLoad = true;
                return false;
            }

            var user = currentCpuLoad.User - _previousCpuLoad.User;
            var system = currentCpuLoad.System - _previousCpuLoad.System;
            var nice = currentCpuLoad.Nice - _previousCpuLoad.Nice;
            var idle = currentCpuLoad.Idle - _previousCpuLoad.Idle;
            var total = user + system + nice + idle;

            _previousCpuLoad = currentCpuLoad;

            if (total <= 0)
            {
                return false;
            }

            load = (double)(total - idle) / total * 100;
            return true;
        }
        catch (Exception exception)
        {
            logger.LogWarning(exception, "Unable to read macOS CPU load.");
            return false;
        }
    }

    private static CpuLoadInfo ReadCpuLoadInfo()
    {
        var loadInfo = new CpuLoadInfo();
        var count = (uint)(Marshal.SizeOf<CpuLoadInfo>() / sizeof(int));

        var result = host_statistics64(mach_host_self(), HostCpuLoadInfo, ref loadInfo, ref count);

        if (result != 0)
        {
            throw new InvalidOperationException($"host_statistics64(HOST_CPU_LOAD_INFO) failed with code {result}.");
        }

        return loadInfo;
    }

    private bool TryGetMemoryUsed(out double usedGb)
    {
        usedGb = 0;

        try
        {
            var host = mach_host_self();

            var result = host_page_size(host, out var pageSize);
            if (result != 0)
            {
                return false;
            }

            var resultLength = (nuint)sizeof(long);
            var totalBytes = 0L;
            result = sysctlbyname("hw.memsize", ref totalBytes, ref resultLength, IntPtr.Zero, 0);
            if (result != 0 || totalBytes <= 0)
            {
                return false;
            }

            var statistics = new VmStatistics64();
            var count = (uint)(Marshal.SizeOf<VmStatistics64>() / sizeof(int));

            result = host_statistics64(host, HostVmInfo64, ref statistics, ref count);
            if (result != 0)
            {
                return false;
            }

            var availablePages = (ulong)statistics.FreeCount + statistics.SpeculativeCount;
            var availableBytes = availablePages * pageSize;
            var usedBytes = Math.Max(0, totalBytes - (long)availableBytes);
            usedGb = usedBytes / 1024d / 1024d / 1024d;

            return true;
        }
        catch (Exception exception)
        {
            logger.LogWarning(exception, "Unable to read macOS memory usage.");
            return false;
        }
    }

    private bool TryGetCpuTemperature(out double temperature)
    {
        temperature = 0;

        try
        {
            using var process = Process.Start(new ProcessStartInfo
            {
                FileName = "/usr/bin/powermetrics",
                Arguments = "--samplers smc -n 1 -i 1",
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false
            });

            if (process is null)
            {
                return false;
            }

            if (!process.WaitForExit(1500))
            {
                process.Kill();
                return false;
            }

            if (process.ExitCode != 0)
            {
                return false;
            }

            var output = process.StandardOutput.ReadToEnd();
            var match = CpuTemperatureRegex().Match(output);

            if (!match.Success ||
                !double.TryParse(match.Groups["temperature"].Value, NumberStyles.Float, CultureInfo.InvariantCulture, out temperature))
            {
                return false;
            }

            return true;
        }
        catch (Exception exception)
        {
            logger.LogDebug(exception, "Unable to read macOS CPU temperature.");
            return false;
        }
    }

    [GeneratedRegex(@"CPU die temperature:\s+(?<temperature>\d+(?:\.\d+)?)\s+C", RegexOptions.IgnoreCase)]
    private static partial Regex CpuTemperatureRegex();

    [DllImport("/usr/lib/libSystem.dylib")]
    private static extern IntPtr mach_host_self();

    [DllImport("/usr/lib/libSystem.dylib")]
    private static extern int host_page_size(IntPtr host, out uint pageSize);

    [DllImport("/usr/lib/libSystem.dylib", EntryPoint = "host_statistics64")]
    private static extern int host_statistics64(IntPtr host, int flavor, ref CpuLoadInfo info, ref uint count);

    [DllImport("/usr/lib/libSystem.dylib", EntryPoint = "host_statistics64")]
    private static extern int host_statistics64(IntPtr host, int flavor, ref VmStatistics64 info, ref uint count);

    [DllImport("/usr/lib/libSystem.dylib")]
    private static extern int sysctlbyname(string name, ref long oldp, ref nuint oldlenp, IntPtr newp, nuint newlen);

    [StructLayout(LayoutKind.Sequential)]
    private struct CpuLoadInfo
    {
        public uint User;
        public uint System;
        public uint Idle;
        public uint Nice;
    }

    [StructLayout(LayoutKind.Sequential)]
    private struct VmStatistics64
    {
        public uint FreeCount;
        public uint ActiveCount;
        public uint InactiveCount;
        public uint WireCount;
        public ulong ZeroFillCount;
        public ulong Reactivations;
        public ulong PageIns;
        public ulong PageOuts;
        public ulong Faults;
        public ulong CowFaults;
        public ulong Lookups;
        public ulong Hits;
        public ulong Purges;
        public uint PurgeableCount;
        public uint SpeculativeCount;
        public ulong Decompressions;
        public ulong Compressions;
        public ulong SwapIns;
        public ulong SwapOuts;
        public uint CompressorPageCount;
        public uint ThrottledCount;
        public uint ExternalPageCount;
        public uint InternalPageCount;
        public ulong TotalUncompressedPagesInCompressor;
    }
}