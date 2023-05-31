#include "raylib.h"
#include <iostream>
#include <vector>

using namespace std;
// Initialisation des variables

const int LARGEUR_ECRAN = 1280;
const int HAUTEUR_ECRAN = 720;
const int BALLE_VITESSE = 4;
const int BALLE_TAILLE = 32;
const int RAQUETTE_LARGEUR = 128;
const int RAQUETTE_HAUTEUR = 32;
const int RAQUETTE_POSITIONY = HAUTEUR_ECRAN - RAQUETTE_HAUTEUR - RAQUETTE_HAUTEUR / 2;
const int RAQUETTE_POSITIONX = LARGEUR_ECRAN / 2 - RAQUETTE_LARGEUR / 2;
const int RAQUETTE_VITESSE = 5;
const int BRIQUE_LARGEUR = 128;
const int BRIQUE_HAUTEUR = 40;
const int BRIQUES_LIGNES = 5;
const int BRIQUES_COLONNES = 10;
const int BRIQUE_SEPARATEUR = 2;

Rectangle balle{ 20, 50, BALLE_TAILLE,BALLE_TAILLE };
int vitesseBalleX = BALLE_VITESSE;
int vitesseBalleY = -BALLE_VITESSE;

Rectangle raquette{ RAQUETTE_POSITIONX,RAQUETTE_POSITIONY,RAQUETTE_LARGEUR,RAQUETTE_HAUTEUR };

// Structure de la brique

struct Brique 
{
    Rectangle rect;
};

Rectangle rect{ 0,0,BRIQUE_LARGEUR,BRIQUE_HAUTEUR };
Brique brique{ rect };


// Initialisation des fonctions

void load();
void unload();
void update();
void draw();
bool collisionRaquetteBalle(Rectangle raquette, Rectangle balle);
void rebondSurRaquette();

int main()
{
    load();
    while (WindowShouldClose) 
    {
        update();
        draw();
    }
    unload();
    return 0;
}

void update() 
{
    // Donner une vitesse initiale à la balle
    
    balle.x += vitesseBalleX;
    balle.y += vitesseBalleY;

    // Collision de la balle avec l'environement

    if (balle.y < 0) 
    {
        vitesseBalleY = -vitesseBalleY;
    }
    if (balle.y > HAUTEUR_ECRAN - BALLE_TAILLE) 
    {
        vitesseBalleY = -vitesseBalleY;
    }
    if (balle.x < 0) 
    {
        vitesseBalleX = -vitesseBalleX;
    }
    if (balle.x > LARGEUR_ECRAN - BALLE_TAILLE) 
    {
        vitesseBalleX = -vitesseBalleX;
    }

    // Déplacement de la raquette du joueur

    if (IsKeyDown(KEY_A)) 
    {
        raquette.x -= RAQUETTE_VITESSE;
    }
    if (IsKeyDown(KEY_D)) 
    {
        raquette.x += RAQUETTE_VITESSE;
    }
    
    // Gestion des collisions entre le joueur et le bords de l'écran

    if (raquette.x < 0) 
    {
        raquette.x += RAQUETTE_VITESSE;
    }

    if (raquette.x > LARGEUR_ECRAN - RAQUETTE_LARGEUR) 
    {
        raquette.x -= RAQUETTE_VITESSE;
    }

    // Gestion de la collision entre le joueur et la balle
  
    if (collisionRaquetteBalle(raquette, balle)) 
    {
        rebondSurRaquette();
    }

}

void draw() 
{
    BeginDrawing();
    ClearBackground(BLACK);

    // On dessine la balle et la raquette du joueur

    DrawRectangle(balle.x, balle.y, balle.width, balle.height, WHITE);
    DrawRectangle(raquette.x, raquette.y, raquette.width, raquette.height, WHITE);

    // On dessine les briques à casser
    vector <Brique> briques{};
    for (int ligne = 0; ligne < BRIQUES_LIGNES; ligne++) 
    {
        for (int colonne = 0; colonne < BRIQUES_COLONNES; colonne++) 
        {
            brique = { rect.x = BRIQUE_LARGEUR * colonne, rect.y = BRIQUE_HAUTEUR * ligne, rect.width - BRIQUE_SEPARATEUR, rect.height - BRIQUE_SEPARATEUR };
            briques.push_back(brique);
        }             
    }
    for (Brique brique : briques) 
    {
        DrawRectangle(brique.rect.x, brique.rect.y, brique.rect.width, brique.rect.height, WHITE);
    }
   
    EndDrawing();
}

void load() 
{
    InitWindow(LARGEUR_ECRAN, HAUTEUR_ECRAN, "CasseBrique");
    SetTargetFPS(60);
}

void unload() 
{
    CloseWindow();
}

// Fonction pour verifier s'il y a une collision entre la raquette et la balle

bool collisionRaquetteBalle(Rectangle raquette, Rectangle balle) 
{
    int xMinRaquette = raquette.x;
    int xMaxRaquette = raquette.x + raquette.width;
    int yMinRaquette = raquette.y;
    int yMaxRaquette = raquette.y + raquette.height;
    int xMinBalle = balle.x;
    int xMaxBalle = balle.x + raquette.width;
    int yMinBalle = balle.y;
    int yMaxBalle = balle.y + balle.height;

    return(!(xMinRaquette > xMaxBalle || xMaxRaquette < xMinBalle || yMinRaquette > yMaxBalle || yMaxRaquette < yMinBalle));
}

// Fonction pour renvoyer la balle dans la direction opposée à la collision

void rebondSurRaquette() 
{
    vitesseBalleY = -vitesseBalleY;
}