using MQTTnet;
using Microsoft.Extensions.Options;

namespace OpenStreamDeck.MonitorService;

public interface IStorageClient : IDisposable
{
    Task Connect(CancellationToken token);
    Task Disconnect();
    Task Save(string topic, string payload);

    bool IsConnected { get; }
}

public record StorageClientSettings(
    string Host, int Port, string ClientId, string RootTopic);

public class StorageClient(
    MqttClientFactory factory,
    IOptions<StorageClientSettings> options,
    ILogger<StorageClient> logger) : IStorageClient
{
    private readonly IMqttClient _client = factory.CreateMqttClient();
    private readonly StorageClientSettings _settings = options.Value;
    private readonly ILogger<StorageClient> _logger = logger;

    public bool IsConnected => _client.IsConnected;

    public async Task Connect(CancellationToken token =  default)
    {
        var mqttClientOptions = new MqttClientOptionsBuilder()
            .WithClientId(_settings.ClientId)
            .WithTcpServer(_settings.Host, _settings.Port)
            .Build();

        _logger.LogInformation(
            "Connecting to MQTT broker {Host}:{Port} as {ClientId}",
            _settings.Host,
            _settings.Port,
            _settings.ClientId);

        await _client.ConnectAsync(mqttClientOptions, token);
    }

    public async Task Save(string topic, string payload)
    {
        var applicationMessage = new MqttApplicationMessageBuilder()
            .WithTopic($"{_settings.RootTopic}/{topic}")
            .WithPayload(payload)
            .Build();
        
        await _client.PublishAsync(applicationMessage, CancellationToken.None);
    }

    public Task Disconnect()
    {
        return _client.DisconnectAsync();
    }

    public void Dispose()
    {
        _client.Dispose();
    }
}
