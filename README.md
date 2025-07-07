# 32_STM32F103ZET6


## HAL库 

### 1 开发环境搭建

```
1. 安装 STM23CubeMX （需要 java8 环境支持）
2. 安装 HAL 库支持包（在线或离线安装）
```
![1751890846075.png](https://img.picui.cn/free/2025/07/07/686bbb98a550c.png)

### 2 使用 STM23CubeMX 创建 HAL 库项目

#### 2.0 创建项目并选择对应芯片

![1751891199113.png](https://img.picui.cn/free/2025/07/07/686bbcfb7e0c7.png)

![1751891366393.png](https://img.picui.cn/free/2025/07/07/686bbda16f8da.png)

#### 2.1 引脚和系统设置

![](http://image.dev.fuming.site/STM32/%E5%9F%BA%E7%A1%8006_HAL/RCC.png)

![](http://image.dev.fuming.site/STM32/%E5%9F%BA%E7%A1%8006_HAL/SYS.png)

![](http://image.dev.fuming.site/STM32/%E5%9F%BA%E7%A1%8006_HAL/Pin.png)

#### 2.2 时钟设置

![](http://image.dev.fuming.site/STM32/%E5%9F%BA%E7%A1%8006_HAL/Clock_Tree.png)

#### 2.3 项目管理配置

![](http://image.dev.fuming.site/STM32/%E5%9F%BA%E7%A1%8006_HAL/%E5%B7%A5%E7%A8%8B%E9%85%8D%E7%BD%AE01.png)

![](http://image.dev.fuming.site/STM32/%E5%9F%BA%E7%A1%8006_HAL/%E5%B7%A5%E7%A8%8B%E9%85%8D%E7%BD%AE02.png)

![](http://image.dev.fuming.site/STM32/%E5%9F%BA%E7%A1%8006_HAL/%E5%B7%A5%E7%A8%8B%E9%85%8D%E7%BD%AE03.png)

### 完成
```
点击GENRERATE CODE完成创建
```
