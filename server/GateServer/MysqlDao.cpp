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

int MysqlDao::Query()
{
    auto con = pool_->GetConnection();
    try
    {
        if (con == nullptr) {
            return -1;
        }

        // ѡ�����ݿ�  
        con->setSchema("lddchat");

        // ִ�в�ѯ  
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
