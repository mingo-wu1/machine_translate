import os
import fitz

from concurrent import futures
import logging

import grpc
import helloworld_pb2
import helloworld_pb2_grpc


class Greeter(helloworld_pb2_grpc.GreeterServicer):

    def SayHello(self, request, context):
        c = open(file="/media/bzl/DATA3/BZLproject/00_AGV/forcompare/mywebserver/example/ml.pdf", mode="rb+")
        # file_context = request.name
        doc = fitz.open(stream=c) # 打开一个pdf文件
        rotate = int(0) # 设置图片的旋转角度        
        trans = fitz.Matrix(2.0, 2.0).prerotate(theta=0)
        # file_context = 
        # chuli
        return helloworld_pb2.HelloReply(message='Hello, %s!' % request.name)


def serve():
    port = '50051'
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    helloworld_pb2_grpc.add_GreeterServicer_to_server(Greeter(), server)
    server.add_insecure_port('[::]:' + port)
    server.start()
    print("Server started, listening on " + port)
    server.wait_for_termination()


if __name__ == '__main__':
    c = open(file="/media/bzl/DATA3/BZLproject/00_AGV/forcompare/mywebserver/example/ml.pdf", mode="rb+")
    # file_context = request.name
    print(type(c))
    name = "test.pdf"
    doc = fitz.open("/media/bzl/DATA3/BZLproject/00_AGV/forcompare/mywebserver/example/ml.pdf")
    trans = fitz.Matrix(2.0, 2.0).prerotate(theta=0)    
    for index in range(doc.page_count):
        page = doc[index]
        pm = page.get_pixmap(matrix=trans, dpi=None, colorspace=fitz.csRGB, clip=None, alpha=False, annots=True)
        new_full_name = name.split(".")[0]
        if doc.page_count > 1 :
            pm.save(filename="%s%s.pdf" % (new_full_name, index), output="png")
        else:
            pm.save(filename="%s.pdf" % (new_full_name), output="png")
    
    print("pdf convert done")
