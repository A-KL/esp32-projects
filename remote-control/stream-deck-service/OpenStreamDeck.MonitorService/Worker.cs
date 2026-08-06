namespace OpenStreamDeck.MonitorService;

public class Worker(IHardwareMonitor hardwareMonitor, IStorageClient storage,  ILogger<Worker> logger) 
    : BackgroundService
{
    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        hardwareMonitor.Open();
        
        while (!stoppingToken.IsCancellationRequested)
        {
            var metrics = hardwareMonitor.Get();

            foreach (var metric in metrics)
            {
                logger.LogInformation("{HardwareId} [{HardwareName}] {MetricValue}{MetricValueUnits}", metric.Id, metric.Name, metric.Value, metric.Units);
            }
            
            if (logger.IsEnabled(LogLevel.Information))
            {
                logger.LogInformation("Worker running at: {time}", DateTimeOffset.Now);
            }

            if (!storage.IsConnected)
            {
                try
                {
                    await storage.Connect(CancellationToken.None);
                }
                catch (Exception e)
                {
                    logger.LogInformation("Unable to connect to MQTT broker: {ErrorMessage}", e.Message);
                    await Task.Delay(1000, stoppingToken);
                }
            }

            foreach (var hardwareMetric in metrics)
            {
                await storage.Save($"{Environment.MachineName.ToLower()}/{hardwareMetric.Id}/{hardwareMetric.Name}",hardwareMetric.Value);
            }

            // Send
            
            await Task.Delay(500, stoppingToken);
        }
        
        hardwareMonitor.Close();
    }
}