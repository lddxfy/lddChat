//引入 Node.js 的 path 模块，用于处理文件路径。
const path = require('path')
//引入 @grpc/grpc-js 库，这是 gRPC 的 Node.js 客户端库。
const grpc = require('@grpc/grpc-js')
//引入 @grpc/proto-loader 库，这是一个用于将 .proto 文件加载为 JavaScript 对象的库
const protoLoader = require('@grpc/proto-loader')
//使用 path.join 方法构建 message.proto 文件的完整路径。
const PROTO_PATH = path.join(__dirname, 'message.proto')
//使用 protoLoader.loadSync 方法同步加载 .proto 文件。这个方法接受一个路径和一个选项对象。
//选项对象指定了加载 .proto 文件时的行为：
//keepCase: 保持字段名称的大小写。
//longs: 将长整数（64位）作为字符串处理。
//enums: 将枚举值作为字符串处理。
//defaults: 为未设置的字段填充默认值。
//oneofs: 支持 protobuf 的 oneof 语法。
const packageDefinition = protoLoader.loadSync(PROTO_PATH, { keepCase: true, longs: String, enums: String, defaults: true, oneofs: true })
//加载的 .proto 文件定义转换为 gRPC 可用的格式。
const protoDescriptor = grpc.loadPackageDefinition(packageDefinition)
//从加载的包定义中获取 message 命名空间，这通常对应于 .proto 文件中定义的消息和服务
const message_proto = protoDescriptor.message
//将 message_proto 对象导出，这样其他模块就可以通过 require 导入它
module.exports = message_proto