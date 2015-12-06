/** @file RTX485.hpp
 *  RS485 bus communications implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 20. November 2015
 */

#ifndef RTX_RS485_H
#define RTX_RS485_H

#include <string>
#include <map>
#include <stdint.h>

#include "SerialPort.hpp"

namespace rtx { namespace hal {

  class RTX485 : public SerialPort {
  public:
    typedef uint8_t DeviceID;

    struct Message {
      DeviceID id;
      std::string data;
    };

    class Device;

    typedef boost::function<void (Message)> DeviceCallback;
    typedef std::map<DeviceID, DeviceCallback> SignalMap;

    struct SignalParams {
      DeviceID id;
      DeviceCallback cb;
    };

    typedef void (*WriteHandle)(Message);
    typedef void (*SignalService)(SignalParams);

    class Device {
    protected:
      DeviceID id;

      WriteHandle write;
      bool m_busReady;
    public:
      Device() {
        id = 255;
        write = nullptr;
        m_busReady = false;
      }

      void init(WriteHandle wHandle) {
        if(wHandle != nullptr)
          write = wHandle;

        m_busReady = true;
      }

      void init(WriteHandle wHandle, SignalService sigRegister) {
        DeviceCallback f;
        f = std::bind1st(std::mem_fun(&Device::signal), this);
        this->init(wHandle, sigRegister, f);
      }

      void init(WriteHandle wHandle, SignalService sigRegister, DeviceCallback dcb) {
        this->init(wHandle);

        if(sigRegister != nullptr) {
          SignalParams sp;
          sp.id = this->id;
                sp.cb = dcb;
          sigRegister(sp);
        }
      }

      virtual void signal(Message m) {
        std::cout << "UNHANDLED DEVICE SIGNAL CAPTURE" << std::endl;
      }

      virtual int send(Message m) {
        if(m_busReady) {
          write(m);
          return 0;
        }

        return -1;
      }

      DeviceID getID() { return id; }
    };

  private:
    WriteHandle writeProxy;

    std::string m_dataBuf;
  public:
    RTX485();

    void on_receive_(const std::string&);
    void sendCommand(DeviceID, std::string);

    int processData(const std::string&, RTX485::Message&);

    void registerDevice(SignalParams);
    void signal(Message);

    WriteHandle getWriteHandle() { return writeProxy; };
  };

}}

#endif // RTX_RS485_H
