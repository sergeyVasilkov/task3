#ifndef TASK3_PATHFINDER_H
#define TASK3_PATHFINDER_H

#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
using namespace std;


class Pathfinder {
public:
    string resultPath;
    int referenceCounter;

    Pathfinder(){
        this->resultPath = new char[1024];
        referenceCounter = 0;
    }

    void getCurrentPath()
    {
        struct stat rootstat{};

        if (lstat("/", &rootstat) == -1)
        {
            return;
        }

        string currpath;
        currpath += ".";

        while (true)
        {
            struct stat currstat{};
            if (lstat(currpath.c_str(), &currstat) == -1)
            {
                return;
            }

            if (equalsStats(rootstat, currstat)) { //проверка дошли ли мы до rootstat
                break;
            }

            string upDir;
            upDir = "../" + currpath ;

            DIR *dir = opendir(upDir.c_str());

            if (dir == nullptr) //проверка вдруг opendir не смогла открыть папку из за прав доступа и т.п.
            {
                cerr << "error: opendir " << upDir << endl << "result path: " << resultPath.c_str() << endl;
                return;
            }

            dirent *de;
            while ((de = readdir(dir)) != nullptr)
            {
                //так как мы будем идти по всем файлам этой директории, то скипаем точку и точку точку
                if ((strcmp(de->d_name, ".")) == 0) continue;
                if ((strcmp(de->d_name, "..")) == 0) continue;

                string depath;
                depath += upDir;
                depath += "/";
                depath += de->d_name;

                struct stat destat{};

                if (lstat(depath.c_str() , &destat) == -1)
                {
                    cerr << "error: lstat for " << depath << endl;
                    return;
                }

                if (equalsStats(destat, currstat)) {
                      string tmp;
                      tmp+="/";
                      tmp+=de->d_name;
                    resultPath = tmp + resultPath;
                    break;
                }
            }
            closedir(dir);
            currpath = "../"+ currpath;
        }
    }


    static bool equalsStats(struct stat s1, struct stat s2){
        return s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino;
    }


    int referenceCountSum(){

        struct stat rootstat{};

        if (lstat("/", &rootstat) == -1)
        {
            return-1;
        }

        string currpath;
        currpath += ".";

        while (true)
        {
            struct stat currstat{};
            if (lstat(currpath.c_str(), &currstat) == -1)
            {
                return-1;
            }

            if (equalsStats(rootstat, currstat)) {
                break;
            }

            string upDir;
            upDir = "../"+ currpath;

            DIR *dir = opendir(upDir.c_str());

            if (dir == nullptr)
            {
                cerr << "error: opendir " << upDir << endl << "result path: " << resultPath.c_str() << endl;
                return-1;
            }

            dirent *de;
            while ((de = readdir(dir)) != nullptr)
            {
                //так как мы будем идти по всем файлам этой директории, то скипаем точку и точку точку
                if ((strcmp(de->d_name, ".")) == 0) continue;
                if ((strcmp(de->d_name, "..")) == 0) continue;

                string depath;
                depath += upDir;
                depath += "/";
                depath += de->d_name;

                struct stat destat{};

                if (lstat(depath.c_str() , &destat) == -1)
                {
                    cerr << "error: lstat for " << depath << endl;
                    return -1;
                }


                if (S_ISLNK(destat.st_mode)) {
                    std::cout <<endl<< "SLINK!!!!" << endl;
                    std::cout << depath;

                    struct stat destatTemp{};
                    struct stat statTemp{};
                    struct stat lstatTemp{};

                    if (stat(depath.c_str() , &destatTemp) == -1)
                    {
                        cerr << "error: lstat for " << depath << endl;
                        return -1;
                    }


                    if (stat(this->resultPath.c_str(), &statTemp) == -1) {
                        cerr << "error: lstat for " << resultPath << endl;
                        cin.get();
                        exit(1);
                    }

                    if (lstat(this->resultPath.c_str(), &lstatTemp) == -1) {
                        cerr << "error: lstat for " << resultPath << endl;
                        cin.get();
                        exit(1);
                    }

                    //проверка, если через stat совпадают, а через lstat не совпадают - значит это наша символьная ссылка
                    if (equalsStats(destatTemp, statTemp)) {
                        if (!equalsStats(destat, lstatTemp)) {
                            referenceCounter++;
                        }
                    }
                }

                if (equalsStats(destat, currstat)) {
                    break;
                }
            }
            closedir(dir);
            currpath = "../"+ currpath;
        }
        return 0;
    }

};


#endif //TASK3_PATHFINDER_H
