  #include <XboxSeriesXControllerESP32_asukiaaa.hpp>

class ControllerInput
{
public:
    virtual void loop()
    { }

    virtual bool isConnected() = 0;

    virtual bool begin() = 0;

    virtual int16_t readLeftStickY() = 0;

    virtual int16_t readRightStickY() = 0;
};

class XboxControllerInput : public ControllerInput
{
    public:
        XboxControllerInput(String deviceAddress) : _controller(deviceAddress)
        { }

        virtual void loop()
        { 
            _controller.onLoop();
        }

        virtual bool isConnected()
        {
            return _controller.isConnected();
        }

        virtual bool begin()
        {
            _controller.begin();

            return true;
        }

        virtual int16_t readLeftStickY()
        {

        }

        virtual int16_t readRightStickY()
        {

        }

    private:
        XboxSeriesXControllerESP32_asukiaaa::Core _controller;
};
