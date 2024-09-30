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
        // ׼�����ô洢����
        std::unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("CALL RegisterUser(?,?,?,@result)"));
        // �����������
        stmt->setString(1, name);
        stmt->setString(2, email);
        stmt->setString(3, pwd);
        
        // ����PreparedStatement��ֱ��֧��ע�����������������Ҫʹ�ûỰ������������������ȡ���������ֵ
          // ִ�д洢����
        stmt->execute();
        // ����洢���������˻Ự��������������ʽ��ȡ���������ֵ�������������ִ��SELECT��ѯ����ȡ����
       // ���磬����洢����������һ���Ự����@result���洢������������������ȡ��
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
        // ׼����ѯ���
        std::unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("Select id from users where email = ?"));
        //ָ������
        stmt->setString(1, email);
        //��ȡ�����
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        //���������
        if(res->next()) {
            std::cout << "this Email user id is: " << res->getString("id") << std::endl;
            pool_->returnConnection(std::move(con));
            return true;
        }
        //û���ҵ�������
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
        // ׼����ѯ���
        std::unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("Select * from users where email = ?"));
        //ָ������
        stmt->setString(1, email);
        //��ȡ�����
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        //û���ҵ�������
        
        //���������
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

        // ѡ�����ݿ�  
        con->setSchema("lddchat");

        // ׼����ѯ���
        std::unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("Select * from users where id = ?"));
        //ָ������
        stmt->setInt(1,uid);
        //��ȡ�����
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

        // ѡ�����ݿ�  
        con->setSchema("lddchat");

        // ׼����ѯ���
        std::unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("Select * from users where name = ?"));
        //ָ������
        stmt->setString(1, uid);
        //��ȡ�����
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
//��֤�������룬uid���յ����������һ����to_uid�Ƿ��ͺ��������һ��
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
        // ׼��SQL���
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("UPDATE friend_apply SET status = 1 "
            "WHERE from_uid = ? AND to_uid = ?"));
        //������������ʱfrom����֤ʱto
        pstmt->setInt(1, to_uid); // from id
        pstmt->setInt(2, uid);
        // ִ�и���
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

        //��ʼ����
        con->setAutoCommit(false);

        // ׼����һ��SQL���, ������֤����������
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT IGNORE INTO friend(self_id, friend_id, back) "
            "VALUES (?, ?, ?) "
        ));
        //������������ʱfrom����֤ʱto
        pstmt->setInt(1, uid); // from id
        pstmt->setInt(2, to_uid);
        pstmt->setString(3, bkname);
        // ִ�и���
        int rowAffected = pstmt->executeUpdate();
        if (rowAffected < 0) {
            con->rollback();
            return false;
        }

        //׼���ڶ���SQL��䣬�������뷽��������
        std::unique_ptr<sql::PreparedStatement> pstmt2(con->prepareStatement("INSERT IGNORE INTO friend(self_id, friend_id, back) "
            "VALUES (?, ?, ?) "
        ));
        //������������ʱfrom����֤ʱto
        pstmt2->setInt(1, to_uid); // from id
        pstmt2->setInt(2, uid);
        pstmt2->setString(3, "");
        // ִ�и���
        int rowAffected2 = pstmt2->executeUpdate();
        if (rowAffected2 < 0) {
            con->rollback();
            return false;
        }

        // �ύ����
        con->commit();
        std::cout << "addfriend insert friends success" << std::endl;

        return true;
    }
    catch (sql::SQLException& e) {
        // ����������󣬻ع�����
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
        // ִ�и���
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
    // ��������  
    std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO users (name,email,pwd) VALUES (?,?,?)"));
    pstmt->setString(1, "cjqtest");
    pstmt->setString(2, "12345@qq.com");
    pstmt->setString(3, "123456");
    pstmt->executeUpdate();

    std::cout << "Row inserted successfully." << std::endl;
    pool_->returnConnection(std::move(con));
    return 1;
}
