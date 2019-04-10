---
typora-root-url: readme_img
---

# Duilib_Demo
建立一个demo，记录自己学习Duilib的过程

## 从零创建一个Duilib工程

**步骤：**

1. 导入Duilib库；
2. exe工程属性中，添加以下3个属性：
   1. “vc++目录”的“包含目录”-->“..\Duilib"
   2. “vc++目录”的“库目录“--> "..\Lib";
   3. “c++”--> "常规” ---> "附加包含目录“---> "..\Bin”
3. 主程序的cpp中，添加主窗口代码。
4. Bin文件夹中的dll，需要拷贝到主程序exe的所在目录下。

**截图**

![](../readme_img/first_demo.png)



# 创建另一个主窗口

使用xml文件，创建主窗口。

代码中，使用CDialogBuilder类创建。

**截图**

![](../readme_img/second_demo.png)







