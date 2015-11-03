#ifndef BaseMotorControl_h
#define BaseMotorControl_h

class BaseMotorControl {
  public:
    BaseMotorControl(int mosfet, int control);
    void on();
    void off();
    void attemptClear();
    bool isOn();
    bool isOff();
    virtual void speed(int percent) = 0;
    int speed();
  protected:
    bool _on;
    int _mosfetPin;
    int _controlPin;
    int _currentSpeed;
 };

#endif