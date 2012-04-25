#ifndef GBStatus_h
#define GBStatus_h

struct State {
  	int delay;
  	int blinkCount;
};

class GBStatus {
  public:
    GBStatus();
    void idle();
    void error();
	void failure();    
	void success();    
    void command();
    void on();
    void off();
    bool isState();
  private:
	static const State COMMAND;
	static const State SUCCESS;
	static const State ERROR;
	static const State FAILURE;	// Max int

  	bool _on;
  	unsigned long _changeStateMillis;
  	int _blinkCount;
	const State *_state;
	void setState(const State *state);
};

extern GBStatus Status;

#endif
