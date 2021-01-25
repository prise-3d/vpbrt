#include "Chemin.hpp"

bool Chemin::readPath(std::ifstream &in){
  char k;

  // récupération éventuelle des coordonnées du pixel
  in >> x >> k >> y >> k;
  if(in.eof()) return false;

  // récupération des luminances
  in >> r >> k >> g >> k >> b >> k;

  // récupération des noeuds du chemin
  std::string ligne;
  getline(in, ligne);
  std::istringstream iss(ligne);
  Point s;
  iss >> s.x;// on tente un premier sommet
  while(iss){// il y a un sommet à lire
    iss >> k >> s.y >> k >> s.z >> k;
    sommets.push_back(s);
    //std::cout << s.x << " " << s.y << " " << s.z << std::endl;
    iss >> s.x;// on tente le sommet suivant
  }


  return true;
}




void Chemin::draw(float r, float g, float b){

  glBegin(GL_LINE_STRIP);
  
  for(int i=0; i<sommets.size(); i++){
    if(i==0) glColor3f(0.0, 0.0, 1.0);
    else glColor3f(r,g,b);
    glVertex3f(sommets[i].x, sommets[i].y, sommets[i].z);
  }

  glEnd();
}



std::ostream& operator<<(std::ostream& out, const Chemin& path){
  out << path.x << "-" << path.y << " (";
  out << path.r << "-" << path.g <<  "-" << path.b << ") ";
  for(int i=0; i<path.sommets.size(); i++){
    out << "[" << path.sommets[i].x;
    out << "," << path.sommets[i].y;
    out << "," << path.sommets[i].z;
    out << "] ";
  }

  return out;
}
