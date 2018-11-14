#include <stdio.h>
#include <stdlib.h>
#define NB_SYSTEME 4


typedef struct coord coord;
struct coord{
    float x;
    float y;
    float z;
};
typedef void (*systeme_dynamique_adresse)(coord, coord*, float, float*);

void parametres_generaux(coord *pos, float *dt, float *t_max, float parametres_systeme[], systeme_dynamique_adresse *systeme_dynamique);
void parametres_lorentz(float parametres_systeme[]);
void parametres_chute_libre(float parametres_systeme[]);
void parametres_satellite(float parametres_systeme[]);
void parametres_polonaise_inversee(float parametres_systeme[]);

void remplir_trajectoire(coord trajectoire[], float temps[], int iterations, float dt, float parametres_systeme[], systeme_dynamique_adresse systeme_dynamique);
void systeme_lorentz(coord instant_t, coord *instant_t_dt, float dt, float parametres_systeme[]);
void systeme_chute_libre(coord instant_t, coord *instant_t_dt, float dt, float parametres_systeme[]);
void systeme_satellite(coord instant_t, coord *instant_t_dt, float dt, float parametres_systeme[]);
void systeme_polonaise_inversee(coord instant_t, coord *instant_t_dt, float dt, float parametres_systeme[]);

void plot_trajectoire(coord trajectoire[], float temps[], int iterations);


void parametres_generaux(coord *pos, float *dt, float *t_max, float parametres_systeme[], systeme_dynamique_adresse *systeme_dynamique) {

    int i;

    printf("Entrez la position de départ :\nx = ");
    scanf("%f", &((*pos).x));
    printf("y = ");
    scanf("%f", &((*pos).y));
    printf("z = ");
    scanf("%f", &((*pos).z));

    printf("Entrez l'intervalle de temps : dt = ");
    scanf("%f", dt);

    printf("Entrez le temps d'arrêt : Tmax = ");
    scanf("%f", t_max);

    do{
        printf("Choisisez le système dynamique :\n1 : Lorentz\n2 : Chute libre\n3 : Satellite\n4 : Notation Polonaise Inversée\nEntrez le numéro associé : ");
        scanf("%d", &i);
    }while((i<1) || (i>NB_SYSTEME));

    if(i==1){
        *systeme_dynamique = systeme_lorentz;
        parametres_lorentz(parametres_systeme);
    }
//    if(i==2){
//        *systeme_dynamique = systeme_chute_libre;
//        parametres_chute_libre(parametres_systeme);
//    }
//    if(i==3){
//        *systeme_dynamique = systeme_satellite;
//        parametres_satellite(parametres_systeme);
//    }
//    if(i==4){
//        *systeme_dynamique = systeme_polonaise_inversee;
//        parametres_polonaise_inversee(parametres_systeme);
//    }


}

void parametres_lorentz(float parametres_systeme[]){
    printf("Entrez sigma : ");
    scanf("%f", &(parametres_systeme[0]));
    printf("Entrez rho : ");
    scanf("%f", &(parametres_systeme[1]));
    printf("Entrez beta : ");
    scanf("%f", &(parametres_systeme[2]));
}

void remplir_trajectoire(coord trajectoire[], float temps[], int iterations, float dt, float parametres_systeme[], systeme_dynamique_adresse systeme_dynamique){
    int i;

    for(i=1; i<=iterations; i++){
        systeme_dynamique(trajectoire[i-1], &(trajectoire[i]), dt, parametres_systeme);
        temps[i] = temps[i-1] + dt;
    }
}

void systeme_lorentz(coord instant_t, coord *instant_t_dt, float dt, float parametres_systeme[]){
    (*instant_t_dt).x = ((parametres_systeme[0] * (instant_t.y - instant_t.x)) * dt) + instant_t.x;
    (*instant_t_dt).y = ((instant_t.x * (parametres_systeme[1] - instant_t.z) - instant_t.y) * dt) + instant_t.y;
    (*instant_t_dt).z = ((instant_t.x * instant_t.y - parametres_systeme[2] * instant_t.z) * dt) + instant_t.z;
}

void plot_trajectoire(coord trajectoire[], float temps[], int iterations){
    int i;
    FILE *p = fopen("resultats.dat", "w");

    for(i=0; i<iterations; i++){
        fprintf(p, "%f %f %f %f\n", temps[i], trajectoire[i].x, trajectoire[i].y, trajectoire[i].z);
    }
    pclose(p);
}

int main(){
    coord pos;
    float dt, t_max;
    float parametres_systeme[100];
    systeme_dynamique_adresse systeme_dynamique;
    int iterations;
    parametres_generaux(&pos, &dt, &t_max, parametres_systeme, &systeme_dynamique);
    iterations = t_max/dt;
    coord trajectoire[iterations];
    float temps[iterations];
    trajectoire[0] = pos;
    temps[0] = 0.0;
    remplir_trajectoire(trajectoire, temps, iterations, dt, parametres_systeme, systeme_dynamique);
    plot_trajectoire(trajectoire, temps, iterations);
    return 0;
}
