#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace evc::database {

class DatabaseManager {
private:
    sqlite3* db;
    std::string dbPath;
    bool isConnected;
    static DatabaseManager* instance;

    // Private constructor for singleton
    DatabaseManager();

public:
    // Singleton pattern
    static DatabaseManager* getInstance();
    static void destroyInstance();

    // Connection management
    bool connect(const std::string& path);
    bool disconnect();
    bool isDBConnected() const { return isConnected && db != nullptr; }

    // Database operations
    bool executeUpdate(const std::string& sql);
    bool executeUpdateWithParams(const std::string& sql, const std::vector<std::string>& params);
    
    // Query operations
    sqlite3_stmt* prepareStatement(const std::string& sql);
    bool executeQuery(const std::string& sql, std::vector<std::map<std::string, std::string>>& results);
    
    // Utility functions
    int getLastInsertRowId() const;
    int getChangesCount() const;
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    
    // Schema management
    bool initializeDatabase();
    bool createTables();
    bool dropAllTables();
    
    // Error handling
    std::string getLastError() const;
    int getLastErrorCode() const;

    // Destructor
    ~DatabaseManager();

private:
    // Helper methods
    bool executeSQL(const std::string& sql);
    bool loadSchema();
};

}

#endif // DATABASE_MANAGER_H