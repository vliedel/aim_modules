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
 * Copyright © 2013 Your Name <your@email>
 *
 * @author	Your Name
 * @date	Current date
 * @company	Your Company
 * @project	Specific Software Project
 */

#include <SwiftenModuleExt.h>
#include <Swiften/Base/Log.h>

using namespace rur;

SwiftenModuleExt::SwiftenModuleExt() {

//	Swift::Log::setLogLevel(Swift::Log::debug);
//	mNetworkFactories = new Swift::BoostNetworkFactories(&mEventLoop);
//
//	//mClient = new Swift::Client("test@dobots.customers.luna.net", "NOTlekkerINSdeinhead", mNetworkFactories);
//	mClient = new Swift::Client("test2@dobots.customers.luna.net", "replicator", mNetworkFactories);
//	//mClient = new Swift::Client("dominik@dobots.customers.luna.net/bart", "ichbinkeinberliner", mNetworkFactories);
//	mClient->setAlwaysTrustCertificates(); // TODO: this is bad!!
//	mClient->onConnected.connect(boost::bind(&SwiftenModuleExt::handleConnected, this));
//	mClient->onPresenceReceived.connect(boost::bind(&SwiftenModuleExt::handlePresenceReceived, this, _1));
//	mClient->onMessageReceived.connect(boost::bind(&SwiftenModuleExt::handleMessageReceived, this, _1));
//	mClient->getEntityCapsProvider()->onCapsChanged.connect(boost::bind(&SwiftenModuleExt::handleCapsChanged, this, _1));
//
//	mTracer = new Swift::ClientXMLTracer(mClient);
//
//	mSoftwareVersionResponder = new Swift::SoftwareVersionResponder(mClient->getIQRouter());
//	mSoftwareVersionResponder->setVersion("Dodedodo", "1.0");
//	mSoftwareVersionResponder->start();
//
////	mFileTransferManager = mClient->getFileTransferManager();
//	mClient->connect();
}

SwiftenModuleExt::~SwiftenModuleExt() {
	delete mNetworkFactories;
	mClient->onConnected.disconnect(boost::bind(&SwiftenModuleExt::handleConnected, this));
	mClient->onPresenceReceived.disconnect(boost::bind(&SwiftenModuleExt::handlePresenceReceived, this, _1));
	mClient->onMessageReceived.disconnect(boost::bind(&SwiftenModuleExt::handleMessageReceived, this, _1));
	mClient->getEntityCapsProvider()->onCapsChanged.disconnect(boost::bind(&SwiftenModuleExt::handleCapsChanged, this, _1));
	delete mClient;
	delete mTracer;
	mSoftwareVersionResponder->stop();
	delete mSoftwareVersionResponder;
//	delete mFileTransferManager;
}

void SwiftenModuleExt::Init(std::string id, std::string username, std::string password, std::string resource, std::string toResource, std::string filename) {
	SwiftenModule::Init(id);

	mFileName = filename;
	mUsername = username;
	mToResource = toResource;

	Swift::Log::setLogLevel(Swift::Log::debug);
	mNetworkFactories = new Swift::BoostNetworkFactories(&mEventLoop);

	std::string jid = username + "@dobots.customers.luna.net/" + resource;
	mClient = new Swift::Client(jid, password, mNetworkFactories);

	mClient->setAlwaysTrustCertificates(); // TODO: this is bad!!
	mClient->onConnected.connect(boost::bind(&SwiftenModuleExt::handleConnected, this));
	mClient->onPresenceReceived.connect(boost::bind(&SwiftenModuleExt::handlePresenceReceived, this, _1));
	mClient->onMessageReceived.connect(boost::bind(&SwiftenModuleExt::handleMessageReceived, this, _1));
	mClient->getEntityCapsProvider()->onCapsChanged.connect(boost::bind(&SwiftenModuleExt::handleCapsChanged, this, _1));

	mTracer = new Swift::ClientXMLTracer(mClient);

	mSoftwareVersionResponder = new Swift::SoftwareVersionResponder(mClient->getIQRouter());
	mSoftwareVersionResponder->setVersion("Dodedodo", "1.0");
	mSoftwareVersionResponder->start();

//	mFileTransferManager = mClient->getFileTransferManager();
	mClient->connect();


	std::cout << "Init" << std::endl;
	mEventLoop.run();
}

