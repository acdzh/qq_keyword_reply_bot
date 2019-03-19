#QQKeyWordreplyBot
------
基于官方[sdkV9](https://d.cqp.me/%E9%A6%96%E9%A1%B5)直接修改，若需要二次发布请自行修改包名！



生成后请将xxxx.dll和xxxx.json拷贝至酷Q文件夹下的app目录（需提前开启开发者权限），重启酷Q



若加载应用时提示-103错误，请做以下修改：


VS菜单"项目"-属性-配置属性-C/C++-代码生成-运行库，把/MD改成/MT





##可实现以下功能：

|命令|功能|
|-----|-----|
|!list key|列出关键词 key 对应的所有回复|
|!add key value|学习回复: key-value|
|!edit key old_value new_value|收到 key 不再回复 old_value，而是回复 new_value|
|!del key value|删除这一个回复_orz|
|!delkey key|删除关键字 key 的所有回复_orz|
|!help|查看帮助|
|!restart|重新加载|
|!backup|备份当前词库|



(仅能识别英文符号, 符号"~"与"!"等价.)


