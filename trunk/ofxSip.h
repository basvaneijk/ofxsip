
#include <pjsua-lib/pjsua.h>

#include <string>
using namespace std;


class ofxSip {

public:
	ofxSip();
	~ofxSip();
	
	
	bool setup(string domain, string username, string password, int port = 5060);
	void stop();
	
	bool makeCall(string url);
	void hangUp();

private:
	bool alreadySetup;
	pjsua_acc_id acc_id;
	
	
    
};