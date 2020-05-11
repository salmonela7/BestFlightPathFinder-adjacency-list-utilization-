/*Duotas lėktuvų skrydžių sąrašas: miestas, miestas, išskridimo laikas, skrydžio trukmė.
Rasti greičiausio skrydžio iš vieno duoto miesto į kitą duotą miestą maršrutą ir jo laiką
(pradinis išskridimo laikas yra nesvarbus). Numatyti atvejį, kad toks maršrutas neegzistuoja.
(grafo realizacija paremta kaiminystės sąrašais) */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <stack>

using namespace std;

void readFile();
void getList();
void setAdjLists();
double toDouble(string str);


class Flight
{
public:
    string from, too, deptime, length;
    bool visited;
    Flight (string fr, string to, string dt, string len) : from(fr), too(to), deptime(dt), length(len), visited(false){}
    string getFrom()
    {
        return from;
    }
    string getTo()
    {
        return too;
    }
    string getDeptime()
    {
        return deptime;
    }
    string getLength()
    {
        return length;
    }
    bool isVisited()
    {
        return visited;
    }
    void setVisited(bool visit)
    {
        visited = visit;
    }
};

class CompleteFlight
{
public:
    list <Flight> compflight;
    double size;
    CompleteFlight (list<Flight> lst, double sz) : compflight(lst), size(sz) {}
    double getSize()
    {
        return size;
    }
    list<Flight> getFlight()
    {
        return compflight;
    }
};

list<Flight> listas;

void readFile()
{
    ifstream file;
    file.open ("skrydziai.txt");
    if (file.is_open())
    {
        string word;
        string words[4];
        int i = 0;
        int k = 0;
        while (file >> word)
        {
            words[i] = word;
            i++;
            if (i == 4)
            {
                k++;
                i = 0;
                listas.push_front(Flight(words[0], words[1], words[2], words[3]));
            }
        }
    }
}

list<Flight> AdjNodeList;
list<Flight> FlightList;
list<Flight> Flights;
list<list<Flight> > AdjList;
list<CompleteFlight> CompleteFlightLists;
list<list<Flight> > CompleteFlights;
stack<string> stekas;
CompleteFlight getBest();
bool areVisited(string frm);
bool allVisited(string frm);

double timee = 0;
bool isFound (list<Flight> lst, string frm, string too);
bool isLast(string frm, string tu);

int main ()
{
    readFile();
    setAdjLists();
    getList();
    string from, to;
    cout << "Is kur norite skristi? ";
    cin >> from;
    cout << "I kur norite skristi? ";
    cin >> to;
    cout << endl;

    for (list<list<Flight> >::iterator itr = AdjList.begin(); itr != AdjList.end(); itr++)
    {
        for (list<Flight>::iterator it=itr->begin(); it != itr->end(); ++it)
        {
            if (it->getFrom() == from && it->getTo() == to)
            {
                cout << "Skrydis rastas! " << it->getFrom() << " " << it->getTo() << " " << it->getDeptime() << " " << it->getLength() << endl;
                return 0;
            }
        }
    }
    string fromm = from;
    bool found = false;
    bool is = false;

    int k = 0;


forLoop:
    for (list<list<Flight> >::iterator itr = AdjList.begin(); itr != AdjList.end(); itr++)
    {
        for (list<Flight>::iterator it=itr->begin(); it != itr->end(); it++)
        {
            if (it->getFrom() == fromm && !it->isVisited() && !found/* && !isLast(itr->begin()->getFrom(), itr->begin()->getTo())*/)
            {
                if (it->getTo() == to)
                {
                    found = true;
                    it->setVisited(true);
                }
                //isidedu skrydi i lista
                FlightList.push_back(*it);
                timee += toDouble(it->getLength());

                //nusistatau nauja skrydi
                stekas.push(fromm);

                //jei skrydis paskutinis, nusistatau kad aplankiau ji
                if (isLast(it->getFrom(), it->getTo()) && !found)
                {

                    it->setVisited(true);
                    FlightList.clear();
                    timee = 0;
                    fromm = stekas.top();
                    stekas.pop();

                    int o = 0;
                    list<Flight>::iterator ite=itr->begin();

                    for (; ite != itr->end(); ite++)
                    {
                        if (ite->isVisited())
                        {
                            o++;
                        }
                    }

                    if (itr->size() == o)
                    {
                        if (stekas.size() > 0)
                        {
                            fromm = stekas.top();
                            stekas.pop();
                        }

                    }
                }
                else if (!isLast(it->getFrom(), it->getTo()) && !found)
                {
                    fromm = it->getTo();
                }
                //resetinu paieska
                if (found)
                {
                    CompleteFlightLists.push_front(CompleteFlight(FlightList, timee));
                    FlightList.clear();
                    timee = 0;
                    found = false;
                    stekas.empty();
                    fromm = from;
                }
                goto forLoop;
                }


        }
    }


    if (CompleteFlightLists.size() > 0)
    {
    CompleteFlight bestlist = getBest();
    list <Flight> best = getBest().getFlight();
    cout << "Skrydziu marsrutas rastas! " << endl;
    for (list<Flight>::iterator it=best.begin(); it != best.end(); ++it)
        {
            cout << it->getFrom()<< " " << it->getTo() << " " << it->getDeptime() << " " << it->getLength() << endl;
        }
        cout << "Kelione uztruko: " << bestlist.getSize() << endl;

        cout << endl;
    }
    else
    {
        cout << "Keliones rasti nepavyko" << endl;
    }




    return 0;
}

