#include "DatabaseManager.h"
#include <sstream>

DatabaseManager::DatabaseManager(const std::string& dbPath) : db(nullptr), dbPath(dbPath) {}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::init(std::string& error) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        return false;
    }

    const char* usersTableSql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE,"
        "password TEXT,"
        "role TEXT"
        ");";

    const char* escrowTableSql =
        "CREATE TABLE IF NOT EXISTS escrow_accounts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "buyer_id INTEGER,"
        "seller_id INTEGER,"
        "agent_id INTEGER,"
        "amount REAL,"
        "balance REAL,"
        "status TEXT"
        ");";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, usersTableSql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        error = errMsg ? errMsg : "Failed to create users table";
        sqlite3_free(errMsg);
        return false;
    }

    if (sqlite3_exec(db, escrowTableSql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        error = errMsg ? errMsg : "Failed to create escrow_accounts table";
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool DatabaseManager::createUser(const std::string& username, const std::string& password, const std::string& role, std::string& error) {
    const char* sql = "INSERT INTO users (username, password, role) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseManager::getUserByUsername(const std::string& username, User& user, std::string& error) {
    const char* sql = "SELECT id, username, password, role FROM users WHERE username = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* unamePtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* pwdPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* rolePtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string uname = unamePtr ? unamePtr : "";
        std::string pwd = pwdPtr ? pwdPtr : "";
        std::string role = rolePtr ? rolePtr : "";
        user = User(id, uname, pwd, role);
        sqlite3_finalize(stmt);
        return true;
    }

    error = "User not found";
    sqlite3_finalize(stmt);
    return false;
}

bool DatabaseManager::getUserById(int userId, User& user, std::string& error) {
    const char* sql = "SELECT id, username, password, role FROM users WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, userId);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* unamePtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* pwdPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* rolePtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string uname = unamePtr ? unamePtr : "";
        std::string pwd = pwdPtr ? pwdPtr : "";
        std::string role = rolePtr ? rolePtr : "";
        user = User(id, uname, pwd, role);
        sqlite3_finalize(stmt);
        return true;
    }

    error = "User not found";
    sqlite3_finalize(stmt);
    return false;
}

bool DatabaseManager::validateUser(const std::string& username, const std::string& password, User& user, std::string& error) {
    const char* sql = "SELECT id, username, password, role FROM users WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* unamePtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* pwdPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* rolePtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string uname = unamePtr ? unamePtr : "";
        std::string pwd = pwdPtr ? pwdPtr : "";
        std::string role = rolePtr ? rolePtr : "";
        user = User(id, uname, pwd, role);
        sqlite3_finalize(stmt);
        return true;
    }

    error = "Invalid username or password";
    sqlite3_finalize(stmt);
    return false;
}

bool DatabaseManager::createEscrow(int buyerId, int sellerId, int agentId, double amount, int& escrowId, std::string& error) {
    const char* sql = "INSERT INTO escrow_accounts (buyer_id, seller_id, agent_id, amount, balance, status) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, buyerId);
    sqlite3_bind_int(stmt, 2, sellerId);
    sqlite3_bind_int(stmt, 3, agentId);
    sqlite3_bind_double(stmt, 4, amount);
    sqlite3_bind_double(stmt, 5, 0.0);
    sqlite3_bind_text(stmt, 6, "INITIATED", -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    escrowId = static_cast<int>(sqlite3_last_insert_rowid(db));
    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseManager::getEscrowById(int escrowId, EscrowAccount& account, std::string& error) {
    const char* sql = "SELECT id, buyer_id, seller_id, agent_id, amount, balance, status FROM escrow_accounts WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, escrowId);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int buyerId = sqlite3_column_int(stmt, 1);
        int sellerId = sqlite3_column_int(stmt, 2);
        int agentId = sqlite3_column_int(stmt, 3);
        double amount = sqlite3_column_double(stmt, 4);
        double balance = sqlite3_column_double(stmt, 5);
        const char* statusPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        std::string status = statusPtr ? statusPtr : "INITIATED";
        account = EscrowAccount(id, buyerId, sellerId, agentId, amount, balance, status);
        sqlite3_finalize(stmt);
        return true;
    }

    error = "Escrow account not found";
    sqlite3_finalize(stmt);
    return false;
}

bool DatabaseManager::updateEscrow(int escrowId, const std::string& status, double balance, std::string& error) {
    const char* sql = "UPDATE escrow_accounts SET status = ?, balance = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, balance);
    sqlite3_bind_int(stmt, 3, escrowId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseManager::getEscrowsForUser(int userId, std::vector<EscrowAccount>& escrows, std::string& error) {
    const char* sql = "SELECT id, buyer_id, seller_id, agent_id, amount, balance, status FROM escrow_accounts WHERE buyer_id = ? OR seller_id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, userId);

    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int buyerId = sqlite3_column_int(stmt, 1);
        int sellerId = sqlite3_column_int(stmt, 2);
        int agentId = sqlite3_column_int(stmt, 3);
        double amount = sqlite3_column_double(stmt, 4);
        double balance = sqlite3_column_double(stmt, 5);
        const char* statusPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        std::string status = statusPtr ? statusPtr : "INITIATED";
        escrows.emplace_back(id, buyerId, sellerId, agentId, amount, balance, status);
    }

    if (rc != SQLITE_DONE) {
        error = sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