//! Replace with your own functionality
void SwiftenModuleExt::Tick() {
	sleep(1);
}

//! Replace with your own functionality
bool SwiftenModuleExt::Stop() {
	return false;
}



void SwiftenModuleExt::handleConnected() {
	std::cout << "Connected" << std::endl;
	Swift::GetRosterRequest::ref rosterRequest = Swift::GetRosterRequest::create(mClient->getIQRouter());
	rosterRequest->onResponse.connect(boost::bind(&SwiftenModuleExt::handleRosterReceived, this, _2));
	rosterRequest->send();
}

struct newline_ws: std::ctype<char> {
        static const mask* make_table() {
                static std::vector<mask> v(classic_table(), classic_table() + table_size);
                v['\n'] |= space; // newline will be classified as separator
                v[' '] &= ~space; // space is no separator
                v['.'] &= ~space; // dot is no separator
                return &v[0];
        }
        newline_ws(std::size_t refs = 0) : ctype<char>(make_table(), false, refs) {}
};

void SwiftenModuleExt::handleRosterReceived(Swift::ErrorPayload::ref error) {
	std::cout << "Roster received" << std::endl;
	if (error) {
		std::cerr << "Error receiving roster. Continuing anyway.";
	}
	// Send initial available presence
	mClient->sendPresence(Swift::Presence::create("Online"));

	std::ifstream mFs(mFileName.c_str());
//	mFs.imbue(std::locale(std::locale(), new newline_ws)); // Can also use getline

	Swift::Message::ref message(new Swift::Message);
	message->setTo(Swift::JID(mUsername + "@dobots.customers.luna.net/" + mToResource));
	message->setType(Swift::Message::Chat);

	std::string body;
	body.clear();
	std::string buffer;
	std::cout << "Start reading file" << std::endl;
	while (std::getline(mFs, buffer)) {
//		std::cout << "buffer: " << buffer << std::endl;
		if (buffer.compare(0, 1, "#", 1) == 0) {
			// a comment
		}
		else if (buffer.size() < 1) {
			if (body.size() > 0) {
				message->setBody(body);
				mClient->sendMessage(message);
				body.clear();
			}
		}
		// sleep N
		else if (buffer.compare(0, 6, "sleep ", 6) == 0) {
			int seconds = atoi(buffer.substr(6).c_str());
			if (seconds > 0)
				sleep(seconds);
		}
		else {
			if (body.size() == 0)
				body = buffer;
			else
				body += "\n" + buffer;
		}
	}
	if (body.size() > 0) {
		message->setBody(body);
		mClient->sendMessage(message);
		body.clear();
	}


//	{
//		message->setTo(Swift::JID(jid));
//		message->setBody("AIM list");
//		message->setType(Swift::Message::Chat);
//		mClient->sendMessage(message);
//	}
//	{
//		message->setTo(Swift::JID(jid));
//		message->setBody("AIM deploy { \
//				\"name\": \"BmpToJpgModule\", \
//				\"type\": \"background\", \
//				\"git\": \"https://github.com/vliedel/BmpToJpgModule_Android.git\", \
//				\"android\": { \
//					\"package\": \"org.dobots.bmptojpgmodule\", \
//					\"url\": \"https://dl.dropboxusercontent.com/u/56370/BmpToJpgModule.apk\" \
//				}, \
//				\"ports\": [ \
//					{ \
//						\"name\": \"Bmp\", \
//						\"dir\": \"in\", \
//						\"type\": \"intarray\" \
//					}, \
//					{ \
//						\"name\": \"Jpg\", \
//						\"dir\": \"out\", \
//						\"type\": \"string\" \
//					} \
//				] \
//			}");
//		message->setType(Swift::Message::Chat);
//		mClient->sendMessage(message);
//	}
//	{
//		message->setTo(Swift::JID(jid));
//		message->setBody("AIM start PictureSelectModule 0");
//		message->setType(Swift::Message::Chat);
//		mClient->sendMessage(message);
//	}
//	{
//		message->setTo(Swift::JID(jid));
//		message->setBody("AIM start BmpToJpgModule 0");
//		message->setType(Swift::Message::Chat);
//		mClient->sendMessage(message);
//	}
//	{
//		message->setTo(Swift::JID(jid));
//		message->setBody("AIM start PictureTransformModule 0");
//		message->setType(Swift::Message::Chat);
//		mClient->sendMessage(message);
//	}
//	{
//		message->setTo(Swift::JID(jid));
//		message->setBody("AIM connect local PictureSelectModule 0 image local PictureTransformModule 0 InImage");
//		message->setType(Swift::Message::Chat);
//		mClient->sendMessage(message);
//	}
//	{
//		message->setTo(Swift::JID(jid));
//		message->setBody("AIM connect local PictureTransformModule 0 OutImage local BmpToJpgModule 0 Bmp");
//		message->setType(Swift::Message::Chat);
//		mClient->sendMessage(message);
//	}
//	{
//		message->setTo(Swift::JID(jid));
//		message->setBody("AIM connect local BmpToJpgModule 0 Jpg raspberry JpgToBmpModule 0 Jpg");
//		message->setType(Swift::Message::Chat);
//		mClient->sendMessage(message);
//	}
}

