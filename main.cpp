#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include<stack>
#include <map>
#include<bits/stdc++.h>
#include <iterator>

using namespace std;

class Station
{
private:




    /// Données spécifiques du sommet
    int m_id;// Indice numérique pour l'affichage
    int m_couleur=0;//0=blanc 1=gris 2=noir
    ///l element i de cette map contient l id de la station voisine i + la distance entre les deux stations
    ///cette atribut n'est pas indispensable,on trouve les voisins d'une station au bout des routes qui partent de cette station
    std::map<Station*,int> m_distances_stations_voisine;
public:

    Station(int id)
    {
        m_id=id;

    }///Si aucune route ne part de la station, on ne pourra atteindre les autres stations
bool on_ne_peut_aller_nulle_part_depuis_cette_station()
{
    if(m_distances_stations_voisine.empty())
        return true;
    else return false;
}

    int get_id()
    {
        return m_id;
    }



    int get_couleur()
    {
        return m_couleur;
    }
    void set_couleur(int col)
    {
        m_couleur=col;
    }
    void ajouter_station_voisine(Station* voisin,int dist)
    {
        m_distances_stations_voisine.insert(pair<Station*,int>(voisin,dist));
    }
    void afficher_voisins()
    {
        cout<<"la station "<<m_id<<" a comme voisins :";
        for (map<Station*,int>::iterator it=m_distances_stations_voisine.begin(); it!=m_distances_stations_voisine.end(); ++it)
        {
            cout <<"la station "<< it->first->get_id() << " (" << it->second << "metre)"<<" ,";

        }
        cout<<endl;

    }

    int get_distance_par_rapport_a_voisin(Station* voisin)
    {
       return m_distances_stations_voisine.find(voisin)->second;
    }
    int get_nb_voisins()
    {
        return m_distances_stations_voisine.size();
    }


};
class Route
{
private:              ///les routes sont a sens unique, une station de depart et une destination
    int m_longueur;
    Station* m_depart;
    Station* m_destination;

public:
    Route(int l,Station* depart,Station* dest)
    {
        m_longueur=l;
        m_depart=depart;
        m_destination=dest;


    }
    Station* get_depart()
    {
        return m_depart;
    }
    Station * get_dest()
    {
        return m_destination;
    }
    int get_longueur()
    {
        return m_longueur;
    }
};


class Reseau
{
private:
    /// Le réseau est constitué d'une collection de stations et de routes
    std::vector<Station*> m_stations;
    std::vector<Route*> m_routes;




    int m_ordre;
    int m_taille;


public:

    Reseau(std::string nomFichier)
    {
        std::ifstream ifs{nomFichier};
        if (!ifs)
            throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );


        ifs >> m_ordre;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture ordre du graphe");

        int id=0;
        ///CREATION DES STATIONS AVEC AJOUT À LA LISTE DU GRAPHE_________________________________________
            for(int i=0;i<m_ordre;i++)
            {
                ifs >> id ;
                if (ifs.fail() )
                    throw std::runtime_error("Probleme lecture id d'une station");

                m_stations.push_back(new Station(id));
            }

        ifs >> m_taille;
        if ( ifs.fail() )
            throw std::runtime_error("Probleme lecture taille du graphe");



     int id_station_destination=0;
     int longueur=0;

///CREATION DES ROUTES ENTRE LES STATIONS________________________________________________
        for (int i=0; i<m_taille; ++i)
        {
            ifs >> id >> id_station_destination >> longueur;

            if ( ifs.fail() )
                throw std::runtime_error("Probleme lecture id d'une station");

              Route* nouvelle_route=new Route(longueur,this->get_station(id),this->get_station(id_station_destination));
              m_routes.push_back(nouvelle_route);
              this->get_station(id)->ajouter_station_voisine(this->get_station(id_station_destination),longueur);

        }


    }
     ~Reseau()
        {


        }
    void afficher()
    {

        std::cout<<"nb sommet "<<m_ordre<<std::endl;
        std::cout<<" adjacence:"<<std::endl;
        for(auto s:m_stations)
        {

           s->afficher_voisins();


        }


    }

///retrouver une station grâce à son Id

    Station* get_station(int id)
    {
        bool trouv=false;

        for(auto s:m_stations)
        {
            if(s->get_id()==id)
           {
               return s;
           }


        }
       if(!trouv)
            return nullptr;


    }