bool allVisited(string frm)
{
    int counter = 0;
    bool tru = false;
    list<list<Flight> >::iterator iter;
    for (list<list<Flight> >::iterator itr=AdjList.begin(); itr != AdjList.end(); ++itr)
    {
        for (list<Flight>::iterator it=itr->begin(); it != itr->end(); ++it)
        {
//            if (it->getFrom() == frm && it->isVisited())
//            {
//                iter = itr;
//                counter++;
//            }
        }
    }
    if (counter == iter->size())
    {
        tru = true;
    }
    return tru;
}

bool isFound (list<Flight> lst, string frm, string too)
{
    bool isfound = false;
    list<Flight>::iterator it=lst.begin();
    string start, endas;
    start = it->getFrom();
    it = lst.end();
    it--;
    endas = it->getTo();

    if (start == frm && endas == too)
    {
        isfound = true;
    }

    return isfound;
}

bool areVisited(string frm)
{
    int counter = 0;
    list<list<Flight> >::iterator iter;
    for (list<list<Flight> >::iterator itr=AdjList.begin(); itr != AdjList.end(); ++itr)
    {
        for (list<Flight>::iterator it=itr->begin(); it != itr->end(); ++it)
        {
            if (it->getFrom() == frm)
            {
                iter = itr;
                if(it->isVisited()) counter++;
            }

        }
    }
    if (counter == iter->size()) return true;
    else return false;
}

CompleteFlight getBest()
{
    list<CompleteFlight>::iterator itr=CompleteFlightLists.begin();
    CompleteFlight mini = *itr;
    for (list<CompleteFlight>::iterator itr=CompleteFlightLists.begin(); itr != CompleteFlightLists.end(); itr++)
    {
        if (itr->getSize() < mini.getSize())
        {
            mini = *itr;
        }
    }
    return mini;
}

bool isLast(string frm, string tu)
{
    bool last = true;
    for(list<list<Flight> >::iterator iter = AdjList.begin(); iter != AdjList.end(); iter++)
    {
        for (list<Flight>::iterator itrr=iter->begin(); itrr != iter->end(); itrr++)
        {
            if (itrr->getFrom() == tu && !itrr->isVisited())
            {
                last = false;
            }

        }
    }
    return last;
}

void getList()
{
    for (list<list<Flight> >::iterator itr=AdjList.begin(); itr != AdjList.end(); ++itr)
    {
        for (list<Flight>::iterator it=itr->begin(); it != itr->end(); ++it)
        {
            cout << it->getFrom()<< " " << it->getTo() << " " << it->getDeptime() << " " << it->getLength() << endl;
        }
        cout << endl;
    }
}


void setAdjLists()
{
    bool found = false;
    for (list<Flight>::iterator iter1 = listas.begin(); iter1 != listas.end(); iter1++)
    {
        for (list<list<Flight> >::iterator iter2 = AdjList.begin(); iter2 != AdjList.end(); iter2++)
        {
            if (iter1->getFrom() == iter2->front().getFrom() && iter1->getTo() != iter2->front().getTo())
            {
                iter2->push_front(*iter1);
                found = true;
            }
        }
        if (!found)
        {
            AdjNodeList.push_front(*iter1);
            AdjList.push_front(AdjNodeList);
            AdjNodeList.clear();
        }
        else if (found)
        {
            found = false;
        }
    }
}

double toDouble (string str)
{
    stringstream ss;
    double dbl;
    ss << str;
    ss >> dbl;
    return dbl;
}
