const config_module = require('./config')
const Redis = require("ioredis");
// 解释以下代码
// const config_module = require('./config') 引入config.js文件，该文件中包含redis服务器的配置信息
// const Redis = require("ioredis"); 引入ioredis模块，该模块是Redis的Node.js客户端
const RedisCli = new Redis({
    host: config_module.redis_host,       // Redis服务器主机名
    port: config_module.redis_port,        // Redis服务器端口号
    password: config_module.redis_passwd, // Redis密码
});


RedisCli.on("error", function (err) {
    console.log("Redis Error:", err);
    RedisCli.quit();
});

async function GetRedisValue(key) {
    try {
        const value = await RedisCli.get(key);
        if(value == null){
            console.log("Redis中没有该key的值");
            return null;
        }
        console.log("Redis中key的值为:", value);
        return value;
    } catch (error) {
        console.log("Redis Error:", error);
        RedisCli.quit();
        return null;
    }
}

async function RedisKeysisExist(key) {
    try {
        const res = await RedisCli.exists(key);
        if(res == null){
            console.log("Redis中没有该key的值");
            return null;
        }
        console.log("Redis中key的值存在");
        return true;
    } catch (error) {
        console.log("Redis Error:", error);
        RedisCli.quit();
        return null;
    }
}

async function SetRedisExpire(key, value,expireTime) {
    try {
        const res = await RedisCli.set(key, value, 'EX', expireTime);
        if(res === null){
            console.log("设置Redis中key的值失败");
            return null;
        }
        console.log("设置Redis中key的值成功:", res);
        return true;
    } catch (error) {
        console.log("SetRedisExpire Error:", error);
        RedisCli.quit();
        return null;
    }
}

function quitRedis() {
    RedisCli.quit();
}


module.exports = {
    GetRedisValue,
    SetRedisExpire,
    RedisKeysisExist,
    quitRedis
}