void SwiftenModuleExt::handlePresenceReceived(Swift::Presence::ref presence) {
	std::cout << "Presence received" << std::endl;
//	if (presence->getType() == Swift::Presence::Subscribe) {
//		Swift::Presence::ref response = Swift::Presence::create();
//		response->setTo(presence->getFrom());
//		response->setType(Swift::Presence::Subscribed);
//		mClient->sendPresence(response);
//	}

//	std::cout << "Writing message.." << std::endl;
//	Swift::Message::ref message(new Swift::Message);
//	message->setTo(Swift::JID("test2@dobots.customers.luna.net/GT-I9000"));
////	message->setTo(Swift::JID("vliedel@dobots.customers.luna.net"));
//	message->setBody("AIM list");
//	message->setType(Swift::Message::Chat);
//	mClient->sendMessage(message);
}

void SwiftenModuleExt::handleCapsChanged(Swift::JID jid) {
//	boost::filesystem::path file("/home/vliedel/DO/aim_modules/README.md");
//	boost::shared_ptr<Swift::FileReadBytestream> fileStream = boost::make_shared<Swift::FileReadBytestream>(file);
//	Swift::OutgoingFileTransfer::ref ft = mClient->getFileTransferManager()->createOutgoingFileTransfer(
//			jid,
//			file,
//			"test",
//			fileStream
//			);
//	if (ft) {
//		std::cout << "Started file transfer" << std::endl;
//		ft->start();
//	} else {
//		std::cout << jid << " doesn't support any kind of file transfer" << std::endl;
//	}
}

void SwiftenModuleExt::handleMessageReceived(Swift::Message::ref message) {
	if (message->getBody().size() > 0) {
		std::cout << "Message received: " << message->getBody() << std::endl;

//		boost::filesystem::path file("/home/vliedel/DO/aim_modules/README.md");
//		boost::shared_ptr<Swift::FileReadBytestream> fileStream = boost::make_shared<Swift::FileReadBytestream>(file);
//		Swift::OutgoingFileTransfer::ref ft = mClient->getFileTransferManager()->createOutgoingFileTransfer(
//				message->getFrom(),
//				file,
//				"test",
//				fileStream
//				);
//		if (ft) {
//			std::cout << "Started file transfer to " << message->getFrom() << std::endl;
//			ft->start();
//		} else {
//			std::cout << message->getFrom() << " doesn't support any kind of file transfer" << std::endl;
//		}
	}
}
