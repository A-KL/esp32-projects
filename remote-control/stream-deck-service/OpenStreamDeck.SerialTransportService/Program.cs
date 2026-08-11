using OpenStreamDeck.SerialTransportService;

var os = Environment.OSVersion;

Console.WriteLine($"OS: {os.Platform} {os.Version}");

var builder = Host.CreateApplicationBuilder(args);

builder.Services.Configure<List<ResilientSerialTransportSettings>>(builder.Configuration.GetSection("SerialTransports"));

builder.Services.AddSingleton<IFormatter<HardwareMetric>, HardwareMetricFormatter>();

builder.Services.AddSingleton<IFormatter<IList<HardwareMetric>>, HardwareMetricsFormatter>();

builder.Services.AddSingleton<ITransport, ResilientSerialTransportWorker>();

builder.Services.AddSingleton<IStorageClient, StorageClient>();

builder.Services.AddHostedService<Worker>();

var host = builder.Build();

host.Run();