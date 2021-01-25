#include "Chemin.hpp"

bool Chemin::readPath(std::ifstream &in){
  char k; // utiliser pour récupérer le caractère séparateur

  // récupération éventuelle des coordonnées du pixel
  in >> x >> k >> y >> k;
  if(in.eof()) return false;

  std::cout << "==============================================================================" << std::endl;
  std::cout << "Origin(" << x << ", " << y << ")" << std::endl;

  // récupération des noeuds du chemin
  std::string ligne;
  getline(in, ligne);
  std::istringstream iss(ligne);
  
  Point s;
  Color currentL;
  unsigned counter = 0;
  iss >> s.x;// on tente un premier sommet
  while(iss){// il y a un sommet à lire
    
    // récupération des coordonées
    iss >> k >> s.y >> k >> s.z >> k;
    sommets.push_back(s);
    std::cout << "Point(" << s.x << ", " << s.y << ", " << s.z << ")";
    
    // récupération des luminances
    // La première luminance correspond à la somme, on la stocke donc en information globale
    if (counter == 0) {
       iss >> l.r >> k >> l.g >> k >> l.b >> k;  
       std::cout << " => Sum of L(" << l.r << ", " << l.g << ", " << l.b << ")" << std::endl;
    }
    else{
       iss >> currentL.r >> k >> currentL.g >> k >> currentL.b >> k;  
       luminances.push_back(currentL);
       std::cout << " => L(" << currentL.r << ", " << currentL.g << ", " << currentL.b << ")" << std::endl;
    }

    iss >> s.x;// on tente le sommet suivant
    counter++;
  }


  return true;
}




void Chemin::draw(Color c){

  glBegin(GL_LINE_STRIP);
  
  for(int i=0; i< sommets.size(); i++){
    if(i==0) glColor3f(0.0, 0.0, 1.0);
    else glColor3f(c.r, c.g, c.b);
    glVertex3f(sommets[i].x, sommets[i].y, sommets[i].z);
  }

  glEnd();
}



std::ostream& operator<<(std::ostream& out, const Chemin& path){
  out << path.x << "-" << path.y << " (";
  out << path.l.r << "-" << path.l.g <<  "-" << path.l.b << ") ";
  for(int i=0; i<path.sommets.size(); i++){
    out << "[" << path.sommets[i].x;
    out << "," << path.sommets[i].y;
    out << "," << path.sommets[i].z;
    out << "] ";
  }

  return out;
}
