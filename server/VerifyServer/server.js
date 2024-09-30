const grpc = require('@grpc/grpc-js') // 引入grpc库
const message_proto = require('./proto') // 引入protobuf定义文件
const const_module = require('./const') // 引入常量模块
const { v4:uuidv4 } = require('uuid'); // 引入uuid库，用于生成唯一ID
const emailModule = require('./email'); // 引入邮件发送模块
const redisModule = require('./redis'); // 引入redis模块

// 定义获取验证码的异步函数
async function GetVerifyCode(call, callback) {
    console.log("email is ", call.request.email) // 打印请求中的邮箱地址
    try{
        let query_res = await redisModule.GetRedisValue(const_module.code_prefix+call.request.email); // 从redis中查询邮箱地址对应的验证码
        let uniqueId;
        if(query_res == null){
            uniqueId = uuidv4();
            if(uniqueId.length > 4){
                uniqueId = uniqueId.substring(0,4);
            }
            let bres = await redisModule.SetRedisExpire(const_module.code_prefix+call.request.email, uniqueId,600); // 设置redis中验证码的过期时间为10分钟
            if(bres == null){
                callback(null, { email:  call.request.email, // 返回失败响应
                    error:const_module.Errors.RedisErr
                }); 
                return;
            }
        }else{
            uniqueId = query_res;
        }
        if(uniqueId.length > 4){
            uniqueId = uniqueId.substring(0,4);
        }
        console.log("uniqueId is ", uniqueId) // 打印生成的唯一ID
        
        let text_str =  '您的验证码为'+ uniqueId +'请三分钟内完成注册' // 生成邮件内容
        // 配置邮件发送选项
        let mailOptions = {
            from: '961693161@qq.com', // 发件人邮箱
            to: call.request.email, // 收件人邮箱
            subject: '验证码', // 邮件主题
            text: text_str, // 邮件内容
        };
        let send_res = await emailModule.SendMail(mailOptions); // 发送邮件
        console.log("send res is ", send_res) // 打印邮件发送结果
        callback(null, { email:  call.request.email, // 返回成功响应
            error:const_module.Errors.Success
        }); 
    }catch(error){
        console.log("catch error is ", error) // 打印错误信息
        callback(null, { email:  call.request.email, // 返回异常响应
            error:const_module.Errors.Exception
        }); 
    }
}

// 主函数，启动grpc服务器
function main() {
    var server = new grpc.Server() // 创建grpc服务器实例
    server.addService(message_proto.VerifyService.service, { GetVerifyCode: GetVerifyCode }) // 添加服务
    server.bindAsync('0.0.0.0:50051', grpc.ServerCredentials.createInsecure(), () => { // 绑定服务器地址和端口
        server.start() // 启动服务器
        console.log('grpc server started') // 打印服务器启动信息        
    })
}

main() // 调用主函数启动服务器