///ALGO DIJKSTRA___________________________________________________________________________________________________________________
void dijkstra(Station* station_so,Station *destinatation)
{
    if(!station_so->on_ne_peut_aller_nulle_part_depuis_cette_station())///Si aucune route ne part de la station So
                                                                      ///on pourra aller nulle part
    {


    int predecesseur[1000];


    for(int i=0; i<1000; i++)
        {
            predecesseur[i]=10000;
        }
        predecesseur[station_so->get_id()]=666;///on marque que la station s0 n' as pas de prédecesseur
    int distance_So_S;
    int distance_S_voisin_de_S;
    int distance_So_voisin_de_S;
    map<Station*,int> distances_par_rapport_a_so;
    priority_queue<pair<Station*,int>,vector<pair<Station*,int>>,greater<pair<Station*,int>>> pq;
    ///cette queue nous permet de connaitre la distance d'une station par rapport à la station So de départ

    for (auto elem:m_stations)///initialisation
    {
        if(elem!=station_so)///chaque station est à une distance de 1000m par rapport a So
        {
            distances_par_rapport_a_so.insert(make_pair(elem,1000));

        }
        else
        {
            distances_par_rapport_a_so.insert(make_pair(elem,0));///la station So est à une distance de 0 d'elle même


        }

    }

pq.push(make_pair(station_so,0));



    while(destinatation->get_couleur()!=1)
    {
        ///selection de la station la plus proche
Station* station_s=pq.top().first;
pq.pop();
station_s->set_couleur(1);
cout<<"sommet S:"<<station_s->get_id()<<" a une distance de "<<distances_par_rapport_a_so.find(station_s)->second<<"m du sommet initial"<<endl;
for(auto &une_route:m_routes)
{

    if(une_route->get_depart()==station_s)///chaque route partant de la station S nous mène vers ses voisins
    {
        Station* voisin_de_s=une_route->get_dest();

        distance_So_S=distances_par_rapport_a_so.find(station_s)->second;

        distance_So_voisin_de_S=distances_par_rapport_a_so.find(voisin_de_s)->second;

        distance_S_voisin_de_S=station_s->get_distance_par_rapport_a_voisin(voisin_de_s);

        if(distance_So_voisin_de_S > distance_So_S + distance_S_voisin_de_S)
        {
            if(distance_So_voisin_de_S==1000)
            distances_par_rapport_a_so.find(voisin_de_s)->second=0;

            distances_par_rapport_a_so.find(voisin_de_s)->second= distance_So_S + distance_S_voisin_de_S ;

            pq.push(make_pair(voisin_de_s,distance_So_S + distance_S_voisin_de_S));

            predecesseur[voisin_de_s->get_id()]=station_s->get_id();


        }


    }
cout<<endl;

}}
///AFFICHAGE_____________________________________________________________________________________________________________

int id_destination=destinatation->get_id();
cout <<id_destination;
do
{
    cout<<"=>"<<predecesseur[id_destination];
    id_destination=predecesseur[id_destination];
}while(predecesseur[id_destination]!=666);
cout<<endl;
cout<<"la plus courte distance entre ces sommets est de "<<distances_par_rapport_a_so.find(destinatation)->second<<"m"<<endl;

}
else
    cout<<"!!!!!!!!!!!!!!!on peut aller nulle part depuis cette station car aucune route n'en sort"<<endl;
}



///FIN ALGO___________________________________________________________________________________________________________________________________________






    int get_ordre()
    {
        return m_ordre;
    }





void menu()
{
    int choix;
    std::cout<<std::endl;

       cout<<"1:parcourir le plus court chemin entre deux stations"<<endl<<"2:quitte la bas"<<endl;

        std::cin>>choix;
        std::cout<<std::endl;
        switch (choix)
        {
        case 1:
            int id_station_depart;
            int id_station_final;
        std::cout<<"choisissez une station de depart parmi les "<<m_ordre<<" stations"<<std::endl;
        std::cin>>id_station_depart;
        std::cout<<"choisissez une station d'arriver parmi les "<<m_ordre-1<<" stations restantes"<<std::endl;
        std::cin>>id_station_final;
        this->dijkstra(this->get_station(id_station_depart),this->get_station(id_station_final));


        std::cout<<std::endl;

            this->menu();
            break;


        case 2:
            std::cout<<"a la bonne journee";
            break;
        }


}





};

int main()
{
    try
    {
        Reseau reseau("reseau.txt");
        reseau.afficher();
reseau.menu();







    }
    catch (const std::exception& e)
    {
        std::cerr << "Attention : " << e.what() << std::endl;
    }

    return 0;
}

