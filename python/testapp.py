#!/usr/bin/env python3
"""
Serves an NLLB MT model using Flask HTTP server
"""
import logging
import os
import sys
import platform
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter
import logging as log
from functools import lru_cache
import time

import flask
from flask import Flask, request, send_from_directory, Blueprint
import transformers
from transformers import AutoModelForSeq2SeqLM, AutoTokenizer


import resource
from typing import Tuple
import sys

log.basicConfig(level=log.INFO)
DEF_MODEL_ID = "facebook/nllb-200-distilled-600M"
DEF_SRC_LNG = 'eng_Latn'
DEF_TGT_LNG = 'kan_Knda'
FLOAT_POINTS = 4
exp = None
app = Flask(__name__)
app.config['JSON_AS_ASCII'] = False

if __name__ == "__main__":

    model_id=DEF_MODEL_ID
    def_src_lang=DEF_SRC_LNG
    # def_tgt_lang=DEF_TGT_LNG, **kwargs):
    # sys_info['model_id'] = model_id

    log.info(f"Loading model {model_id} ...")
    model = AutoModelForSeq2SeqLM.from_pretrained(model_id)
    log.info(f"Loading default tokenizer for {model_id} ...")
    # tokenizer = AutoTokenizer.from_pretrained(model_id)
    # src_langs = tokenizer.additional_special_tokens
    # tgt_langs = src_langs

    def get_tokenizer(src_lang=def_src_lang):
        log.info(f"Loading tokenizer for {model_id}; src_lang={src_lang} ...")
        #tokenizer = AutoTokenizer.from_pretrained(model_id)
        return AutoTokenizer.from_pretrained(model_id, src_lang=src_lang)
    
    src_lang = "eng_Latn"
    tgt_lang="zho_Hans"
    sources = ['No Language Left Behind, no language']
    st = time.time()
    tokenizer = get_tokenizer(src_lang=src_lang)

    # if not sources:
    #    return "Please submit 'source' parameter", 400
    max_length = 80
    inputs = tokenizer(sources, return_tensors="pt", padding=True,)
    print("inputs:",inputs)
    translated_tokens = model.generate(
        **inputs, forced_bos_token_id=tokenizer.lang_code_to_id[tgt_lang],
        max_length = max_length)
    output = tokenizer.batch_decode(translated_tokens, skip_special_tokens=True)

    res = dict(source=sources, translation=output,
               src_lang = src_lang, tgt_lang=tgt_lang,
               time_taken = round(time.time() - st, 3), time_units='s')
    print("res:", res)