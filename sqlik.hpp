#ifndef PROCCESS_SQL_HPP
#define PROCCESS_SQL_HPP

#include <iostream>
#include <memory>
#include <set>
#include <sqlite3.h>
#include <string>
#include <vector>

struct Dish {
    int id;
    std::string name;
    double calories;
    std::string ingredients;
};

class sqlik {
public:
    sqlik();                
    ~sqlik();               
    int GenerateUniqueID();
    bool AddDish(const std::string& name, double calories, const std::string& ingredients); 
    std::vector<Dish> GetDishes(); 
    void DeleteDish(int id); 
    void UpdateDish(int id, const std::string& name, double calories, const std::string& ingredients);
private:
    sqlite3* db;                 
};

#endif
