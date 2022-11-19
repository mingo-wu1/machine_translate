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
https://blog.51cto.com/u_15284125/5191796
https://blog.csdn.net/ddazz0621/article/details/120988507
https://blog.csdn.net/hz5034/article/details/77367871
https://www.cnblogs.com/zengkefu/p/5814780.html
https://blog.csdn.net/initphp/category_9265172.html
```
# websocket 分析
```
https://www.zhihu.com/question/20215561/answer/40316953
```
