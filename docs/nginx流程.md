nginx配置
```
https://www.nginx.org.cn/article/detail/545
```

#### nginx_conf_parse

![nginx_conf_parse](images/nginx_conf_parse.png)

#### nginx_conf_ctx

![nginx_conf_ctx](images/nginx_conf_ctx.png)

#### nginx_module_t

![nginx_module_t](images/nginx_module_t.png)



#### ngx_init_cycle

![ngx_init_cycle](images/ngx_init_cycle.jpeg)

#### nginx_http (https://blog.51cto.com/u_15284125/5191796)

![nginx_cycle_init](images/nginx_http.jpg)

#### nginx_cycle_init

![nginx_cycle_init](images/nginx_cycle_init.png)

#### nginx

![nginx](images/nginx.png)

#### NginxHTTP模块调度流程

![NginxHTTP模块调度流程](images/NginxHTTP模块调度流程.png)

#### Nginx模块类图

![Nginx模块类图](images/Nginx模块类图.png)

#### NginxFeature

![NginxFeature](images/NginxFeature.png)

#### Nginx并发模型

![Nginx并发模型](images/Nginx并发模型.png)

```
typedef ngx_cycle_s ngx_cycle_t;

struct ngx_cycle_s { // 链表
    /* 保存着所有模块存储配置项的结构体指针，它首先是一个数组，每个数组成员又是一个指针，
    这个指针指向另一个存储着指针的数组 */
    void                  ****conf_ctx;
    //内存池
    ngx_pool_t               *pool; // 链表

    /* 日志模块中提供生成计本ngx_log_t日志对象的功能，这里的log是还没有解析配置前，重定向到屏幕的。
    在ngx_init_cycle函数执行后，会更需nginx.conf中的配置项重新构造出log */
    ngx_log_t                *log; // 链表
    /* new_log暂时存储根据nginx.conf生成的新log，之后会赋值给上述log */
    ngx_log_t                 new_log; // 一个链表单元

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    /* 对于poll、rtsig这样的事件模块，会以有效文件句柄来预先建立ngx_connection_t结构体，以加速事件
    的收集和分发。这时，files就会保存所有ngx_connection_t的指针组成的数组。 */
    ngx_connection_t        **files; // 二维结构体数组
    //可用连接池
    ngx_connection_t         *free_connections; // 结构体数组
    //可用连接池的总数
    ngx_uint_t                free_connection_n;

    //可以重复使用连接队列
    ngx_queue_t               reusable_connections_queue; //队列

    //监听端口数组，ngx_listening_t
    ngx_array_t               listening; // 数组
    //保存着Nginx所有要操作的目录
    ngx_array_t               paths; // 数组
    //ngx_open_file_t结构体组成的数组，表示Nginx已经打开的所有文件。由模块添加，ngx_init_cycle中打开
    ngx_list_t                open_files; // 结构体 + 内部链表
    //ngx_shm_zone_t组成的数组，每个元素表示一块共享内存
    ngx_list_t                shared_memory; // 结构体 + 内部链表

    //当前进程中所有连接对象的总数
    ngx_uint_t                connection_n; // u int
    //上述files中存在的有效文件句柄个数
    ngx_uint_t                files_n; // u int

    //指向当前进程中的所有连接对象
    ngx_connection_t         *connections; // 结构体数组
    //指向当前进程中的所有读事件对象
    ngx_event_t              *read_events; // 结构体数组
    //指向当前进程中所有写事件对象
    ngx_event_t              *write_events; // 结构体数组

    //用于引用上一个ngx_cycle_t对象
    ngx_cycle_t              *old_cycle; // 链表

    //配置文件相对于安装目录的路径名称
    ngx_str_t                 conf_file; // 字符串结构体    typedef struct {size_t      len;u_char     *data;} ngx_str_t;
    //处理配置文件时需要特殊处理的命令行携带参数
    ngx_str_t                 conf_param;
    //配置文件所在目录的路径
    ngx_str_t                 conf_prefix;
    //Nginx安装目录的路径
    ngx_str_t                 prefix;
    //用于进程间同步文件锁名称
    ngx_str_t                 lock_file;
    //gethostname得到的主机名
    ngx_str_t                 hostname;
};

// 至少是一个链表
    typedef struct ngx_pool_s        ngx_pool_t;

    struct ngx_pool_s {
        ngx_pool_data_t       d;
        size_t                max;
        ngx_pool_t           *current;
        ngx_chain_t          *chain;
        ngx_pool_large_t     *large;
        ngx_pool_cleanup_t   *cleanup;
        ngx_log_t            *log;
    };

// 数组
    typedef struct ngx_array_s       ngx_array_t;
    struct ngx_array_s {
        void        *elts;
        ngx_uint_t   nelts;
        size_t       size;
        ngx_uint_t   nalloc;
        ngx_pool_t  *pool;
    };

// 结构体 + 内部链表
    typedef struct {
        ngx_list_part_t  *last;
        ngx_list_part_t   part;
        size_t            size;
        ngx_uint_t        nalloc;
        ngx_pool_t       *pool;
    } ngx_list_t;

:last: 指向该链表的最后一个节点。
:part: 该链表的首个存放具体元素的节点。
:size: 链表中存放的具体元素所需内存大小。
:nalloc: 每个节点所含的固定大小的数组的容量。
:pool: 该list使用的分配内存的pool。

好，我们在看一下每个节点的定义。

.. code:: c

    typedef struct ngx_list_part_s  ngx_list_part_t;
    struct ngx_list_part_s {
        void             *elts;
        ngx_uint_t        nelts;
        ngx_list_part_t  *next;
    };

ngx_cycle_s结构体支持的主要方法
函数名	参数含义	执行意义
ngx_cycle_t *ngx_init_cycle(ngx_cycle_t *old_cycle)	old_cycle表示临时的ngx_cycle_t指针，一般仅用来传递ngx_cycle_t结构体中的配置文件路径等	返回初始化成功的完整的ngx_cycle_t结构体
ngx_int_t ngx_process_options(ngx_cycle_t *cycle)	cycle通常是刚刚分配的ngx_cycle_t结构体指针	用运行Nginx可能携带的目录参数来初始化cycle
ngx_int_t ngx_add_inherited_sockets(ngx_cycle_t *cycle)	同上	通过该函数来使用已经打开的TCP监听端口
ngx_int_t ngx_open_listening_sockets(ngx_cycle_t *cycle)	同上	监听、绑定cycle中的listening动态数组指定的相应端口
void ngx_configure_listening_sockets(ngx_cycle_t *cycle)	同上	根据nginx.conf中的配置项设置已经监听的句柄
void ngx_close_listening_sockets(ngx_cycle_t *cycle)	同上	关闭cycle的listening中的套接字
void ngx_master_process_cycle(ngx_cycle_t *cycle)	同上	进入master进程的工作循环
void ngx_single_process_cycle(ngx_cycle_t *cycle)	同上	进入单进程（非master、worker进程工作模式的工作循环
void ngx_start_cache_manager_processes(ngx_cycle_t *cycle,ngx_channel_t *ch)	同上，ch是将要向子进程发送的信息	向所有已经打开的channel（通过socketpair生成的句柄进行通信）发送ch信息
void ngx_signal_worker_processes(ngx_cycle_t *cycle,int signo)	同上，signo是信号	处理worker进程接收到的信号
ngx_uint_t ngx_reap_children(ngx_cycle_t *cycle)	同上	检查master进程的所有子进程，根据每个子进程的状态判断是否要启动子进程
void ngx_master_process_exit(ngx_cycle_t *t)	同上	退出master工作循环
void ngx_worker_process_cycle(ngx_cycle_t *cycle,void *data)	同上	进入worker工作循环
void ngx_worker_process_init(ngx_cycle_t *cycle,ngx_uint_t priority）	同上，priority是worker进程的系统优先级	进入worker进程工作循环之前的初始化工作
void ngx_cache_manager_process_cycle(ngx_cycle_t *cycle，void *data)	同上，data是传入ngx_cache_manager_ctx_t结构体的指针	执行缓存管理工作的循环方法
void ngx_worker_process_exit(ngx_cycle_t *cycle)	同上	退出worker进程工作循环
void ngx_process_events_and_timers(ngx_cycle_t *cycle)	同上	使用事件模块处理截止到目前已收到的事件
```
command
```
配置项的相关属性是由ngx_command_t这个结构体设置的,与conf 里面的配置有关
```
配置项
```
https://blog.csdn.net/ZX714311728/article/details/62237743

1. ngx_command_t

为了统一配置项目的解析，Nginx 定义了如下数据类型对所有的 Nginx 配置项进行了统一的描述。

typedef struct ngx_command_s         ngx_command_t;

struct ngx_command_s {
    /*
     * 该配置指令的名称，如 daemon, worker_processes 等
     */
    ngx_str_t             name;
    /*
     * 该配置项的类型，指定配置项可以出现的位置。如，出现在 server{}
     * 或 location{} 中，以及它可以携带的参数个数
     */
    ngx_uint_t            type;
    /*
     * 当 Nginx 在解析配置的时候，如果遇到这个配置指令，将会把读取到的值
     * 传递给这个函数进行处理.
     * @cf: 保存从配置文件读取到原始字符串以及相关的一些信息。这个参数的
     *      args 字段是一个 ngx_array_t 类型的数组，该数组的首个元素是这个
     *      配置指令本身，第二个元素是指令的第一个参数，第三个元素是第二个
     *      参数，以此类推.
     * @cmd: 这个配置指令对应的 ngx_command_t 结构
     * @conf: 就是定义的存储这个配置值的结构体，即该配置指令所在模块的配置信息
     *        结构体.
     */
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
    /*
     * 该字段主要被 HTTP/RTMP 系列模块所用，该字段指定当前配置项存储的内存
     * 位置。实际上是使用哪个内存池的问题。因为 http/rtmp 模块对所有 http 模块
     * 都要保存的配置信息，划分了 main，server，location 三个地方进行存储，
     * 每个地方都有一个内存池用来分配存储这些信息的内存。这里可能的值有
     * NGX_HTTP_MAIN_CONF_OFFSET(直接为 0 表示该项)、NGX_HTTP_SRV_CONF_OFFSET 以及
     * NGX_HTTP_LOC_CONF_OFFSET
     */
    ngx_uint_t            conf;
    /*
     * 指定该配置项值的精确存放位置，一般指定为某个结构体变量的字段偏移。因为对于
     * 配置信息的存储，一般我们都是定义一个结构体来存储。比如定义了结构体 A，该项
     * 配置的值需要存储到该结构体的 b 字段，那么这里可以填写 offsetof(A, b)。
     * 对于有些配置项，若它的值不需要保存或者是需要保存到更为复杂的结构中时，直接
     * 设为 0.
     */
    ngx_uint_t            offset;
    /*
     * 该字段存储一个指针，可以指向任何一个在读取配置项过程需要的数据，以便于进行
     * 配置读取的处理。大多数时候，都不需要该值，直接设为 0 即可.
     */
    void                 *post;
};

2. ngx_conf_t

typedef struct ngx_conf_s            ngx_conf_t;

struct ngx_conf_s {
    char                 *name;
    /* 
     * 用于存放该配置项的元素字符串，它是一个数组，假设当前配置项为"daemon off;"
     * 则 args[0] 指向 "daemon"，args[1] 指向 "off".
     */
    ngx_array_t          *args;

    /* 
     * 指向当前的核心结构体 ngx_cycle_t
     */
    ngx_cycle_t          *cycle;
    /*
     * 指向核心结构体所用的内存池
     */
    ngx_pool_t           *pool;
    ngx_pool_t           *temp_pool;
    /* 保存的是配置文件的信息 */
    ngx_conf_file_t      *conf_file;
    ngx_log_t            *log;

    void                 *ctx;
    /* 
     * 当前解析配置项所在的模块的类型 
     */
    ngx_uint_t            module_type;
    ngx_uint_t            cmd_type;

    ngx_conf_handler_pt   handler;
    char                 *handler_conf;
};

ngx_open_file_s

struct ngx_open_file_s {
    ngx_fd_t              fd;
    ngx_str_t             name;

    void                (*flush)(ngx_open_file_t *file, ngx_log_t *log);
    void                 *data;
};

ngx_conf_file_t

typedef struct ngx_file_s            ngx_file_t;

struct ngx_file_s {
    /*
     * 打开的文件描述符
     */
    ngx_fd_t                   fd;
    /*
     * 保存该文件的绝对路径
     */
    ngx_str_t                  name;
    /*
     * ngx_file_info_t 即 struct stat 结构体类型
     */
    ngx_file_info_t            info;

    off_t                      offset;
    off_t                      sys_offset;

    ngx_log_t                 *log;

#if (NGX_THREADS || NGX_COMPAT)
    ngx_int_t                (*thread_handler)(ngx_thread_task_t *task,
                                               ngx_file_t *file);
    void                      *thread_ctx;
    ngx_thread_task_t         *thread_task;
#endif

#if (NGX_HAVE_FILE_AIO || NGX_COMPAT)
    ngx_event_aio_t           *aio;
#endif

    unsigned                   valid_info:1;
    unsigned                   directio:1;
};

typedef struct ngx_buf_s  ngx_buf_t;

struct ngx_buf_s {
    /* 
     * pos通常是用来告诉使用者本次应该从pos这个位置开始处理内存中的数据，
     * 这样设置是因为同一个ngx_buf_t可能被多次反复处理。当然，pos的含义
     * 是由使用它的模块定义的 
     */
    u_char          *pos;
    /* 
     * last通常表示有效的内容到此为止，注意，pos与last之间的内存是希望nginx
     * 处理的内容 
     */
    u_char          *last;
    /* 
     * 处理文件时，file_pos与file_last的含义与处理内存时的pos与last相同，
     * file_pos表示将要处理的文件位置，file_last表示截止的文件位置 
     */
    off_t            file_pos;
    off_t            file_last;

    /* 
     * 如果ngx_buf_t缓冲区用于内存，那么start指向这段内存的起始地址
     */
    u_char          *start;         /* start of buffer */
    /* 
     * 与start成员对应，指向缓冲区内存的末尾 
     */
    u_char          *end;           /* end of buffer */
    /* 
     * 表示当前缓冲区的类型，例如由哪个模块使用就指向这个模块
     * ngx_module_t 变量的地址 
     */
    ngx_buf_tag_t    tag;
    /* 
     * 引用的文件 
     */   
    ngx_file_t      *file;
    /* 
     * 当前缓冲区的影子缓冲区，该成员很少用到 
     */
    ngx_buf_t       *shadow;


    /* 
     * 临时内存标志位，为1时表示数据在内存中且这段数据可以修改 
     */
    /* the buf's content could be changed */
    unsigned         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    unsigned         memory:1;

    /* the buf's content is mmap()ed and must not be changed */
    unsigned         mmap:1;

    /* 
     * 标志位，为1时表示可回收 
     */
    unsigned         recycled:1;
    /* 
     * 标志位，为1时表示这段缓冲区处理的是文件而不是内存 
     */
    unsigned         in_file:1;
    /* 
     * 标志位，为1时表示需要执行flush操作 
     */
    unsigned         flush:1;
    /* 
     * 标志位，对于操作这块缓冲区时是否使用同步方式，需谨慎考虑，这可能会阻塞Nginx进程，
     * Nginx中所有操作几乎都是异步的，这是它支持高并发的关键。有些框架代码在sync为1时
     * 可能会用阻塞的方式进行I/O操作，它的意义视使用它的Nginx模块而定
     */
    unsigned         sync:1;
    /* 
     * 标志位，表示是否是最后一块缓冲区，因为ngx_buf_t可以由ngx_chain_t链表串联起来，
     * 因此，当last_buf为1时，表示当前是最后一块待处理的缓冲区
     */
    unsigned         last_buf:1;
    /* 
     * 标志位，表示是否是ngx_chain_t中的最后一块缓冲区 
     */
    unsigned         last_in_chain:1;

    /* 
     * 标志位，表示是否是最后一个影子缓冲区，与shadow域配合使用。通常不建议使用它 
     */
    unsigned         last_shadow:1;
    /* 
     * 标志位，表示当前缓冲区是否属于临时文件 
     */
    unsigned         temp_file:1;

    /* STUB */ int   num;
};


typedef struct {
    /*
     * 保存配置文件的信息
     */
    ngx_file_t            file;
    /*
     * 储存从配置文件中读取到的数据
     */
    ngx_buf_t            *buffer;
    ngx_buf_t            *dump;
    /*
     * 记录当前正在解析的行号
     */
    ngx_uint_t            line;
} ngx_conf_file_t;

ngx_conf_dump_t

typedef struct {
    ngx_str_t             name;
    ngx_buf_t            *buffer;
} ngx_conf_dump_t;

3. ngx_conf_parse

该函数是开始解析配置文件的入口函数。

/*
 * 该函数是一个间接递归函数，就是虽然在该函数体内看不到直接对其本身的调用，
 * 但是它执行的一些其他函数(如ngx_conf_handler())内会有调用到ngx_conf_parse()
 * 函数，从而形成递归。这一般在处理复杂配置项和一些特殊配置指令时发生.
 *
 * 该函数总体分为三个步骤:
 * 1. 判断当前解析状态
 * 2. 读取配置标记 token
 * 3. 当读取了合适数量的标记 token 后对其进行实际的处理，也就是将配置值转换为
 *    Nginx 内对应控制变量的值.
 *
 * 当进入到 ngx_conf_parse() 函数时，首先做的第一步就是判断当前解析过程处在一个
 * 什么样的状态，这有三种可能:
 * 1. 正要开始解析一个配置文件: 此时的参数filename指向一个配置文件路径字符串，
 *    需要函数ngx_conf_parse() 打开该文件并获取相关的文件信息(比如文件描述符等)
 *    以便下面代码读取文件内容并进行解析。此外还有一种属于此种情况，就是当遇到
 *    include 指令时也将以这种状态调用 ngx_conf_parse() 函数，因为 include 指令
 *    表示一个新的配置文件要开始解析。状态标记为type=parse_file;
 *
 * 2. 正要开始解析一个复杂配置项值：此时配置文件已经打开并且也已经对文件进行了
 *    部分解析，当遇到复杂配置项比如events、http 等时，这些复杂配置项的处理函数
 *    又会递归调用 ngx_conf_parse() 函数，此时解析的内容还是来自当前的配置文件，
 *    因此无需再次打开它，状态标记为type=parse_block;
 *
 * 3. 正要开始解析命令行参数配置项值：在对用户通过命令行 -g 参数输入的配置信息
 *    进行解析时处于这种状态，如nginx -g 'daemon on;'，Nginx在调用ngx_conf_parse()
 *    函数对命令行参数配置信息'daemon on;'进行解析时就是这种状态，状态标记为
 *    type=parse_param.
 */
char *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    char             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    enum {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#if (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = NULL;
#endif

    /* 首先判断当前解析状态 */
    if (filename) {

        /* open configuration file */

        /* 以只读方式打开配置文件 */
        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);
        if (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            return NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        /* 该宏即为 fstat，获取配置文件的信息，保存到 file.info 中 */
        if (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        cf->conf_file->buffer = &buf;

        /* 为这块缓存分配内存，用于存储从配置文件中读取的数据 */
        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        if (buf.start == NULL) {
            goto failed;
        }

        /* pos通常是用来告诉使用者本次应该从pos这个位置开始处理内存中的数据，
         * 这样设置是因为同一个ngx_buf_t可能被多次反复处理。当然，pos的含义
         * 是由使用它的模块定义的 */
        buf.pos = buf.start;
        /* last通常表示有效的内容到此为止，注意，pos与last之间的内存是希望nginx
         * 处理的内容，由于该缓存中还没有数据，因此置为 start  */
        buf.last = buf.start;
        /* 与start成员对应，指向缓冲区内存的末尾 */
        buf.end = buf.last + NGX_CONF_BUFFER;
        /* 标志位，为 1 表示数据在内存中且这段数据可以被修改 */
        buf.temporary = 1;

        /* 将该配置文件的信息赋值给 cf->conf_file */
        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.data = filename->data;
        cf->conf_file->file.offset = 0;
        cf->conf_file->file.log = cf->log;
        cf->conf_file->line = 1;

        /* 当首次开始解析配置文件时，类型为 parse_file */
        type = parse_file;

        if (ngx_dump_config
#if (NGX_DEBUG)
            || 1
#endif
           )
        {
            if (ngx_conf_add_dump(cf, filename) != NGX_OK) {
                goto failed;
            }

        } else {
            cf->conf_file->dump = NULL;
        }

    /* 由开头的分析知，开始解析复杂配置项时，为此类型 */
    } else if (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        type = parse_block;

    /* 若为解析命令行参数配置项时，为此类型 */
    } else {
        type = parse_param;
    }

    
    /* 判断好当前解析状态后就开始读取配置文件内容，配置文件是由一个个token组成的，
     * 因此接下来就是循环从配置文件里读取token. */
    for ( ;; ) {
        rc = ngx_conf_read_token(cf);

        /*
         * ngx_conf_read_token() may return
         *
         *    NGX_ERROR             there is error
         *    NGX_OK                the token terminated by ";" was found
         *    NGX_CONF_BLOCK_START  the token terminated by "{" was found
         *    NGX_CONF_BLOCK_DONE   the "}" was found
         *    NGX_CONF_FILE_DONE    the configuration file is done
         */

        if (rc == NGX_ERROR) {
            goto done;
        }

        /* 若返回值表示当前块解析结束 */
        if (rc == NGX_CONF_BLOCK_DONE) {

            if (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        /* 若返回值表示解析配置文件结束 */
        if (rc == NGX_CONF_FILE_DONE) {

            if (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        /* 若返回值表示开始解析复杂配置项 */
        if (rc == NGX_CONF_BLOCK_START) {

            if (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        
        if (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            if (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            if (rv == NGX_CONF_OK) {
                continue;
            }

            if (rv == NGX_CONF_ERROR) {
                goto failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, rv);

            goto failed;
        }

        /* 遍历所有的模块，找到与该当前读取到的指令长度和名称都相同的
         * ngx_command_t，然后调用该 ngx_comand_t 的 set 回调函数，
         * 将读取到的指令参数设置到该模块的配置信息结构体中的相应字段 */
        rc = ngx_conf_handler(cf, rc);

        if (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:

    if (filename) {
        if (cf->conf_file->buffer->start) {
            ngx_free(cf->conf_file->buffer->start);
        }

        if (ngx_close_file(fd) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_ALERT, cf->log, ngx_errno,
                          ngx_close_file_n " %s failed",
                          filename->data);
            rc = NGX_ERROR;
        }

        cf->conf_file = prev;
    }

    if (rc == NGX_ERROR) {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}

3.1 ngx_conf_read_token

static ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_line;
    ngx_str_t   *word;
    ngx_buf_t   *b, *dump;

    found = 0;
    need_space = 0;
    last_space = 1;
    /* 注释标志位 */
    sharp_comment = 0;
    variable = 0;
    /* 单/双引号标志位 */
    quoted = 0;
    /* 单引号标志位 */
    s_quoted = 0;
    /* 双引号标志位 */
    d_quoted = 0;

    cf->args->nelts = 0;
    b = cf->conf_file->buffer;
    dump = cf->conf_file->dump;
    /* 指向当前开始解析的位置 */
    start = b->pos;
    /* 当前解析的行 */
    start_line = cf->conf_file->line;

    /* 配置文件的大小 */
    file_size = ngx_file_size(&cf->conf_file->file.info);

    for ( ;; ) {

        /* 当 pos 大于等于 last 时，表示当前缓存中没有待解析的有效数据
         * 需要从配置文件中读取新的数据 */
        if (b->pos >= b->last) {

            /* 若该配置文件的偏移值已经大于等于文件大小，说明配置文件的数据
             * 已经读取完毕 */
            if (cf->conf_file->file.offset >= file_size) {

                if (cf->args->nelts > 0 || !last_space) {

                    if (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        return NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "unexpected end of file, "
                                  "expecting \";\" or \"}\"");
                    return NGX_ERROR;
                }

                /* 配置文件数据读取完毕，且没有出现错误，则返回该标志 */
                return NGX_CONF_FILE_DONE;
            }

            /* 若文件还没有读取完毕，则计算当前buf缓存中已经解析过的数据大小 */
            len = b->pos - start;

            if (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                if (d_quoted) {
                    ch = '"';

                } else if (s_quoted) {
                    ch = '\'';

                } else {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       10, start);
                    return NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                return NGX_ERROR;
            }

            if (len) {
                ngx_memmove(b->start, start, len);
            }

            /* 计算当前配置文件剩余待解析的数据大小 */
            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            if (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            /* 从配置文件中读取 size 字节到 b->start + len 指向的缓存处,
             * file.offset 保存此次从配置文件中读取的字节数，返回值 n 为
             * 读取到的字节数 */
            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            if (n == NGX_ERROR) {
                return NGX_ERROR;
            }

            /* 若实际读取到的字节数 n 与指定要求读取的字节数不相等，则
             * 表明发生错误了 */
            if (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                return NGX_ERROR;
            }

            /* pos 指向当前待解析的有效内容的起始处 */
            b->pos = b->start + len;
            /* last 指向当前待解析的有效内容的末尾 */
            b->last = b->pos + n;
            start = b->start;

            if (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        /* 取出一个字符 */
        ch = *b->pos++;

        /* 若当前字符为换行符 '\n' */
        if (ch == LF) {
            /* 行计数器加 1 */
            cf->conf_file->line++;

            /* 若该标志位为 1，表示当前行为注释 */
            if (sharp_comment) {
                /* 重置该标志位为 0 */
                sharp_comment = 0;
            }
        }

        /* 若当前行为注释，则回到循环开始，重新开始读取数据 */
        if (sharp_comment) {
            continue;
        }

        if (quoted) {
            quoted = 0;
            continue;
        }

        if (need_space) {
            if (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                last_space = 1;
                need_space = 0;
                continue;
            }

            if (ch == ';') {
                return NGX_OK;
            }

            if (ch == '{') {
                return NGX_CONF_BLOCK_START;
            }

            if (ch == ')') {
                last_space = 1;
                need_space = 0;

            } else {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected \"%c\"", ch);
                return NGX_ERROR;
            }
        }

        if (last_space) {
            /* 若当前字符为以下这些字符，则回到循环开始继续取下一个字符 */
            if (ch == ' ' || ch == '\t' || ch == CR || ch == LF) {
                continue;
            }

            /* 若当前读取到的是注释，下面这两个变量可忽略 */
            /* 记录当前读取指令的起始位置 */
            start = b->pos - 1;
            /* 记录当前读取指令的起始行 */
            start_line = cf->conf_file->line;

            switch (ch) {

            case ';':
            case '{':
                if (cf->args->nelts == 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"%c\"", ch);
                    return NGX_ERROR;
                }

                if (ch == '{') {
                    return NGX_CONF_BLOCK_START;
                }

                return NGX_OK;

            /* 为该字符，表明当前的复杂配置项的内容已经读取完毕 */
            case '}':
                if (cf->args->nelts != 0) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "unexpected \"}\"");
                    return NGX_ERROR;
                }

                return NGX_CONF_BLOCK_DONE;

            case '#':
                /* 若当前字符为 '#'，表示遇到注释了，因此回到循环开始
                 * 继续去下一个字符 */
                sharp_comment = 1;
                continue;

            case '\\':
                quoted = 1;
                last_space = 0;
                continue;

            case '"':
                start++;
                d_quoted = 1;
                last_space = 0;
                continue;

            case '\'':
                start++;
                s_quoted = 1;
                last_space = 0;
                continue;

            default:
                last_space = 0;
            }

        } else {
            if (ch == '{' && variable) {
                continue;
            }

            variable = 0;

            if (ch == '\\') {
                quoted = 1;
                continue;
            }

            if (ch == '$') {
                variable = 1;
                continue;
            }

            if (d_quoted) {
                if (ch == '"') {
                    d_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } else if (s_quoted) {
                if (ch == '\'') {
                    s_quoted = 0;
                    need_space = 1;
                    found = 1;
                }

            } else if (ch == ' ' || ch == '\t' || ch == CR || ch == LF
                       || ch == ';' || ch == '{')
            {
                last_space = 1;
                found = 1;
            }

            /* found 为 1 表明读取到完整的一个字符串，该指令的名称/参数 */
            if (found) {
                word = ngx_array_push(cf->args);
                if (word == NULL) {
                    return NGX_ERROR;
                }

                word->data = ngx_pnalloc(cf->pool, b->pos - 1 - start + 1);
                if (word->data == NULL) {
                    return NGX_ERROR;
                }

                for (dst = word->data, src = start, len = 0;
                     src < b->pos - 1;
                     len++)
                {
                    if (*src == '\\') {
                        switch (src[1]) {
                        case '"':
                        case '\'':
                        case '\\':
                            src++;
                            break;

                        case 't':
                            *dst++ = '\t';
                            src += 2;
                            continue;

                        case 'r':
                            *dst++ = '\r';
                            src += 2;
                            continue;

                        case 'n':
                            *dst++ = '\n';
                            src += 2;
                            continue;
                        }

                    }
                    *dst++ = *src++;
                }
                *dst = '\0';
                word->len = len;

                /* 若当前字符为 ';'，表明一个指令已经读取完毕 */
                if (ch == ';') {
                    return NGX_OK;
                }

                /* 若当前字符为 '{'，则表明将要开始解析复杂配置项 */
                if (ch == '{') {
                    return NGX_CONF_BLOCK_START;
                }

                found = 0;
            }
        }
    }
}

3.2 ngx_conf_handler

/*
 * Nginx的每一个配置指令都对应一个ngx_command_s数据类型变量，记录着该配置指令的解析回调函数、
 * 转换值存储位置等，而每一个模块又都把自身所相关的所有指令以数组的形式组织起来，所以函数
 * ngx_conf_handler()首先做的就是查找当前指令所对应的ngx_command_s变量，这通过循环遍历各个
 * 模块的指令数组即可。
 */
static ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    char           *rv;
    void           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    /* 此时 cf->elts 数组保存的是从配置文件中读取到的一行指令 */
    name = cf->args->elts;

    found = 0;

    /* 遍历所有模块 */
    for (i = 0; cf->cycle->modules[i]; i++) {

        /* 获取该模块保存的所有配置指令的数组 */
        cmd = cf->cycle->modules[i]->commands;
        if (cmd == NULL) {
            continue;
        }

        for ( /* void */ ; cmd->name.len; cmd++) {

            /* 若指令的名称长度不等，下一个 */
            if (name->len != cmd->name.len) {
                continue;
            }

            /* 若内容不相等，则继续下一个 */
            if (ngx_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            /* 若上面检测到指令的长度和名称都相等，则表示找到 */
            found = 1;

            if (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }

            /* is the directive's location right ? */

            /*  */
            if (!(cmd->type & cf->cmd_type)) {
                continue;
            }

            if (!(cmd->type & NGX_CONF_BLOCK) && last != NGX_OK) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                    "directive \"%s\" is not terminated by \";\"",
                     name->data);
                return NGX_ERROR;
            }

            if ((cmd->type & NGX_CONF_BLOCK) && last != NGX_CONF_BLOCK_START) 
            {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "directive \"%s\" has no opening \"{\"",
                                   name->data);
                return NGX_ERROR;
            }

            /* is the directive's argument count right ? */

            /* NGX_CONF_ANY：该配置指令可以出现在任意配置级别上 */
            if (!(cmd->type & NGX_CONF_ANY)) {

                /* 配置指令可以接受的是"on"或者"off"，最终会被转成bool值 */
                if (cmd->type & NGX_CONF_FLAG) {

                    if (cf->args->nelts != 2) {
                        goto invalid;
                    }
    
                /* 配置指令接受至少一个参数 */
                } else if (cmd->type & NGX_CONF_1MORE) {

                    if (cf->args->nelts < 2) {
                        goto invalid;
                    }

                /* 配置指令接受至少两个参数 */
                } else if (cmd->type & NGX_CONF_2MORE) {

                    if (cf->args->nelts < 3) {
                        goto invalid;
                    }

                } else if (cf->args->nelts > NGX_CONF_MAX_ARGS) {

                    goto invalid;

                } else if (!(cmd->type & argument_number[cf->args->nelts - 1]))
                {
                    goto invalid;
                }
            }

            /* set up the directive's configuration context */

            conf = NULL;

            /* 可以出现在配置文件的最外层。例如已经提供的配置指令daemon，
             * master_process等 */
            if (cmd->type & NGX_DIRECT_CONF) {
            
                /* 获取该模块对应的配置文件结构体，该配置结构体用于保存
                 * 该模块感兴趣的配置项值 */
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            /* 配置文件最外层，如http、mail、events、error_log 等 */
            } else if (cmd->type & NGX_MAIN_CONF) {
            
                /* 获取该模块的总配置信息结构体，对于http模块，当解析到http时，
                 * 该http核心模块ngx_http_module由于没有实现create_conf方法，
                 * 因此，当第一次解析到http{}时，这里获取到的conf的具体内容
                 * 为NULL，需要在解析http{}时才分配 */
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            /* 对应解析http{}里的内容时 */
            } else if (cf->ctx) {
                /* 获取该http配置项应存储在http的main、server、locaiton这三块内存池
                 * 中哪个内存池上 */
                confp = *(void **) ((char *) cf->ctx + cmd->conf);

                if (confp) {
                
                    /* 又根据该模块的ctx_index序号获取在该内存池(即指针数组)中
                     * 指向该类模块对应配置信息结构体的指针 */
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            /* 调用该配置指令对应的回调函数，将读取到到值设置到该配置结构体
             * conf 的相应字段 */
            rv = cmd->set(cf, cmd, conf);

            if (rv == NGX_CONF_OK) {
                return NGX_OK;
            }

            if (rv == NGX_CONF_ERROR) {
                return NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            return NGX_ERROR;
        }
    }

    if (found) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "\"%s\" directive is not allowed here", name->data);

        return NGX_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "unknown directive \"%s\"", name->data);

    return NGX_ERROR;

invalid:

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                       "invalid number of arguments in \"%s\" directive",
                       name->data);

    return NGX_ERROR;
}
```