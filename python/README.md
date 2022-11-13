# 训练过程参考
```
https://blog.csdn.net/javastart/article/details/122756853
```

# 获取训练模型
```
mkdir nllb-200-distilled-600M 
cd nllb-200-distilled-600M
下载: https://huggingface.co/facebook/nllb-200-distilled-600M/tree/main
代码修改: DEF_MODEL_ID = "./nllb-200-distilled-600M" # new path

```

# 功能(翻译官)
```
一款多功能智能翻译手机软件，它有多个翻译功能可以使用，包括文本翻译、语音翻译、同声传译、拍照翻译、文档翻译，日常各种翻译需求几乎都可以满足。
进入软件首页就可以输入文本进行翻译，翻译后的文字支持播放译文，还能复制文字，翻译速度快而且还挺准确的。
翻译之后还会自动保存翻译记录，方便多次查看。
支持翻译的语言有二十多种，包括中文、英语、日语、韩语、法语、俄语等等，几乎常见的都包括了。
它也有文档翻译功能可以使用，支持PDF、Word、Excel、PPT和TXT文档的翻译，翻译完成之后重新保存到手机上就可以查看了，翻译的效果也是挺好的。
还有语音翻译的功能，按住话筒按钮就能录制语音并进行翻译，翻译以对话的形式展示，有了这个功能，就能与外国人实现无障碍沟通。
好了，以上就是好用的网页翻译插件和两个好用的翻译工具，大家可以根据自己平时的翻译需求去选择使用。最后，觉得好用的话，记得点赞呀！
```

# 跑通程序，测试预训练模型(在use_translate.py文件中写入：)
```
from transformers import AutoModelWithLMHead, AutoTokenizer, pipeline

mode_name = "trans_model"
model = AutoModelWithLMHead.from_pretrained(mode_name)
tokenizer = AutoTokenizer.from_pretrained(mode_name)
translation = pipeline("translation_zh_to_en", model=model, tokenizer=tokenizer)
translate_result = translation('自然语言处理的技术之一：机器翻译', max_length=400)
print(translate_result)
# [{'translation_text': 'One of the technologies for natural language processing: machine translation'}]

如果控制台输出：

[{'translation_text': 'One of the technologies for natural language processing: machine translation'}]

则说明模型是通的，都没有问题
拆解Pipeline，逐步进行翻译任务

from transformers import AutoModelWithLMHead, AutoTokenizer

# 加载预训练模型
mode_name = "trans_model"
model = AutoModelWithLMHead.from_pretrained(mode_name)
tokenizer = AutoTokenizer.from_pretrained(mode_name)

# 开始翻译
text = "自然语言处理的技术之一：机器翻译"

# 步骤1：将文本变为token，返回pytorch的tensor
tokenized_text = tokenizer.prepare_seq2seq_batch([text], return_tensors='pt')
# 也可以使用：
# tokenized_text = tokenizer([text], return_tensors="pt")

# 步骤2：通过模型，得到预测出的token
translation = model.generate(**tokenized_text)  # 执行翻译，返回翻译后的tensor

# 步骤3：将预测出的token转为单词
translated_text = tokenizer.batch_decode(translation, skip_special_tokens=True)
print(translated_text)

输出：

['One of the technologies for natural language processing: machine translation']

因此我们可以发现，整体的流程是:
    将原始中文文本变为token
    通过模型，得到预测出的token（对应英文的token）
    将预测出的token转为英文单词
```

# 两个优秀的语音识别项目(做语音识别实时翻译用的)
```
https://github.com/Z-yq/TensorflowASR
https://github.com/yeyupiaoling/PPASR
```

# pdf转docx库比较
```
pdf2docx
PyMuPDF
pdfplumber
pdfminer
camelot
PyPDF2	读
PyMuPDF	读
pdflib	读
PDF表格	读
PDFMiner.six	读
PDF查询	读
pdfrw	读，写/创作
PyFPDF	写/创作
python-office
pdfminer3k
一个是pdfminer3k，一个是python-docx，其中pdfminer3k用来解析pdf提取出文本内容，python-docx用来将解析出的文本内容写入word文档
```

论文
```
https://mp.weixin.qq.com/s?__biz=Mzg2OTEzODA5MA==&mid=2247590131&idx=1&sn=80cc7ea6da5dedf77a69d5d66eae356a&chksm=cea28b36f9d50220c0f97361f63afb041052ce7e1bc5eecf5de7fe0d54e31d3a89b5e74bfc8f&token=1635955199&lang=zh_CN#rd
```
