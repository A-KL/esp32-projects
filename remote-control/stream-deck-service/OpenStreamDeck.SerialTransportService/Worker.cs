namespace OpenStreamDeck.SerialTransportService;

public class Worker(IStorageClient storage, ITransport transport, IFormatter<IList<HardwareMetric>> formatter, ILogger<Worker> logger) : BackgroundService
{
    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        while (!stoppingToken.IsCancellationRequested)
        {
            if (!storage.IsConnected)
            {
                try
                {
                    await storage.Connect(stoppingToken);
                }
                catch (Exception error)
                {
                    logger.LogError("Unable to connect to MQTT broker:  {ErrorMessage}", error.Message);
                    await Task.Delay(1000, stoppingToken);
                }
            }
            
            var metrics = new List<HardwareMetric>();// hardwareMonitor.Get();
            //
            // foreach (var metric in metrics)
            // {
            //    logger.LogInformation("{HardwareId} [{HardwareName}] {MetricValue}{MetricValueUnits}", metric.Id, metric.Name, metric.Value, metric.Units);
            // }

            if (!transport.Available())
            {
                await Task.Delay(1000, stoppingToken);
                continue;
            }
            
            //if (logger.IsEnabled(LogLevel.Information))
            //{
            //    logger.LogInformation("Worker running at: {time}", DateTimeOffset.Now);
            //}

            var message = formatter.Format(metrics);

            logger.LogDebug("Sending: {Message}", message);

            transport.Send(message);

            await Task.Delay(500, stoppingToken);
        }

        transport.Close();
        await storage.Disconnect();
        //hardwareMonitor.Close();
    }
}