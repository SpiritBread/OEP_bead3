#pragma once

class Turistacsoport{
protected:
    int _db;
public:
    Turistacsoport(int darab){_db=darab;}
    virtual int getDb() = 0;
};

class Japan : public Turistacsoport{
public:
    Japan(int darab) : Turistacsoport(darab) {}
    int getDb() override {return _db;}
};

class Modern : public Turistacsoport{
public:
    Modern(int darab) : Turistacsoport(darab) {}
    int getDb() override {return _db;}
};

class Harmadik : public Turistacsoport{
public:
    Harmadik(int darab) : Turistacsoport(darab) {}
    int getDb() override {return _db;}
};
