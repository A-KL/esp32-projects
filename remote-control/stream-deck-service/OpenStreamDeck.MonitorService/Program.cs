using MQTTnet;
using OpenStreamDeck.MonitorService;

var os = Environment.OSVersion;

Console.WriteLine($"OS: {os.Platform} {os.Version}");

var builder = Host.CreateApplicationBuilder(args);

builder.Services.Configure<StorageClientSettings>(builder.Configuration.GetSection("StorageClient"));

builder.Services.AddSingleton<MqttClientFactory>();

builder.Services.AddSingleton<IStorageClient, StorageClient>();

if (os.Platform is PlatformID.Unix or PlatformID.MacOSX)
{
    builder.Services.AddSingleton<IHardwareMonitor, AppleHardwareMonitor>();
}
else
{
    builder.Services.AddSingleton<IHardwareMonitor, WindowsHardwareMonitor>();
}

builder.Services.AddHostedService<Worker>();

var host = builder.Build();

host.Run();
