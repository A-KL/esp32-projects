using HidSharp.Reports;
using Microsoft.Extensions.Options;
using System.IO.Ports;

namespace StreamDeck.Service;

public interface ITransport : IDisposable
{
    bool Send(string message);
    bool Available();
    void Close();
}

public record ResilientSerialTransportSettings(
    string PortName, int BaudRate);

public class ResilientSerialTransportWorker(
    IOptions<List<ResilientSerialTransportSettings>> settings, 
    ILogger<ResilientSerialTransportWorker> logger) : ITransport
{
    private readonly SerialPort _port = new ();
    
    public Task SendAsync(string data, CancellationToken cancellationToken)
    {
        if (_port.IsOpen)
        {
            _port.WriteLine(data);
        }

        return Task.CompletedTask;
    }

    public bool Send(string message)
    {
        if (!_port.IsOpen)
        {
            return false;
        }

        _port.WriteLine(message);

        return true;
    }
    
    public bool Available()
    {
        var availablePorts = SerialPort.GetPortNames();

        foreach (var setting in settings.Value)
        {
            if (!_port.IsOpen && !availablePorts.Contains(setting.PortName))
            {
                logger.LogWarning("Port '{SelectedPort}' is not available", setting.PortName);
                logger.LogDebug("Available ports:\r\n{AvailablePorts}", string.Join("\r\n", availablePorts));

                continue;
            }

            if (_port.IsOpen)
            {
                return true;
            }

            _port.PortName = setting.PortName;
            _port.BaudRate = setting.BaudRate;
            _port.Parity = Parity.None;
            _port.DataBits = 8;
            _port.NewLine = "\r";

            try
            {
                _port.Open();
                logger.LogInformation("Port '{SelectedPort}' is ready now.", setting.PortName);

                return true;
            }
            catch (Exception e)
            {
                logger.LogError("Port '{SelectedPort}' is unable to communicate. Error: {ErrorMessage}",
                    setting.PortName,
                    e.Message);

                continue;
            }
        }

        return false;
    }

    public void Close()
    {
        if (_port.IsOpen)
        {
            _port.Close();
        }
    }

    public void Dispose()
    {
        Close();
    }
}