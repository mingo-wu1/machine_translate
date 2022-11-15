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
文字
```
Pages.py
31行
        for page in self:
            if page.skip_parsing: continue

            # init and extract data from PDF
            raw_page = RawPageFactory.create(page_engine=fitz_doc[page.id], backend='PyMuPDF')

            print(help(raw_page)) # 这里输出文字

            raw_page.restore(**settings)
            
这里是raw_oage
class RawPageFitz(pdf2docx.page.RawPage.RawPage)
 |  RawPageFitz(page_engine=None)
 |  
 |  A wrapper of ``fitz.Page`` to extract source contents.
 |  
 |  Method resolution order:
 |      RawPageFitz
 |      pdf2docx.page.RawPage.RawPage
 |      pdf2docx.page.BasePage.BasePage
 |      pdf2docx.layout.Layout.Layout
 |      builtins.object
 |  
 |  Methods defined here:
 |  
 |  extract_raw_dict(self, **settings)
 |      Extract source data with page engine. Return a dict with the following structure:
 |      ```
 |          {
 |              "width" : w,
 |              "height": h,    
 |              "blocks": [{...}, {...}, ...],
 |              "shapes" : [{...}, {...}, ...]
 |          }
 |      ```
 |  
 |  ----------------------------------------------------------------------
 |  Methods inherited from pdf2docx.page.RawPage.RawPage:
 |  
 |  __init__(self, page_engine=None)
 |      Initialize page layout.
 |      
 |      Args:
 |          page_engine (Object): Source pdf page.
 |  
 |  calculate_margin(self, **settings)
 |      Calculate page margin.
 |      
 |      .. note::
 |          Ensure this method is run right after cleaning up the layout, so the page margin is 
 |          calculated based on valid layout, and stay constant.
 |  
 |  clean_up = inner(*args, **kwargs)
 |  
 |  parse_section(self, **settings)
 |      Detect and create page sections.
 |      
 |      .. note::
 |          - Only two-columns Sections are considered for now.
 |          - Page margin must be parsed before this step.
 |  
 |  process_font(self, fonts: pdf2docx.font.Fonts.Fonts)
 |      Update font properties, e.g. font name, font line height ratio, of ``TextSpan``.
 |      
 |      Args:
 |          fonts (Fonts): Fonts parsed by ``fonttools``.
 |  
 |  restore = inner(*args, **kwargs)
 |  
 |  ----------------------------------------------------------------------
 |  Readonly properties inherited from pdf2docx.page.RawPage.RawPage:
 |  
 |  raw_text
 |      Extracted raw text in current page. Should be run after ``restore()`` data.
 |  
 |  text
 |      All extracted text in this page, with images considered as ``<image>``. 
 |      Should be run after ``restore()`` data.
 |  
 |  ----------------------------------------------------------------------
 |  Readonly properties inherited from pdf2docx.page.BasePage.BasePage:
 |  
 |  bbox
 |  
 |  working_bbox
 |      bbox with margin considered.
 |  
 |  ----------------------------------------------------------------------
 |  Data descriptors inherited from pdf2docx.page.BasePage.BasePage:
 |  
 |  __dict__
 |      dictionary for instance variables (if defined)
 |  
 |  __weakref__
 |      list of weak references to the object (if defined)
 |  
 |  ----------------------------------------------------------------------
 |  Methods inherited from pdf2docx.layout.Layout.Layout:
 |  
 |  assign_blocks(self, blocks: list)
 |      Add blocks (line or table block) to this layout. 
 |      
 |      Args:
 |          blocks (list): a list of text line or table block to add.
 |      
 |      .. note::
 |          If a text line is partly contained, it must deep into span -> char.
 |  
 |  assign_shapes(self, shapes: list)
 |      Add shapes to this cell. 
 |      
 |      Args:
 |          shapes (list): a list of Shape instance to add.
 |  
 |  contains(self, *args, **kwargs)
 |      Whether given element is contained in this layout.
 |  
 |  parse(self, **settings)
 |      Parse layout.
 |      
 |      Args:
 |          settings (dict): Layout parsing parameters.
 |  
 |  store(self)
 |      Store parsed layout in dict format.
(END)





这里是raw_page
lass RawPageFitz(RawPage):
    '''A wrapper of ``fitz.Page`` to extract source contents.'''

    def extract_raw_dict(self, **settings):
        raw_dict = {}
        if not self.page_engine: return raw_dict

        # actual page size
        *_, w, h = self.page_engine.rect # always reflecting page rotation
        raw_dict.update({ 'width' : w, 'height': h })
        self.width, self.height = w, h

        # pre-processing layout elements. e.g. text, images and shapes
        text_blocks = self._preprocess_text(**settings)
        raw_dict['blocks'] = text_blocks

        image_blocks = self._preprocess_images(**settings)
        raw_dict['blocks'].extend(image_blocks)
        
        shapes, images =  self._preprocess_shapes(**settings)
        raw_dict['shapes'] = shapes
        raw_dict['blocks'].extend(images)

        hyperlinks = self._preprocess_hyperlinks()
        raw_dict['shapes'].extend(hyperlinks)        
       
        # Element is a base class processing coordinates, so set rotation matrix globally
        Element.set_rotation_matrix(self.page_engine.rotation_matrix)

        return raw_dict
    

    def _preprocess_text(self, **settings):
        '''Extract page text and identify hidden text. 
        
        NOTE: All the coordinates are relative to un-rotated page.

            https://pymupdf.readthedocs.io/en/latest/page.html#modifying-pages
            https://pymupdf.readthedocs.io/en/latest/functions.html#Page.get_texttrace
            https://pymupdf.readthedocs.io/en/latest/textpage.html
        '''
        ocr = settings['ocr']
        if ocr==1: raise SystemExit("OCR feature is planned but not implemented yet.")

        # all text blocks no matter hidden or not
        raw = self.page_engine.get_text('rawdict', flags=64)
        text_blocks = raw.get('blocks', [])

        # potential UnicodeDecodeError issue when trying to filter hidden text:
        # https://github.com/dothinking/pdf2docx/issues/144
        # https://github.com/dothinking/pdf2docx/issues/155
        try:
            spans = self.page_engine.get_texttrace()
        except SystemError:
            logging.warning('Ignore hidden text checking due to UnicodeDecodeError in upstream library.')
            spans = []
        
        if not spans: return text_blocks

        # ignore hidden text if ocr=0, while extract only hidden text if ocr=2
        if ocr==2:
            f = lambda span: span['type']!=3  # find displayed text and ignore it
        else:
            f = lambda span: span['type']==3  # find hidden text and ignore it
        filtered_spans = list(filter(f, spans))
        
        def span_area(bbox):
            x0, y0, x1, y1 = bbox
            return (x1-x0) * (y1-y0)

        # filter blocks by checking span intersection: mark the entire block if 
        # any span is matched
        blocks = []
        for block in text_blocks:
            intersected = False
            for line in block['lines']:
                for span in line['spans']:
                    for filter_span in filtered_spans:
                        intersected_area = get_area(span['bbox'], filter_span['bbox'])
                        if intersected_area / span_area(span['bbox']) >= FACTOR_A_HALF \
                            and span['font']==filter_span['font']:
                            intersected = True
                            break
                    if intersected: break # skip further span check if found
                if intersected: break     # skip further line check

            # keep block if no any intersection with filtered span
            if not intersected: blocks.append(block)

        return blocks


    def _preprocess_images(self, **settings):
        '''Extract image blocks. Image block extracted by ``page.get_text('rawdict')`` doesn't 
        contain alpha channel data, so it has to get page images by ``page.get_images()`` and 
        then recover them. Note that ``Page.get_images()`` contains each image only once, i.e., 
        ignore duplicated occurrences.
        '''
        # ignore image if ocr-ed pdf: get ocr-ed text only
        if settings['ocr']==2: return []
        
        return ImagesExtractor(self.page_engine).extract_images(settings['clip_image_res_ratio'])


    def _preprocess_shapes(self, **settings):
        '''Identify iso-oriented paths and convert vector graphic paths to pixmap.'''
        paths = self._init_paths(**settings)
        return paths.to_shapes_and_images(
            settings['min_svg_gap_dx'], 
            settings['min_svg_gap_dy'], 
            settings['min_svg_w'], 
            settings['min_svg_h'], 
            settings['clip_image_res_ratio'])
    

    @debug_plot('Source Paths')
    def _init_paths(self, **settings):
        '''Initialize Paths based on drawings extracted with PyMuPDF.'''
        raw_paths = self.page_engine.get_cdrawings()
        return Paths(parent=self).restore(raw_paths)
    

    def _preprocess_hyperlinks(self):
        """Get source hyperlink dicts.

        Returns:
            list: A list of source hyperlink dict.
        """
        hyperlinks = []
        for link in self.page_engine.get_links():
            if link['kind']!=2: continue # consider internet address only
            hyperlinks.append({
                'type': RectType.HYPERLINK.value,
                'bbox': tuple(link['from']),
                'uri' : link['uri']
            })

        return hyperlinks                
```
table去边框
```
Layout.py
    def parse(self, **settings):
        '''Parse layout.

        Args:
            settings (dict): Layout parsing parameters.
        '''
        if not self.blocks: return

        # parse tables
        # self._parse_table(**settings)

```
