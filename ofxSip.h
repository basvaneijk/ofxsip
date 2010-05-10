
/**
 * ofxSip.h
 *
 * Adds SIP functionality to openframeworks. Must be GPL because pjsip is GPL.
 *
 * Author: Marek Bereza 10-05-10
 * 
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation; either version 2 of the License, or
 # (at your option) any later version.
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License
 # along with this program; if not, write to the Free Software
 # Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 #
 */
#include "pjsua-lib/pjsua.h"

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