#pragma once
#include "turistacsoport.h"
#include <vector>
#include <fstream>
#define MAXALLAPOT 100
#define MINALLAPOT 1

class Varos {
private:
    int _allapot;
    std::vector<Turistacsoport*> turistatervez;
    std::vector<Turistacsoport*> turistamegy;
    std::vector<int> allapotok;
    std::vector<int> elottallapot;
    std::vector<int> bevetelek;

public:
    enum ERRORS {FileError};
    //100 000 Ft turistankent, 1 000 000 000 Ft felett minden 20 000 000 Ft = 1 pont javulas
    //100 Ft turistankent, 1 000 000 Ft felett minden 20 000 Ft = 1 pont javulas
    //1 Ft turistankent, 10 000 Ft felett minden 200 Ft = 1 pont javulas
    //utobbit fogom hasznalni penznek, felesleges lenne tobb nullaval szamolni
    Varos(){}
    void felujitas(int osszeg){
        elottallapot.push_back(_allapot);
        //std::cout << "Osszeg: " << osszeg <<std::endl;
        if (osszeg>10000){  //(14023-10000)/200 = 20, szoval a sima div jo lesz
            _allapot += (osszeg - 10000)/200;
            //std::cout << "Javult allapot: " << _allapot <<std::endl;
        }
        if (_allapot > MAXALLAPOT){
            _allapot = MAXALLAPOT;
        }
    }

    void rontas(int m, int h){
        _allapot -= ((m/100) + (h/50));
        //std::cout << "Rontott allapot: " << _allapot <<std::endl;
        //ugy tunik ezt nem en rontottam el, csak a 4000 modern es 6000 harmadik kicsit tobbet ront a varos allapotan, mint amennyi elfogadhato lenne
        if (_allapot < MINALLAPOT){
            _allapot = MINALLAPOT;
        }
    }

    void turistaSzezon(int japantervez,int moderntervez,int harmadiktervez){
        turistatervez.push_back(new Japan(japantervez));
        turistatervez.push_back (new Modern(moderntervez));
        turistatervez.push_back (new Harmadik(harmadiktervez));
    }

    void turistaMent(){
        int i=0,j,m,h;
        while(i != turistatervez.size()){
            j=turistatervez[i]->getDb();
            i++;
            m=turistatervez[i]->getDb();
            i++;
            h=turistatervez[i]->getDb();
            i++;

            if (_allapot<=33){
                lepusztultallapot(j,m,h);
            }else if (_allapot > 67){
                joallapot(j,m,h);
            }else{
                atlagosallapot(j,m,h);
            }
            allapotok.push_back(_allapot);
        }
    }


    void joallapot(int japantervez,int moderntervez,int harmadiktervez){
        int j,m,h;
        j=japantervez*(1.2);
        m=moderntervez*(1.3);
        h=harmadiktervez;
        rontas(m,h);
        //std::cout << "Ennyi ember ment: " << j+m+h <<std::endl;
        felujitas(j+m+h);
        bevetelek.push_back( (j+m+h)* 100000 );
        turistamegy.push_back(new Japan(j));
        turistamegy.push_back(new Modern(m));
        turistamegy.push_back(new Harmadik(h));
    }

    void atlagosallapot(int japantervez,int moderntervez,int harmadiktervez){
        int j,m,h;
        j=japantervez;
        m=moderntervez*(1.1);
        h=harmadiktervez*(1.1);
        rontas(m,h);
        //std::cout << "Ennyi ember ment: " << j+m+h <<std::endl;
        felujitas(j+m+h);
        bevetelek.push_back( (j+m+h)* 100000 );
        turistamegy.push_back(new Japan(j));
        turistamegy.push_back(new Modern(m));
        turistamegy.push_back(new Harmadik(h));
    }

