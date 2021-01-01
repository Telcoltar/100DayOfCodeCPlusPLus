#include <map>
#include <stdexcept>

struct AccountDatabase {
    virtual void updateBalance(long accountId, double amount)=0;
    virtual ~AccountDatabase() = default;
};

struct InMemoryAccountDatabase : AccountDatabase {
    std::map<long, double> database;

    void createAccount(long id) {
        database[id] = 0;
    }

    void updateBalance(long accountId, double amount) override {
        if (database.find(accountId) != database.end()) {
            database[accountId] += amount;
        } else {
            throw std::runtime_error("Account not found");
        }
    }

    double getBalance(long accountId) {
        if (database.find(accountId) != database.end()) {
            return database[accountId];
        } else {
            throw std::runtime_error("Account not found");
        }
    }
};

int main() {
    InMemoryAccountDatabase db;
    db.createAccount(12);
    db.updateBalance(12, 40.5);
    printf("Balance of Account with id 12: %f\n", db.getBalance(12));
    return 0;
}