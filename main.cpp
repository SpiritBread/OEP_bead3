#include <iostream>
#include "varos.h"


using namespace std;

#define NORMAL_MODE
#ifdef NORMAL_MODE


int main()
{
    Varos* varos= new Varos();
    try{
        varos->beolvas("bemenet 2.txt");  //bemenet.txt, bemenet 2.txt
    }catch(Varos::ERRORS e){
        if (e == Varos::FileError){
            std::cerr << "Wrong file name!\n";
        }
    }

    varos->turistaMent();
    varos->kiiras();
    //cout << "Japomegy: " << varos->getMegyElem(0) <<endl;
    return 0;
}

#else
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Rossz faljnev") {
    Varos* varos = new Varos();
    CHECK_THROWS(varos->beolvas("heble.txt"));
}

TEST_CASE("Adattarolas"){
    SECTION("Tervez"){
        Varos* v1 = new Varos();
        v1->beolvas("bemenet.txt");
        Varos* v2 = new Varos();
        v2->beolvas("bemenet.txt");
        CHECK(v1->tervezEquals(v2));
    }

    SECTION("Megy"){
        Varos* v1 = new Varos();
        v1->beolvas("bemenet.txt");
        Varos* v2 = new Varos();
        v2->beolvas("bemenet.txt");
        CHECK(v1->megyEquals(v2));
    }
}

TEST_CASE("Turistament"){
    SECTION("Lepusztult"){
        Varos* varos= new Varos();
        varos->beolvas("lepusztult.txt");
        varos->turistaMent();
        CHECK(varos->getMegyElem(0) == 0);
        CHECK(varos->getMegyElem(1) == varos->getTervezElem(1));
        CHECK(varos->getMegyElem(2) == varos->getTervezElem(2));
    }

    SECTION("Atlagos"){
        Varos* varos= new Varos();
        varos->beolvas("atlagos.txt");
        varos->turistaMent();
        CHECK(varos->getMegyElem(0) == varos->getTervezElem(0));
        CHECK(varos->getMegyElem(1) == (varos->getTervezElem(1))*1.1);
        CHECK(varos->getMegyElem(2) == (varos->getTervezElem(2))*1.1);
    }

    SECTION("Jo"){
        Varos* varos= new Varos();
        varos->beolvas("jo.txt");
        varos->turistaMent();
        CHECK(varos->getMegyElem(0) == (varos->getTervezElem(0))*1.2);
        CHECK(varos->getMegyElem(1) == (varos->getTervezElem(1))*1.3);
        CHECK(varos->getMegyElem(2) == varos->getTervezElem(2));
    }
}

TEST_CASE("Allapot"){
    SECTION("Lepusztult -> Jo"){
        Varos* varos= new Varos();
        varos->beolvas("allapot1.txt");
        varos->turistaMent();
        CHECK(varos->getAllapot(0)>67);
    }

    SECTION("Atlagos -> Jo"){
        Varos* varos= new Varos();
        varos->beolvas("allapot2.txt");
        varos->turistaMent();
        CHECK(varos->getAllapot(0)>67);
    }

    SECTION("Jo -> Lepusztult"){
        Varos* varos= new Varos();
        varos->beolvas("allapot3.txt");
        varos->turistaMent();
        CHECK(varos->getAllapot(0)<=33);
    }

    SECTION("Jo -> Atlagos"){
        Varos* varos= new Varos();
        varos->beolvas("allapot4.txt");
        varos->turistaMent();
        CHECK(varos->getAllapot(0)>33);
        CHECK(varos->getAllapot(0)<=67);
    }
}

#endif
