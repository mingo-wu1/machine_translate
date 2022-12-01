#### HTTP

```
////////////  nginx/src/event/ngx_event_accept.c  /////////////
//接受新的连接
void  ngx_event_accept(ngx_event_t *ev)
{
     ......    
     //如果剩余的空闲连接数小于连接池总数的1/8，下次执行ngx_process_events_and_timers时
     //就不再竞争accept锁
     ngx_accept_disabled = ngx_cycle->connection_n / 8
                              - ngx_cycle->free_connection_n;
     ......
     //从连接池中取出一个连接并进行初始化
     c = ngx_get_connection(s, ev->log);
     ......
    //将新建的连接通过ngx_add_conn函数指针加入到IO复用函数的等待队列中
    if (ngx_add_conn && (ngx_event_flags & NGX_USE_EPOLL_EVENT) == 0) {
          if (ngx_add_conn(c) == NGX_ERROR) {
                ngx_close_accepted_connection(c);
                return;
          }
    }
  
    log->data = NULL;
    log->handler = NULL;
    //ls对应的handler是在ngx_http_add_listening函数中将ngx_http_init_connection
    //函数赋值给了handler
    ls->handler(c);
    ......
}
```


```
////////////  nginx/src/event/modules/ngx_epoll_module.c  /////////////
//epoll模块事件处理函数
static ngx_int_t  ngx_epoll_process_events(ngx_cycle_t *cycle, ngx_msec_t timer, ngx_uint_t flags)
{
     ......
     //等待新连接（最多阻塞timer时长）
     events = epoll_wait(ep, event_list, (int) nevents, timer);
      
     ......
     //如果设置了NGX_POST_EVENTS就说明当前持有accept锁，应当将新事件放到队列中，尽快返回
     if (flags & NGX_POST_EVENTS) {
         queue = rev->accept ? &ngx_posted_accept_events
                                    : &ngx_posted_events;
         ngx_post_event(rev, queue);
     } else {
                //没有持有accept锁，可以同步执行事件处理函数
                //rev->handler在ngx_http_init_connection函数中被赋值为ngx_http_wait_request_handler
                rev->handler(rev);
     }
     .......
}
```
