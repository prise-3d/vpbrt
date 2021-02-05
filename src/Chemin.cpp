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

  std::cout << "==============================================================" << std::endl;
  std::cout << "Display new path of pixel (" << x << ", " << y << ")" << std::endl;
  std::cout << "==============================================================" << std::endl;
  std::cout << "Sum of luminance is: L(" << l.r << ", " << l.g << ", " << l.b << ")" << std::endl;
  std::cout << "Number of bounds is: " << sommets.size() - 1 << std::endl;

  glBegin(GL_LINE_STRIP);
  
  for(int i=0; i< sommets.size(); i++){

    // first point (camera to first hit position)
    if(i==0) { 
      glColor3f(0.0, 1.0, 0.0);
        std::cout << "Camera position is: (" << sommets[i].x << ", " << sommets[i].y << ", " << sommets[i].z << ")" << std::endl;
        std::cout << "--------------------------------------------------------------" << std::endl;

    }
    else {

        std::cout << "Bound n°" << (i) << ": x(" << sommets[i].x << ", " << sommets[i].y << ", " << sommets[i].z << ")" << std::endl;
        std::cout << "   - L(" << luminances[i - 1].r << ", " << luminances[i - 1].g << ", " << luminances[i - 1].b << ")" << std::endl;

      // ray color depending of luminance contribution
      if (luminances[i - 1].r < limit && luminances[i - 1].g < limit && luminances[i - 1].b < limit) {
        glColor3f(0.0, 0.0, 1.0); 
      }
      else {
        glColor3f(1.0, 0.0, 0.0); 
      }
    }
    glLineWidth(10.0);
    glVertex3f(sommets[i].x, sommets[i].y, sommets[i].z);
  }

  std::cout << "==============================================================" << std::endl;

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