    void lepusztultallapot(int japantervez,int moderntervez,int harmadiktervez){
        int j,m,h;
        j=0;
        m=moderntervez;
        h=harmadiktervez;
        rontas(m,h);
        //std::cout << "Ennyi ember ment: " << j+m+h <<std::endl;
        felujitas(j+m+h);
        bevetelek.push_back( (j+m+h)* 100000 );
        turistamegy.push_back(new Japan(0));
        turistamegy.push_back(new Modern(m));
        turistamegy.push_back(new Harmadik(h));
    }

    void beolvas(const std::string &fileName){
        std::ifstream f(fileName);
        if(f.fail())
        {
            throw(FileError);
        }

        unsigned int allapot;
        unsigned int ev;
        f >> allapot;
        f >> ev;

        _allapot=allapot;

        int japantervez,moderntervez,harmadiktervez;
        for (unsigned int i=0;i<ev;++i){
            f >> japantervez >> moderntervez >> harmadiktervez;
            turistaSzezon(japantervez,moderntervez,harmadiktervez);
        }
    }

    void kiiras(){
        int i=0,j,m,h;
        int evszam=1;
        std::string e,u;
        while(i != turistatervez.size()){
            j=i;
            m=i+1;
            h=i+2;
            if (elottallapot[evszam-1]<=33){
                e="Lepusztult";
            }else if (elottallapot[evszam-1] > 67){
                e="Jo allapotu";
            }else{
                e="Atlagos";
            }
            if (allapotok[evszam-1]<=33){
                u="Lepusztult";
            }else if (allapotok[evszam-1] > 67){
                u="Jo allapotu";
            }else{
                u="Atlagos";
            }
            std::cout << "----------" << evszam << ".EV----------" << std::endl << "Allapot a felujitas elott (az ev vegen): " << elottallapot[evszam-1] << " (" << e << ")" << std::endl << "Allapot a felujitas utan (az ev vegen): " << allapotok[evszam-1] << " (" << u << ")" << std::endl << "Bevetel az ev vegen: " << bevetelek[evszam-1] <<std::endl << std::endl << "Tervezett jonni:" << std::endl;
            std::cout <<turistatervez[j]->getDb() << " japan tervezett." <<std::endl;
            std::cout <<turistatervez[m]->getDb() << " modern tervezett." <<std::endl;
            std::cout <<turistatervez[h]->getDb() << " harmadik tervezett." <<std::endl;
            std::cout << std::endl << "Valoban eljott:" <<std::endl;
            std::cout <<turistamegy[j]->getDb() << " japan jott." << std::endl;
            std::cout <<turistamegy[m]->getDb() << " modern jott." <<std::endl;
            std::cout <<turistamegy[h]->getDb() << " harmadik jott." <<std::endl << std::endl;
            i += 3;
            evszam++;
        }
    }


    //tesztesetekhez:

    int getAllapot(const int i) {return allapotok[i];}

    int getTervezElem(const int i){
        return (turistatervez[i]->getDb());
    }

    int getTervezSize(){
        return (turistatervez.size());
    }

    bool tervezEquals(Varos* varos){
        if (turistatervez.size() != varos->getTervezSize()){
            return false;
        }
        for (int i=0;i<turistatervez.size();++i){
            if (turistatervez[i]->getDb() != varos->getTervezElem(i)){
                return false;
            }
        }
        return true;    //ha hamis lenne mar kiszalltunk volna, ha eljut a vegere, fixen igaz
    }

    int getMegyElem(const int i){
        return (turistamegy[i]->getDb());
    }

    int getMegySize(){
        return (turistamegy.size());
    }

    bool megyEquals(Varos* varos){
        if (turistamegy.size() != varos->getMegySize()){
            return false;
        }
        for (int i=0;i<turistamegy.size();++i){
            if (turistamegy[i]->getDb() != varos->getMegyElem(i)){
                return false;
            }
        }
        return true;    //ha hamis lenne mar kiszalltunk volna, ha eljut a vegere, fixen igaz
    }
};
