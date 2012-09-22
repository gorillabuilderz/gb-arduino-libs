#ifndef Transport_h
#define Transport_h

#include <Arduino.h>

class Transport : public Stream
{
  public:
	// 20 seconds enough? Too much?
  	static const unsigned long DEFAULT_TIMEOUT = 20000;
  	
  	Transport(unsigned long timeout = DEFAULT_TIMEOUT);
  	
    virtual bool initialise();
    // Inherited method overloading
    using Print::write;
    virtual size_t write(const uint8_t byte) { return 0; };
    virtual bool waitForData();
    virtual int available();
    virtual int read() { return 0; };
    virtual void prepareRead() {};
    virtual void prepareWrite() {};
	virtual void select() {};
    virtual void deselect() {};
    virtual int peek() { return 0; };
    virtual void flush() {};

    unsigned long getTimeout();
	
  private:
  	unsigned long _timeout;
};

#endif
