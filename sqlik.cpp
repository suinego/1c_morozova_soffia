#include "sqlik.hpp"

sqlik::sqlik() {
    int64_t error = sqlite3_open("/home/sofiia/Desktop/caloriedirectory/food.db", &db);
    if (error) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        exit(1);
    }

    const char sql_dishes[] =
        "CREATE TABLE IF NOT EXISTS dishes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "calories REAL NOT NULL,"
        "ingredients TEXT NOT NULL"
        ");";

    error = sqlite3_exec(db, sql_dishes, nullptr, nullptr, nullptr);
    if (error != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        exit(1);
    }
}

sqlik::~sqlik() {
    sqlite3_close(db);
}

// Метод для добавления блюда
bool sqlik::AddDish(const std::string& name, double calories, const std::string& ingredients) {
    char* sql = sqlite3_mprintf(
        "INSERT INTO dishes (name, calories, ingredients) VALUES ('%q', %f, '%q');",
        name.c_str(), calories, ingredients.c_str());
    
    int64_t error = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
    sqlite3_free(sql);
    
    if (error != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
        return false;
    }
    return true;
}

std::vector<Dish> sqlik::GetDishes() {
    std::vector<Dish> dishes;
    std::string query = "SELECT id, name, calories, ingredients FROM dishes;";
    
    sqlite3_stmt* stmt;
    int64_t error = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (error != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return dishes;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Dish dish;
        dish.id = sqlite3_column_int(stmt, 0);
        dish.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        dish.calories = sqlite3_column_double(stmt, 2);
        dish.ingredients = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        dishes.push_back(dish);
    }
    
    sqlite3_finalize(stmt);
    return dishes;
}

void sqlik::UpdateDish(int id, const std::string& name, double calories, const std::string& ingredients) {
    char* sql = sqlite3_mprintf(
        "UPDATE dishes SET name = '%q', calories = %f, ingredients = '%q' WHERE id = %d;",
        name.c_str(), calories, ingredients.c_str(), id);

    int64_t error = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
    if (error != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
    }
    sqlite3_free(sql);
}

void sqlik::DeleteDish(int id) {
    char* sql = sqlite3_mprintf("DELETE FROM dishes WHERE id = %d;", id);

    int64_t error = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
    if (error != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
    }
    sqlite3_free(sql);
}

