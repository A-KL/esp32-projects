#pragma once

#include <stdlib.h>
#include <esp32-hal-log.h>
#include <NimBLEDevice.h>

#define SCAN_TIME 10

enum class CnnStatus : uint8_t
{
    Idle,
    Scanning,
    DeviceFound,
    Connecting,
    Connected
};

class AdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks
{
public:
    void clear() {
        _devices.clear();
    }

    void onResult(NimBLEAdvertisedDevice *advertisedDevice)
    {
        log_i("Found device: %s", advertisedDevice->toString().c_str());

        _devices.push_back(advertisedDevice);

        // auto deviceName = advertisedDevice->getName();

        // for (size_t i = 0; i < number_of_supported_pads;  i++)
        // {
        //     if (deviceName == supported_pads[i].name)
        //     {
        //         log_i("** Found '%s'\n", supported_pads[i].name);

        //         foundDevice = advertisedDevice;
        //         found_pad = &supported_pads[i];
        //         cnn_status = CnnStatus::DeviceFound;
        //         scanner->stop();
        //     }
        // }
    }
private:
    std::vector<NimBLEAdvertisedDevice*> _devices;
};

class ClientCallbacks : public NimBLEClientCallbacks
{
private:
    CnnStatus _status;
public:
    inline CnnStatus Status() { return _status;}
    
    ClientCallbacks() : _status(CnnStatus::Idle)
    { }

    void onConnect(NimBLEClient *pClient)
    {
        log_i("** onConnect\n");
    };

    void onDisconnect(NimBLEClient *pClient)
    {
        log_i("** onDisconnect\n");
        log_i("Disconnected\n");
        _status = CnnStatus::Idle;
    };

    uint32_t onPassKeyRequest()
    {
        log_i("** onPassKeyRequest\n");
        return 0;
    };

    bool onConfirmPIN(uint32_t pass_key)
    {
        log_i("** onConfirmPIN\n");
        return true;
    };
};

NimBLEScan *scanner;
NimBLEAdvertisedDevice *foundDevice;

ClientCallbacks clientCallback;
AdvertisedDeviceCallbacks devicesCallback;

// pad_definition_t *supported_pads = nullptr;
// size_t number_of_supported_pads = 0;
// pad_definition_t *found_pad;

CnnStatus cnn_status = CnnStatus::Idle;

void ScanCompleteCB(NimBLEScanResults results)
{
    log_i("** Scan ended");
    if (clientCallback.Status() == CnnStatus::Scanning)
    {
        cnn_status = CnnStatus::Idle;
        log_i(" ...restarting\n");
    }
    else
    {
        log_i("\n");
    }
}

void CharacteristicNofifyCB(NimBLERemoteCharacteristic *characteristic, uint8_t *data, size_t length, bool is_notify)
{
    // static retro_joystick_status_t old_status;
    // auto status = old_status;

    // if (found_pad->bt_process_fn(data, length, &status))
    // {
    //     if (memcmp(&old_status, &status, sizeof(retro_joystick_status_t)))
    //     {
    //         BLERETRO_LOGF("        Data %d-> ", millis());
    //         BLERETRO_LOGF("isNotify: %d", is_notify);
    //         BLERETRO_LOGF("btnA: %d ", status.btnA);
    //         BLERETRO_LOGF("btnA_alt: %d ", status.btnA_alt);
    //         BLERETRO_LOGF("btnB: %d ", status.btnB);
    //         BLERETRO_LOGF("btnB_alt: %d ", status.btnB_alt);
    //         BLERETRO_LOGF("Up: %d ", status.up);
    //         BLERETRO_LOGF("Left: %d ", status.left);
    //         BLERETRO_LOGF("Right: %d ", status.right);
    //         BLERETRO_LOGF("Down: %d ", status.down);
    //         BLERETRO_LOGF("Auto inc: %d ", status.auto_inc);
    //         BLERETRO_LOGF("Auto dec: %d ", status.auto_dec);
    //         BLERETRO_LOGF("\n");

    //         old_status = status;

    //         StatusToGPIO(status);
    //     }
    // }
}

bool ble_connect_to_hid(NimBLEAdvertisedDevice *device)
{
    log_i("Connecting");

    auto client = NimBLEDevice::createClient(device->getAddress());

    client->setClientCallbacks(&clientCallback, false);
    client->setConnectTimeout(5);

    //cnn_status = CnnStatus::Connecting;
    auto retryCount = 9;
    auto connected = false;

    do {
        log_i("Connecting %d", retryCount);

        client->connect(true);
        delay(500);
        retryCount--;
    } 
    while (!client->isConnected() && retryCount > 0);

    auto services = client->getServices(true);

    static auto hidServiceUUID = NimBLEUUID("1812");

    for (auto service : *services)
    {
        auto serviceUUID = service->getUUID();
        if (serviceUUID.equals(hidServiceUUID))
        {
            log_i("-> Found HID\n");
            auto characteristics = service->getCharacteristics(true);

            for (auto characteristic : *characteristics)
            {
                log_i("---> Found characteristic %s\n", characteristic->toString());

                if (characteristic->canRead())
                {
                    log_i("-----> Can read...\n");
                    int retryCount = 3;
                    auto str = characteristic->readValue();
                    while (str.size() == 0 && retryCount > 0)
                    {
                        retryCount--;
                        str = characteristic->readValue();
                    }
                    log_i("-----> ...'%s'\n", str);
                }

                if (characteristic->canNotify())
                {
                    log_i("-----> Can notify. Suscribing.\n");

                    characteristic->subscribe(true, CharacteristicNofifyCB, true);
                    connected = true;
                    log_i("Connected.\n");
                }
            }
        }
    }

    if (!connected)
    {
        NimBLEDevice::deleteClient(client);
        log_i("Can't connect");
    }
    return connected;
}

void ble_init()
{
    log_i("Setting up Bluetooth BLE");

    NimBLEDevice::setScanFilterMode(CONFIG_BTDM_SCAN_DUPL_TYPE_DEVICE);
    NimBLEDevice::setScanDuplicateCacheSize(200);
    NimBLEDevice::init("m-drive-mini");
    NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_PUBLIC);
    NimBLEDevice::setSecurityAuth(true, false, false);

    log_i("Setting up scanner");
    devicesCallback.clear();
    scanner = NimBLEDevice::getScan();
    scanner->setAdvertisedDeviceCallbacks(&devicesCallback);
    scanner->setActiveScan(true);
    scanner->setInterval(1000);
    scanner->setWindow(900);
}

CnnStatus ble_scan_and_connect()
{
    log_i("Starting scanner\n");
    scanner->clearResults();
    scanner->clearDuplicateCache();
    scanner->setDuplicateFilter(true);

    scanner->start(SCAN_TIME, &ScanCompleteCB, false);
    return CnnStatus::Scanning;
}

CnnStatus ble_loop()
{
    if (cnn_status == CnnStatus::Idle)
    {
        cnn_status = ble_scan_and_connect();
    }
    else if (cnn_status == CnnStatus::DeviceFound)
    {
        if (foundDevice != nullptr)
        {
            ble_connect_to_hid(foundDevice);
        }
        else
        {
            cnn_status = CnnStatus::Idle;
        }
    } else if (cnn_status == CnnStatus::Connected)
    {
        log_i("Connected");
       // DoAutofire();
    }

    return cnn_status;
}