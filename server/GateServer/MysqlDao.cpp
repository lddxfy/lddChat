#include "MysqlDao.h"
#include "ConfigMgr.h"
MysqlDao::MysqlDao()
{
	auto &cfg = ConfigMgr::GetInstance();
    const auto& host = cfg["Mysql"]["Host"];
    const auto& port = cfg["Mysql"]["Port"];
    const auto& pwd = cfg["Mysql"]["Passwd"];
    const auto& schema = cfg["Mysql"]["Schema"];
    const auto& user = cfg["Mysql"]["User"];
    pool_.reset(new MysqlConPool(5,host,port,user,pwd,schema));
}

MysqlDao::~MysqlDao()
{
    pool_->Close();
}

int MysqlDao::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
    auto con = pool_->GetConnection();
    try {
        if (con == nullptr) {
            return false;
        }
        // 准备调用存储过程
        std::unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("CALL RegisterUser(?,?,?,@result)"));
        // 设置输入参数
        stmt->setString(1, name);
        stmt->setString(2, email);
        stmt->setString(3, pwd);
        
        // 由于PreparedStatement不直接支持注册输出参数，我们需要使用会话变量或其他方法来获取输出参数的值
          // 执行存储过程
        stmt->execute();
        // 如果存储过程设置了会话变量或有其他方式获取输出参数的值，你可以在这里执行SELECT查询来获取它们
       // 例如，如果存储过程设置了一个会话变量@result来存储输出结果，可以这样获取：
        std::unique_ptr<sql::Statement> stmtResult(con->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmtResult->executeQuery("SELECT @result AS result"));
        if (res->next()) {
            int result = res->getInt("result");
            std::cout << "Result: " << result << std::endl;
            pool_->returnConnection(std::move(con));
            return result;
        }
        pool_->returnConnection(std::move(con));
        return -1;
    }
    catch (sql::SQLException& e) {
        pool_->returnConnection(std::move(con));
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return -1;
    }
	
}

int MysqlDao::CheckEmail(const std::string& email)
{
    auto con = pool_->GetConnection();
    if (con == nullptr)  return false;
    try
    {
        // 准备查询语句
        std::unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("Select id from users where email = ?"));
        //指定参数
        stmt->setString(1, email);
        //获取结果集
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        //遍历结果集
        if(res->next()) {
            std::cout << "this Email user id is: " << res->getString("id") << std::endl;
            pool_->returnConnection(std::move(con));
            return true;
        }
        //没有找到该邮箱
        else {
            std::cout << "this Email is not exist" << std::endl;
            pool_->returnConnection(std::move(con));
            return false;
        }
    }
    catch (sql::SQLException& e)
    {
        pool_->returnConnection(std::move(con));
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return 0;
}

int MysqlDao::ResetPwd(const std::string& name, const std::string& pwd)
{
    auto con = pool_->GetConnection();
    if (con == nullptr)  return false;
    try
    {
        std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement("Update users set pwd = ? where name = ?"));

        stmt->setString(1, pwd);
        stmt->setString(2, name);

        int updateCount = stmt->executeUpdate();
        std::cout << "Updated rows: " << updateCount << std::endl;
        pool_->returnConnection(std::move(con));
        return true;

    }
    catch (sql::SQLException& e)
    {
        pool_->returnConnection(std::move(con));
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return 0;
}

int MysqlDao::CheckLogin(const std::string& email,const std::string& pwd,Userinfo& userinfo)
{
    auto con = pool_->GetConnection();
    if (con == nullptr)  return false;
    try
    {
        // 准备查询语句
        std::unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("Select * from users where email = ?"));
        //指定参数
        stmt->setString(1, email);
        //获取结果集
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        //没有找到该邮箱
        
        //遍历结果集
        if(res->next()) {
            auto userid = res->getInt("id");
            auto username = res->getString("name");
            auto useremail = res->getString("email");
            auto userpwd = res->getString("pwd");
            if (userpwd != pwd) {
                std::cout << "pwd is not match" << std::endl;
                pool_->returnConnection(std::move(con));
                return false;
            }
            userinfo.id = userid;
            userinfo.name = username;
            userinfo.pwd = userpwd;
            userinfo.email = useremail;

            std::cout << "found user! "<< std::endl;
            pool_->returnConnection(std::move(con));
            return true;
        }else{
            std::cout << "this Email is not exist" << std::endl;
            pool_->returnConnection(std::move(con));
            return false;
        }
        
        
    }
    catch (sql::SQLException& e)
    {
        pool_->returnConnection(std::move(con));
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return 0;
}

int MysqlDao::Query()
{
    auto con = pool_->GetConnection();
    try
    {
        if (con == nullptr) {
            return -1;
        }

        // 选择数据库  
        con->setSchema("lddchat");

        // 执行查询  
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM users LIMIT 1"));

        if (res->next()) {
            std::cout << "Query Result: " << res->getString("name") << std::endl;
            pool_->returnConnection(std::move(con));
            return 1;
        }
        else {
            std::cout << "No rows returned." << std::endl;
            pool_->returnConnection(std::move(con));
            return 0;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
    return 0;
}

int MysqlDao::insert()
{
    auto con = pool_->GetConnection();
    // 插入数据  
    std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO users (name,email,pwd) VALUES (?,?,?)"));
    pstmt->setString(1, "cjqtest");
    pstmt->setString(2, "12345@qq.com");
    pstmt->setString(3, "123456");
    pstmt->executeUpdate();

    std::cout << "Row inserted successfully." << std::endl;
    pool_->returnConnection(std::move(con));
    return 1;
}
