#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Geometry/Scene.hpp"
#include "interactions.hpp"
#include "Camera.hpp"

extern Scene *curScene;
extern Camera maCamera;


/** 
 * Fonction permettant de dessiner le repère du monde sous 
 * forme d'une croix 3D.
 * @param dim la taille de la ligne représentant un demi-axe.
 */

static void repere(float dim)
{

  glBegin(GL_LINES);
 
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(-dim, 0.0, 0.0);
  glVertex3f( dim, 0.0, 0.0);
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(0.0,-dim, 0.0);
  glVertex3f(0.0, dim, 0.0);
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.0, -dim);
  glVertex3f(0.0, 0.0,  dim);

  glEnd();
}

/**
 * FOnction permettant de recalculer la zone d'affichage
 * dans la fenêtre lors d'une modification de sa taille
 * par l'utilisateur. La zone est calculé de manière à conserver
 * le rapport d'aspect initial défini dans le fichier pbrt.
 */
void retailler(GLsizei largeur, GLsizei hauteur){
  int xmin, ymin;


  // calcul du rapport d'aspect de la fenêtre modifiée
  float ratio=(float)largeur/(float)hauteur;

  // calcul de la zone d'affichage pour conserver le rapport
  // d'aspect défini initialement
  if(maCamera.ratio > ratio){// on prend toute la largeur
    xmin = 0; 
    ymin = (hauteur-largeur/maCamera.ratio)/2.0;
    // maj des dimensions de l'image pour la perspective
    maCamera.largeur = largeur; maCamera.hauteur = hauteur-2*ymin;
  }else{// on prend toute la hauteur
    ymin =0; 
    xmin = (largeur-hauteur*maCamera.ratio)/2.0;
    // maj des dimensions de l'image pour la perspective
    maCamera.largeur = largeur-2*xmin; maCamera.hauteur = hauteur;
  }
  // On définit la nouvelle zone d'affichage
  glViewport(xmin, ymin, largeur-2*xmin, hauteur-2*ymin);



  glutPostRedisplay();

}


/**
 * Fonction utilisée pour afficher le monde 3D à visualiser. Cette fonction
 * sera appelée à chaque fois qu'un "rafraichissement" de la fenêtre
 * d'affichage sera nécessaire.
 */

void dessiner(void){
    /* effacer l'ecran */
  glClearColor(0.0, 0.0, 0.0, 1.0);

  /* raz de la fenetre avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(maCamera.fov, (float)maCamera.largeur/(float)maCamera.hauteur,
		 0.1, 1000000.0 );
  //  glViewport(0,0, maCamera.largeur, maCamera.hauteur);

  
  float *o = maCamera.o;
  float *v = maCamera.v;
  float *up = maCamera.up;
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glScalef(-1.0, 1.0, 1.0);// prise en compte du repère main gauche
  gluLookAt(o[0], o[1], o[2], v[0], v[1], v[2], up[0], up[1], up[2]);

    /* dessin des objets */
  repere(2.0);

  glColor3f(1.0, 1.0, 1.0);

  if(curScene){
    if(curScene->isInsideBB(Point3f(o[0], o[1], o[2])))
      glColor3f(1.0, 1.0, 1.0);
    else
      glColor3f(1.0, 0.0, 0.0);
    curScene->draw();
  }

			    
  
  glFlush();

  glutSwapBuffers();
  return;

}

