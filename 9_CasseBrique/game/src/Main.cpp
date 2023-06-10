#include "raylib.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;
// Initialisation des variables

const int LARGEUR_ECRAN = 1280;
const int HAUTEUR_ECRAN = 720;
const int BALLE_VITESSE = 6;
const int BALLE_TAILLE = 32;
const int RAQUETTE_LARGEUR = 128;
const int RAQUETTE_HAUTEUR = 32;
const int RAQUETTE_POSITIONY = HAUTEUR_ECRAN - RAQUETTE_HAUTEUR - RAQUETTE_HAUTEUR / 2;
const int RAQUETTE_POSITIONX = LARGEUR_ECRAN / 2 - RAQUETTE_LARGEUR / 2;
const int RAQUETTE_VITESSE = 8;
const int BRIQUE_LARGEUR = 128;
const int BRIQUE_HAUTEUR = 40;
const int BRIQUES_LIGNES = 5;
const int BRIQUES_COLONNES = 10;
const int BRIQUE_SEPARATEUR = 2;
const int POSITION_BALLEX = RAQUETTE_POSITIONX;
const int POSITION_BALLEY = RAQUETTE_POSITIONY - BALLE_TAILLE * 2;
const int VIE_DEPART = 3;
const int totalBrique = BRIQUES_COLONNES * BRIQUES_LIGNES;

Rectangle balle{ POSITION_BALLEX, POSITION_BALLEY, BALLE_TAILLE,BALLE_TAILLE };
int vitesseBalleX = BALLE_VITESSE;
int vitesseBalleY = -BALLE_VITESSE;

Rectangle raquette{ RAQUETTE_POSITIONX,RAQUETTE_POSITIONY,RAQUETTE_LARGEUR,RAQUETTE_HAUTEUR };

int nbBriques = totalBrique;
int vie = VIE_DEPART;
int etatJeu = 0;
string gameOver;
string relancer = "Appuyez sur R pour relancer une partie";
bool resultatVisible = false;

Sound sonCollisionMur;
Sound sonDegatBrique;
Sound sonPerteVie;
Sound sonCollisionRaquette;


// Structure de la brique

/// <summary>
/// x / y / width / height
/// </summary>
struct Brique 
{
    Rectangle rect;
    bool visible;
    Color color;
};

/// <summary>
/// x / y / width / height
/// </summary>
Brique brique{ { 0,0,BRIQUE_LARGEUR,BRIQUE_HAUTEUR }, true };    //Predfab brique
vector <Brique> briques{};

// Initialisation des fonctions

void load();
void unload();
void update();
void draw();
bool collisionRaquetteBalle(Rectangle raquette, Rectangle balle);
void rebondSurRaquette();
void relancerBalle();
void relancerPartie();

