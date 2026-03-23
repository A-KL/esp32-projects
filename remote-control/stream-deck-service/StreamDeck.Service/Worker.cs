namespace StreamDeck.Service;

public class Worker(ITransport transport, IHardwareMonitor hardwareMonitor, IFormatter<IList<HardwareMetric>> formatter, ILogger<Worker> logger) : BackgroundService
{
    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        hardwareMonitor.Open();

        while (!stoppingToken.IsCancellationRequested)
        {
            var metrics = hardwareMonitor.Get();

            foreach (var metric in metrics)
            {
               // logger.LogInformation("{HardwareId} [{HardwareName}] {MetricValue}{MetricValueUnits}", metric.Id, metric.Name, metric.Value, metric.Units);
            }

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
        hardwareMonitor.Close();
    }
}