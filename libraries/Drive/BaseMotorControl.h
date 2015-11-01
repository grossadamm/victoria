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
  protected:
    bool _on;
    int _mosfetPin;
    int _controlPin;
 };

#endif