int main()
{
    srand(time(nullptr));

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
    if (etatJeu == 0) 
    {
        // Donner une vitesse initiale à la balle

        balle.x += vitesseBalleX;
        balle.y += vitesseBalleY;

        // Collision de la balle avec l'environement

        if (balle.y < 0)
        {
            vitesseBalleY = -vitesseBalleY;
            PlaySound(sonCollisionMur);
        }
        if (balle.y > HAUTEUR_ECRAN - BALLE_TAILLE)
        {
            --vie;
            PlaySound(sonPerteVie);
            relancerBalle();
            // Changement de l'état du jeu si le joueur n'a plus de vie
            if (vie <= 0)
            {
                etatJeu = 1;
            }
        }
        if (balle.x < 0)
        {
            vitesseBalleX = -vitesseBalleX;
            balle.x = 0;
            PlaySound(sonCollisionMur);
        }
        if (balle.x > LARGEUR_ECRAN - BALLE_TAILLE)
        {
            vitesseBalleX = -vitesseBalleX;
            balle.x = LARGEUR_ECRAN - BALLE_TAILLE;
            PlaySound(sonCollisionMur);
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

        // Gestion de la collision entre la balle est les briques

        for (int i = 0; i < briques.size(); i++)
        {
            int xMinBalle = balle.x;
            int xMaxBalle = balle.x + balle.width;
            int yMinBalle = balle.y;
            int yMaxBalle = balle.y + balle.height;
            int xMinBrique = briques[i].rect.x;
            int xMaxBrique = briques[i].rect.x + briques[i].rect.width;
            int yMinBrique = briques[i].rect.y;
            int yMaxBrique = briques[i].rect.y + briques[i].rect.height;
            if (!(xMinBalle > xMaxBrique || xMaxBalle < xMinBrique || yMinBalle > yMaxBrique || yMaxBalle < yMinBrique))
            {
                if (!briques[i].visible)
                {
                    continue;
                }
                //balle.y = yMinBrique + BALLE_TAILLE + 10;
                vitesseBalleY = -vitesseBalleY;
                briques[i].visible = false;
                nbBriques--;
                PlaySound(sonDegatBrique);
            }
        }
        if (nbBriques <= 0) 
        {
            etatJeu = 2;
        }
    }
    // Vérifier si le joueur à perdu

    if (etatJeu == 1) 
    {
        gameOver = "Game Over";
        resultatVisible = true;
        relancerPartie();             
    }

    // Verifier si le joeur à gagner
    if (etatJeu == 2)
    {
        gameOver = "Victoire !";
        resultatVisible = true;
        relancerPartie();
    }
}

void draw() 
{
    BeginDrawing();
    ClearBackground(BLACK);

    // On dessine la balle et la raquette du joueur

    DrawRectangle(balle.x, balle.y, balle.width, balle.height, WHITE);
    DrawRectangle(raquette.x, raquette.y, raquette.width, raquette.height, WHITE);

    // On dessine les briques

    for (Brique& brique : briques) 
    {
        if (brique.visible) 
        {
            DrawRectangle(brique.rect.x, brique.rect.y, brique.rect.width, brique.rect.height, brique.color);
        }      
    }

    // On dessine le nombre de vies du joueur

    DrawText(to_string(vie).c_str(), LARGEUR_ECRAN / 8, HAUTEUR_ECRAN / 1.5, 40, RED);
   
    // On dessine le game over et la phrase pour relancer la partie quand le jeu est fini
    if (resultatVisible) 
    {
        DrawText(gameOver.c_str(), LARGEUR_ECRAN / 3, HAUTEUR_ECRAN / 2, 50, WHITE);
        DrawText(relancer.c_str(), LARGEUR_ECRAN / 3, HAUTEUR_ECRAN / 2 + 80, 40, BLUE);
    }
    EndDrawing();
}

void load() 
{
    InitWindow(LARGEUR_ECRAN, HAUTEUR_ECRAN, "CasseBrique");
    SetTargetFPS(60);
    InitAudioDevice();

    sonCollisionMur = LoadSound("sonCollisionMur.wav");
    sonDegatBrique = LoadSound("sonDegatBrique.wav");
    sonPerteVie = LoadSound("sonPerteVie.wav");
    sonCollisionRaquette = LoadSound("sonCollisionRaquette.wav");

    // On ajoute les briques à casser

    for (int ligne = 0; ligne < BRIQUES_LIGNES; ligne++)
    {
        for (int colonne = 0; colonne < BRIQUES_COLONNES; colonne++)
        {
            // On créer une couleur différente pour chaque brique
            char randomColorR = rand() % 255 + 1;
            char randomColorG = rand() % 255 + 1;
            char randomColorB = rand() % 255 + 1;
            Color color = { randomColorR,randomColorG,randomColorB,255 };
            Brique br = { { (const float)BRIQUE_LARGEUR * colonne, (const float)BRIQUE_HAUTEUR * ligne, brique.rect.width - BRIQUE_SEPARATEUR, brique.rect.height - BRIQUE_SEPARATEUR},true,color };
            briques.push_back(br);           
        }
    }
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

    return(!(xMinBalle> xMaxRaquette || yMinBalle > yMaxRaquette|| xMaxBalle < xMinRaquette ||  yMaxBalle < yMinRaquette));
}

// Fonction pour renvoyer la balle dans la direction opposée à la collision

void rebondSurRaquette()
{
    vitesseBalleY = -vitesseBalleY;
    PlaySound(sonCollisionRaquette);
}
void relancerBalle()
{
    balle.x = POSITION_BALLEX;
    balle.y = POSITION_BALLEY;
    vitesseBalleY = -vitesseBalleY;
}

void relancerPartie() 
{
    if (IsKeyPressed(KEY_R))
    {
        etatJeu = 0;
        resultatVisible = false;

        vie = VIE_DEPART;
        balle.x = POSITION_BALLEX;
        balle.y = POSITION_BALLEY;

        nbBriques = totalBrique;

        //  On rend visible les briques à nouveau
        for (Brique& brique : briques)
        {
            brique.visible = true;
        }
    }   
}