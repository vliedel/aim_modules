/**
 * @file SwiftenModuleExt.cpp
 * @brief SwiftenModule extension
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software being used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 *
 * @author  ...
 * @date    ...
 * @company Almende B.V.
 * @case    Artificial Intelligence Framework
 */

#include <SwiftenModule.h>

#include <iostream>
#include <boost/bind.hpp>
#include <Swiften/Swiften.h>

namespace rur {

class SwiftenModuleExt: public SwiftenModule {
public:
	SwiftenModuleExt();
	~SwiftenModuleExt();

	// The tick function will be called from the SwiftenModuleMain file
	void Tick();

	// As soon as Stop() returns "true", the SwiftenModuleMain will stop the module
	bool Stop();

	void Init(std::string id, std::string username, std::string password, std::string resource, std::string toResource, std::string filename);
private:
	Swift::SimpleEventLoop mEventLoop;
	//boost::BoostNetworkFactories *networkFactories;
	Swift::BoostNetworkFactories *mNetworkFactories;

	Swift::Client* mClient;
	Swift::ClientXMLTracer* mTracer;
	Swift::SoftwareVersionResponder* mSoftwareVersionResponder;
	std::string mFileName;
	std::string mUsername;
	std::string mToResource;

	// Callbacks
	void handleConnected();
	void handleRosterReceived(Swift::ErrorPayload::ref error);
	void handlePresenceReceived(Swift::Presence::ref presence);
	void handleCapsChanged(Swift::JID jid);
	void handleMessageReceived(Swift::Message::ref message);
};

}
