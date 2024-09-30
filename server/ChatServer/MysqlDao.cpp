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

int MysqlDao::QueryUserFromId(int uid, std::shared_ptr<Userinfo>& userinfo)
{
    auto con = pool_->GetConnection();
    try
    {
        if (con == nullptr) {
            return -1;
        }

        // 选择数据库  
        con->setSchema("lddchat");

        // 准备查询语句
        std::unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("Select * from users where id = ?"));
        //指定参数
        stmt->setInt(1,uid);
        //获取结果集
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());



        if (res->next()) {
            auto userid = res->getInt("id");
            auto username = res->getString("name");
            auto useremail = res->getString("email");
            auto userpwd = res->getString("pwd");
            auto usernick = res->getString("nick");
            auto usersex = res->getInt("sex");
            auto userhead = res->getString("icon");

            userinfo->id = userid;
            userinfo->name = username;
            userinfo->pwd = userpwd;
            userinfo->email = useremail;
            userinfo->icon = userhead;
            userinfo->nick = usernick;
            userinfo->sex = usersex;

            std::cout << "found user! " << std::endl;
            pool_->returnConnection(std::move(con));
            return true;
        }
        else {
            std::cout << "userid is not exist!" << std::endl;
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

int MysqlDao::QueryUserFromId(std::string uid, std::shared_ptr<Userinfo>& userinfo)
{
    auto con = pool_->GetConnection();
    try
    {
        if (con == nullptr) {
            return -1;
        }

        // 选择数据库  
        con->setSchema("lddchat");

        // 准备查询语句
        std::unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("Select * from users where name = ?"));
        //指定参数
        stmt->setString(1, uid);
        //获取结果集
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());



        if (res->next()) {
            auto userid = res->getInt("id");
            auto username = res->getString("name");
            auto useremail = res->getString("email");
            auto userpwd = res->getString("pwd");
            auto usernick = res->getString("nick");
            auto usersex = res->getInt("sex");
            auto userhead = res->getString("icon");

            userinfo->id = userid;
            userinfo->name = username;
            userinfo->pwd = userpwd;
            userinfo->email = useremail;
            userinfo->icon = userhead;
            userinfo->nick = usernick;
            userinfo->sex = usersex;

            std::cout << "found user! " << std::endl;
            pool_->returnConnection(std::move(con));
            return true;
        }
        else {
            std::cout << "user name is not exist!" << std::endl;
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

int MysqlDao::GetApplyList(int to_uid, std::vector<std::shared_ptr<ApplyInfo>>& applylist, int begin, int limit)
{
    auto con = pool_->GetConnection();
    if (con == nullptr) {
        return false;
    }
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("select apply.from_uid, apply.status, users.name, "
            "users.nick, users.sex,users.icon from friend_apply as apply join users on apply.from_uid = users.id where apply.to_uid = ? "
            "and apply.id > ? order by apply.id ASC LIMIT ? "));

        pstmt->setInt(1, to_uid);
        pstmt->setInt(2, begin);
        pstmt->setInt(3, limit);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            auto name = res->getString("name");
            auto uid = res->getInt("from_uid");
            auto status = res->getInt("status");
            auto nick = res->getString("nick");
            auto sex = res->getInt("sex");
            auto icon = res->getString("icon");
            auto apply_ptr = std::make_shared<ApplyInfo>(uid,name,"",icon,nick,sex,status);
            applylist.push_back(apply_ptr);
        }
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
int MysqlDao::GetFriendList(int uid, std::vector<std::shared_ptr<Userinfo>>& friendlist, int begin, int limit)
{
    auto con = pool_->GetConnection();
    if (con == nullptr) {
        return false;
    }
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("select apply.friend_id, apply.back, users.name, "
            "users.nick, users.sex,users.icon from friend as apply join users on apply.friend_id = users.id where apply.self_id = ? "
            "and apply.id > ? order by apply.id ASC LIMIT ? "));

        pstmt->setInt(1, uid);
        pstmt->setInt(2, begin);
        pstmt->setInt(3, limit);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            auto name = res->getString("name");
            auto uid = res->getInt("friend_id");
            //auto status = res->getInt("status");
            auto nick = res->getString("nick");
            auto sex = res->getInt("sex");
            auto icon = res->getString("icon"); 
            auto back = res->getString("back");
            auto friend_ptr = std::make_shared<Userinfo>();
            
            friend_ptr->name = name;
            friend_ptr->id = uid;
            friend_ptr->nick = nick;
            friend_ptr->sex = sex;
            friend_ptr->icon = icon;
            friend_ptr->back = back;
            friendlist.push_back(friend_ptr);
        }
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
//验证好友申请，uid是收到好友申请的一方，to_uid是发送好友申请的一方
int MysqlDao::AuthFriendApply(int uid, int to_uid)
{
    auto con = pool_->GetConnection();
    if (con == nullptr) {
        return false;
    }

    Defer defer([this, &con]() {
        pool_->returnConnection(std::move(con));
        });

    try {
        // 准备SQL语句
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("UPDATE friend_apply SET status = 1 "
            "WHERE from_uid = ? AND to_uid = ?"));
        //反过来的申请时from，验证时to
        pstmt->setInt(1, to_uid); // from id
        pstmt->setInt(2, uid);
        // 执行更新
        int rowAffected = pstmt->executeUpdate();
        if (rowAffected < 0) {
            return false;
        }
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }


    return true;
}

int MysqlDao::AddFriend(int uid, int to_uid, std::string bkname)
{
    auto con = pool_->GetConnection();
    if (con == nullptr) {
        return false;
    }

    Defer defer([this, &con]() {
        pool_->returnConnection(std::move(con));
        });

    try {

        //开始事务
        con->setAutoCommit(false);

        // 准备第一个SQL语句, 插入认证方好友数据
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT IGNORE INTO friend(self_id, friend_id, back) "
            "VALUES (?, ?, ?) "
        ));
        //反过来的申请时from，验证时to
        pstmt->setInt(1, uid); // from id
        pstmt->setInt(2, to_uid);
        pstmt->setString(3, bkname);
        // 执行更新
        int rowAffected = pstmt->executeUpdate();
        if (rowAffected < 0) {
            con->rollback();
            return false;
        }

        //准备第二个SQL语句，插入申请方好友数据
        std::unique_ptr<sql::PreparedStatement> pstmt2(con->prepareStatement("INSERT IGNORE INTO friend(self_id, friend_id, back) "
            "VALUES (?, ?, ?) "
        ));
        //反过来的申请时from，验证时to
        pstmt2->setInt(1, to_uid); // from id
        pstmt2->setInt(2, uid);
        pstmt2->setString(3, "");
        // 执行更新
        int rowAffected2 = pstmt2->executeUpdate();
        if (rowAffected2 < 0) {
            con->rollback();
            return false;
        }

        // 提交事务
        con->commit();
        std::cout << "addfriend insert friends success" << std::endl;

        return true;
    }
    catch (sql::SQLException& e) {
        // 如果发生错误，回滚事务
        if (con) {
            con->rollback();
        }
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }


    return true;
}

int MysqlDao::AddFriendApply(int uid, int touid)
{
    auto con = pool_->GetConnection();
    if (con == nullptr) {
        return false;
    }
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO friend_apply(from_uid,to_uid) values(?,?)"
        "ON DUPLICATE KEY UPDATE from_uid = from_uid, to_uid = to_uid"));
        pstmt->setInt(1, uid);
        pstmt->setInt(2, touid);
        // 执行更新
        int res = pstmt->executeUpdate();
        return res > 0;

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
