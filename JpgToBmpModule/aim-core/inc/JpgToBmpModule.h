/**
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
 * @author               Homer J. Simpson
 * @copyright            Springfield Power Company
 * @date                 nov 12, 2013
 * @license              State
 */

#ifndef JPGTOBMPMODULE_H_
#define JPGTOBMPMODULE_H_

#include <string>
#include <vector>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
// zeromq specific headers
#include <zmq.hpp>
#include <json_spirit_reader.h>
#include <pthread.h>


namespace rur {

struct Param {
  std::string module_id;
};

typedef std::vector<int> long_seq;

/**
 * Port name service record. This is like a domain name service record, but instead of containing an IP address and an
 * URI, it comes with a "name" that can be resolved as a "host", "port", and "pid". The name is something like "/write",
 * the host something like "127.0.0.1" or "dev.almende.com" (that is resolvable by dns), "port" is a TCP/UDP port, and
 * "pid" is the process identifier.
 */
typedef struct psn_record_t {
  std::string name;
  std::string host;
  std::string port;
  std::string pid;
} pns_record;

// Following structure makes it easier to store state information per socket
typedef struct zmq_socket_ext_t {
  zmq::socket_t *sock;
  std::string name;
  bool ready;
} zmq_socket_ext;

class JpgToBmpModule {
private:
  Param *cliParam;
  
  // the socket over which is communicated with the name server
  zmq::socket_t *ns_socket;
  // standard control socket over which commands arrive to connect to some port for example
  zmq::socket_t *cmd_socket;
  pthread_t cmdThread;
  pthread_mutex_t cmdMutex;
  // standard control socket over which commands arrive to connect to some port for example
  std::vector<zmq_socket_ext*> zmq_sockets;
  std::string portJpgValue;
  zmq_socket_ext portJpgIn;
  
  zmq_socket_ext portBmpOut;
  
  static void* readCommandsHelper(void* object) {
    ((JpgToBmpModule*)object)->readCommands();
    return NULL;
  }
  
  void readCommands();
protected:
  static const int channel_count = 2;
  const char* channel[2];
  // the standard zeromq context object
  zmq::context_t *context;
  // some default debug parameter
  char debug;
  /**
   * The resolve function can be called by modules to get a new socket (and if you want host name and port). It can also
   * be used by the connector, to bind to these previously set up sockets.
   */
  void Resolve(pns_record & record);
  void SendAck(zmq::socket_t *s, bool state);
  bool ReceiveAck(zmq::socket_t *s, bool & state, bool blocking);
  char* GetReply(zmq::socket_t *s, bool & state, bool blocking, int & reply_size);
  void SendRequest(zmq::socket_t *s, bool & state, bool blocking, std::string str);
  void HandleCommand();
  void Connect(std::string source, std::string target);
  zmq::socket_t* GetSocket(std::string name);

public:
  // Default constructor
  JpgToBmpModule();
  
  // Default destructor
  virtual ~JpgToBmpModule();
  
  // Extend this with your own code, first call JpgToBmpModule::Init(name);
  void Init(std::string& name);
  
  // Function to get Param struct (to subsequently set CLI parameters)
  inline Param *GetParam() { return cliParam; }
  
  // Overwrite this function with your own code
  virtual void Tick() = 0;
  
  // Overwrite this function with your own code
  bool Stop() { return false; }
  
  /**
   * The "readJpg" function receives stuff over a zeromq REP socket. It works as a client. It is better not
   * to run it in blocking mode, because this would make it impossible to receive message on other ports (under which 
   * the /pid/control port). The function returns NULL if there is no new item available.
   */
  // Read from this function and assume it means something
  // Remark: check if result is not NULL
  std::string *readJpg(bool blocking=false);
  
  /**
   * The "writeBmp" function sends stuff over a zeromq REQ socket. It works as a server. It cannot be blocking because this
   * would make it impossible to receive message on other ports (under which the /pid/control port). It could have been
   * blocking if it is known if it is connected to a REP port (but the connected() function is apparently not meant for
   * that).
   */
  // Write to this function and assume it ends up at some receiving module
  bool writeBmp(const long_seq &output);
  
};
} // End of namespace

#endif // JPGTOBMPMODULE_H_
