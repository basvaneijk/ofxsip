#include "ofxSip.h"



/* Callback called by the library upon receiving incoming call */
void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
							 pjsip_rx_data *rdata)
{
    pjsua_call_info ci;
	
    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);
	
    pjsua_call_get_info(call_id, &ci);
	
    PJ_LOG(3,("ofxSip.cpp", "Incoming call from %.*s!!",
			  (int)ci.remote_info.slen,
			  ci.remote_info.ptr));
	
    /* Automatically answer incoming calls with 200/OK */
    pjsua_call_answer(call_id, 200, NULL, NULL);
}

/* Callback called by the library when call's state has changed */
void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
    pjsua_call_info ci;
	
    PJ_UNUSED_ARG(e);
	
    pjsua_call_get_info(call_id, &ci);
    PJ_LOG(3,("ofxSip.cpp", "Call %d state=%.*s", call_id,
			  (int)ci.state_text.slen,
			  ci.state_text.ptr));
}

/* Callback called by the library when call's media state has changed */
void on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info ci;
	
    pjsua_call_get_info(call_id, &ci);
	
    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
		// When media is active, connect call to sound device.
		pjsua_conf_connect(ci.conf_slot, 0);
		pjsua_conf_connect(0, ci.conf_slot);
    }
}



ofxSip::ofxSip() {
	alreadySetup = false;
}
ofxSip::~ofxSip() {
	stop();
}

void ofxSip::stop() {
	if(alreadySetup) {
		// Destroy pjsua 
		pjsua_destroy();
		alreadySetup = false;
	}
}

bool ofxSip::setup(string domain, string username, string password, int port) {

	pj_status_t status;
	
	
	
	
	/* Create pjsua first! */
	status = pjsua_create();
	if (status != PJ_SUCCESS) {
		printf("Error in pjsua_create()\n");
		return false;
	}
	
	
	/* Init pjsua */
	{
		pjsua_config cfg;
		pjsua_logging_config log_cfg;
		
		pjsua_config_default(&cfg);
		cfg.cb.on_incoming_call = &on_incoming_call;
		cfg.cb.on_call_media_state = &on_call_media_state;
		cfg.cb.on_call_state = &on_call_state;
		
		pjsua_logging_config_default(&log_cfg);
		log_cfg.console_level = 4;
		
		status = pjsua_init(&cfg, &log_cfg, NULL);
		if (status != PJ_SUCCESS) {
			printf("Error in pjsua_init()\n");
		}
	
	}

	
	
		 /* Add UDP transport. */
	{
		pjsua_transport_config cfg;
		
		pjsua_transport_config_default(&cfg);
		cfg.port = port;
		status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
		if (status != PJ_SUCCESS) {
			printf("Error creating transport\n");
			return false;
		}
	}
	
	
	
	
	
	
	/* Initialization is done, now start pjsua */
	status = pjsua_start();
	if (status != PJ_SUCCESS) {
		printf("Error starting pjsua\n");
		return false;
	}

	/* Register to SIP server by creating SIP account. */
	{
		pjsua_acc_config cfg;
		
		pjsua_acc_config_default(&cfg);
		
		string cfgId = string("sip:")+username+"@"+domain;
		cfg.id = pj_str((char*)cfgId.c_str());
		
		string regUri = string("sip:") + domain;
		cfg.reg_uri = pj_str((char*)regUri.c_str());
		
		cfg.cred_count = 1;
		cfg.cred_info[0].realm = pj_str((char*)domain.c_str());
		cfg.cred_info[0].scheme = pj_str("digest");
		cfg.cred_info[0].username = pj_str((char*)username.c_str());
		cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
		cfg.cred_info[0].data = pj_str((char*)password.c_str());
		
		status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
		if (status != PJ_SUCCESS) {
			printf("Error adding account\n");
			return false;
		}
	}



	alreadySetup = true;
	return true;
}


bool ofxSip::makeCall(string url) {
	if(!alreadySetup) {
		printf("ofxSip: Failed to make a call to %s\n", url.c_str());
		return false;
	}
	pj_status_t status;
	
	// verify url
	status = pjsua_verify_sip_url(url.c_str());
	if (status != PJ_SUCCESS) {
		printf("Invalid URL '%s'\n", url.c_str());
		return false;
	}
	
	
	
	/* If URL is specified, make call to the URL. */

	pj_str_t uri = pj_str((char*)url.c_str());
	status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, NULL);
	if (status != PJ_SUCCESS) {
		printf("Error making call\n");
		return false;
	}
	


	
	
	return true;
}


void ofxSip::hangUp() {
	pjsua_call_hangup_all();
}
	
    
 