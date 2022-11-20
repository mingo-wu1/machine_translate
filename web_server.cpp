#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& user) {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayHello(&context, request, &reply); // 请求

    // Act upon its status.
    if (status.ok()) {
      std::cout << reply.fuck() << std::endl;
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

int grpc_client(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint specified by
  // the argument "--target=" which is the only expected argument.
  // We indicate that the channel isn't authenticated (use of
  // InsecureChannelCredentials()).
  std::string target_str;
  std::string arg_str("--target");
  if (argc > 1) {
    std::string arg_val = argv[1];
    size_t start_pos = arg_val.find(arg_str);
    if (start_pos != std::string::npos) {
      start_pos += arg_str.size();
      if (arg_val[start_pos] == '=') {
        target_str = arg_val.substr(start_pos + 1);
      } else {
        std::cout << "The only correct argument syntax is --target="
                  << std::endl;
        return 0;
      }
    } else {
      std::cout << "The only acceptable argument is --target=" << std::endl;
      return 0;
    }
  } else {
    target_str = "localhost:50051";
  }
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;

  return 0;
}

////////////////////////////////

#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }

    std::string target_str = "localhost:50051";
    GreeterClient greeter(
        grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    std::string user("world");
    std::string reply = greeter.SayHello(user);
    std::cout << "Greeter received: " << reply << std::endl;
}

int web_server() {
    // Create a server endpoint
    server echo_server;

    try {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(bind(&on_message,&echo_server,::_1,::_2));

        // Listen on port 9002
        echo_server.listen(websocketpp::lib::asio::ip::tcp::v4(),10002);

        // Start the server accept loop
        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
    return 0;
}

extern "C"{
#include "web_server.h"
}

int main(int argc, char** argv){
    ngx_buf_t        *b; // 链表
    ngx_log_t        *log; // 链表 struct ngx_log_s {ngx_uint_t log_level;ngx_open_file_t *file;ngx_atomic_uint_t connection;time_t disk_full_time;ngx_log_handler_pt handler;void *data;ngx_log_writer_pt writer;void *wdata;char *action;ngx_log_t *next;};
    ngx_uint_t        i; // u int
    ngx_conf_dump_t  *cd;
    ngx_core_conf_t  *ccf;

    ngx_debug_init(); // # define ngx_debug_init() 空的

    if (ngx_strerror_init() != NGX_OK) { // 系统的error字符串使用迭代方式赋值给ngx_sys_errlist数组{int len, char* data}
        return 1;
    }

    if (ngx_get_options(argc, argv) != NGX_OK) { // switch case
        return 1;
    }

    if (ngx_show_version) { // ngx_get_options通过switch case获取的case 'h':ngx_show_version = 1;ngx_show_help = 1;break;case 'v':ngx_show_version = 1;break;case 'V':ngx_show_version = 1;

        ngx_show_version_info(); // 打印help信息

        if (!ngx_test_config) { //case 't':ngx_test_config = 1;break;case 'T':ngx_test_config = 1;
            return 0;
        }
    }

    /* TODO */ ngx_max_sockets = -1;

    ngx_time_init(); // 初始化nginx环境的当前时间, 了解原理就行, ngx_time_update 重要

#if (NGX_PCRE)
    ngx_regex_init();
#endif

    ngx_pid = ngx_getpid(); // getpid() #define ngx_getpid   getpid       #define ngx_getppid  getppid
    ngx_parent = ngx_getppid(); // getppid()

    log = ngx_log_init(ngx_prefix, ngx_error_log); // (uchar*,uchar*)主进程启动的时候，将一些出错内容或者结果输到标准输出，调用ngx_log_init 函数后，默认日志文件为：安装路径/logs/error.log，如果这个文件没有权限访问的话，会直接报错退出。在mian函数结尾处，在ngx_master_process_cycle函数调用之前，会close掉这个日志文件。
    if (log == NULL) { // log 结构体 就是 struct ngx_log_s{ngx_uint_t log_level(日志等级);ngx_open_file_t *file(句柄和文件名);......} , 而 ngx_open_file_t 就是  struct ngx_open_file_s {ngx_fd_t fd(int类型); ngx_str_t name(文件名);......} , typedef struct {size_t len;u_char *data;} ngx_str_t; (就是文件名) , typedef int ngx_fd_t;
        return 1; 
    }

    /* STUB */
#if (NGX_OPENSSL)
    ngx_ssl_init(log);
#endif

    if (ngx_os_init(log) != NGX_OK) { // 调用ngx_os_init()初始化系统相关变量，如内存页面大小ngx_pagesize,ngx_cacheline_size,最大连接数ngx_max_sockets等; 可能这里不需要
        return 1;
    }

    /*
     * ngx_crc32_table_init() requires ngx_cacheline_size set in ngx_os_init()
     */

    if (ngx_crc32_table_init() != NGX_OK) { // 调用ngx_crc32_table_init()初始化CRC表(后续的CRC校验通过查表进行，效率高);
        return 1;
    }

    /*
     * ngx_slab_sizes_init() requires ngx_pagesize set in ngx_os_init()
     */

    ngx_slab_sizes_init(); // 无数据结构, 就变量 slab 大小的初始化 nginx 通过自己实现的slab机制来减少内存的碎片化 而nginx的slab机制相对于linux内核的slab机制就显得相对的简单, 通过nginx可以更快的理解slab机制。

    // typedef struct ngx_module_s      ngx_module_t;
    // struct ngx_module_s {
    //     ngx_uint_t            ctx_index;
    //     ngx_uint_t            index;
    //     ngx_uint_t            spare0;
    //     ngx_uint_t            spare1;
    //     ngx_uint_t            abi_compatibility;
    //     ngx_uint_t            major_version;
    //     ngx_uint_t            minor_version;
    //     void                 *ctx;
    //     ngx_command_t        *commands;
    //     ngx_uint_t            type;

    // #define NGX_MODULE_V1                                                         \
    //     NGX_MODULE_UNSET_INDEX, NGX_MODULE_UNSET_INDEX,                           \
    //     NULL, 0, 0, nginx_version, NGX_MODULE_SIGNATURE
    // ngx_module_t  ngx_conf_module = {
    //     NGX_MODULE_V1,
    //     NULL,                                  /* module context */
    //     ngx_conf_commands,                     /* module directives */
    //     NGX_CONF_MODULE,                       /* module type */
    //     NULL,                                  /* init master */
    //     NULL,                                  /* init module */
    //     NULL,                                  /* init process */
    //     NULL,                                  /* init thread */
    //     NULL,                                  /* exit thread */
    //     ngx_conf_flush_files,                  /* exit process */
    //     NULL,                                  /* exit master */
    //     NGX_MODULE_V1_PADDING
    // };

    if (ngx_preinit_modules() != NGX_OK) { // 迭代模块数组,  对模块打标, 主要是前置的初始化模块，对模块进行编号处理 // ngx_module_t *ngx_modules[] = {&ngx_core_module,&ngx_errlog_module,&ngx_conf_module,// &ngx_regex_module,&ngx_events_module,&ngx_event_core_module,&ngx_epoll_module,....}
        return 1;
    }

    ngx_use_stderr = 0;

    web_server();
}
