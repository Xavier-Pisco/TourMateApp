#include "User.h"

void User::findDestino(string str) {
    cout<<str<<"\n";
    //find Vertex com o nome str

}

void User::findOrigem(string str) {
    cout<<str<<"\n";
    //find Vertex com o nome str
}


User::User() {
    this->origin=NULL;
    this->destiny=NULL;
    this->preferenceList=NULL;
    this->poiList=NULL;
}

void User::setDisponibilidade(float time) {
    this->disp=time;
}
