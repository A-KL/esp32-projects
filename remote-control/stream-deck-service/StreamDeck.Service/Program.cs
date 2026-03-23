using StreamDeck.Service;

var builder = Host.CreateApplicationBuilder(args);

builder.Services.AddSingleton(builder.Configuration.GetSection("SerialTransport").Get<ResilientSerialTransportSettings>());

builder.Services.AddSingleton<IFormatter<HardwareMetric>, HardwareMetricFormatter>();

builder.Services.AddSingleton<IFormatter<IList<HardwareMetric>>, HardwareMetricsFormatter>();

builder.Services.AddSingleton<ITransport, ResilientSerialTransportWorker>();

builder.Services.AddSingleton<IHardwareMonitor, WindowsHardwareMonitor>();

builder.Services.AddHostedService<Worker>();

var host = builder.Build();
host.Run();