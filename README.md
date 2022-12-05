# machine_translate
machine translate

# 编译工程
```
mkdir build && cd build && cmake ..
```

# 编译grpc
```
git submodule update --init
mkdir build && cd build && cmake ..
make package
bash c-ares-1.17.2-Linux.sh # all yes
cp -r c-ares-1.17.2-Linux/c-ares-1.17.2-Linux <your path>
# CMakeLists.txt中添加 list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_part/c-ares-1.17.2-Linux) # 安装路径前缀
python3 -m pip grpcio-tools
```

# init uni-app
```
node -v # 14.20.0
sudo npm install -g @vue/cli@4
sudo npm install @vue/cli-service -g
sudo npm install @dcloudio/vue-cli-plugin-hbuilderx -g
sudo npm install @dcloudio/uni-cli-i18n -g
sudo npm install @dcloudio/uni-cli-shared -g
sudo npm install module-alias -g
sudo npm install @dcloudio/vue-cli-plugin-uni -g
sudo npm install @dcloudio/webpack-uni-pages-loader -g
sudo npm install @dcloudio/uni-h5 -g
sudo npm install node-sass@4.14 -g # https://www.npmjs.com/package/node-sass

vue create -p dcloudio/uni-preset-vue mt
cd mt
npm run serve
```
# 构建样例1
```
git clone git@github.com:kingloomy/en-translate.git
npm install # 安装package里面的依赖包
npm run dev # Compile and Hot-Reload for Development
npm run build # Type-Check, Compile and Minify for Production
npm run lint # Lint with ESLint
```
# 构建样例2
```
git clone git@github.com:RSurya99/vue-auto-translate.git
npm install # 安装package里面的依赖包
npm run serve # Compiles and hot-reloads for development
npm run build # Type-Check, Compile and Minify for Production , How to deploy
npm run lint # Lint with ESLint
```
# github 搜索例子
```
translate app vue
```
# routemap
```
公道-人的时间-短视频完播率
悬疑-浅入深到结果,连续剧最后是结果,上来给答案话题就废了,不断制作话题,引起疑问,把人性拉爆,把悬疑拉爆
```

# 引入c
```
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavformat/avio.h"
#include "libswscale/version.h"
}
```
# nginx 分析
```
框架流程
https://blog.51cto.com/u_15284125/5191796
https://blog.csdn.net/ddazz0621/article/details/120988507
https://blog.csdn.net/hz5034/article/details/77367871
https://www.cnblogs.com/zengkefu/p/5814780.html
https://blog.csdn.net/initphp/category_9265172.html
https://baijiahao.baidu.com/s?id=1702854349675416021&wfr=spider&for=pc
https://zhuanlan.zhihu.com/p/552580039
https://blog.csdn.net/weixin_52967653/article/details/126562936
https://blog.csdn.net/haogenmin/article/details/118527213
https://www.cnblogs.com/xuewangkai/p/11158576.html
https://view.inews.qq.com/a/20220329A09C7900
http://t.zoukankan.com/ExMan-p-10555444.html
https://cxybb.com/article/qq_27788177/116008082
架构
https://www.cnblogs.com/applelife/p/10447284.html
结构体
https://www.w3cschool.cn/nginx/ilmq1pe5.html
https://blog.csdn.net/locahuang/article/details/115718415
https://www.furyblog.com/?p=565
https://blog.csdn.net/fzy0201/article/details/19281503
连接池
http://blog.chinaunix.net/uid-29006187-id-4636563.html
https://blog.csdn.net/xixihahalelehehe/article/details/123075592
https://blog.csdn.net/Edidaughter/article/details/109564411
https://juejin.cn/post/7028586736301113381
https://baijiahao.baidu.com/s?id=1682253381648787898&wfr=spider&for=pc
事件驱动epoll
https://blog.51cto.com/yyxianren/5721144
https://www.modb.pro/db/444904
https://zhuanlan.zhihu.com/p/495188004
https://www.21cto.com/article/2556
http
https://blog.csdn.net/fdsafwagdagadg6576/article/details/121019371
https://zhuanlan.zhihu.com/p/384217325
进程通信，进程状态，信号，守护进程，进程模型
...
信号处理，子进程实战，文件IO，读配置文件、设置标题、日志打印，目录规划，makefile
...
网络模型，TCP三次握手详析，TCP状态转换、telnet，wireshark，listen()队列剖析、阻塞非阻塞、同步异步，IO复用，监听端口实战、epoll介绍及原理详析、多线程、线程池、心跳包、并发、收发数据、服务器安全与完善、超负荷安全处理、综合压力、惊群、性能优化、事件驱动
...
```

# websocket 分析
```
https://www.zhihu.com/question/20215561/answer/40316953
```
