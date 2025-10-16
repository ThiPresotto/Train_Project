#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define WEIGHT 2*474 //474 = largura das imagens Wagons
#define HEIGHT 2*265 //265 = altura das imagens Wagons

typedef struct wagon{
    bool aceso;
    struct wagon* right;
    struct wagon* left;
}wagon;

wagon* cria_node(){
    wagon* node;
    node = (wagon*)malloc(sizeof(wagon));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    node->left = NULL;
    node->right= NULL;
    node->aceso = (rand() % 2 == 0) ? false : true;
    return node;
}

void criar_trem(int n,wagon** head){
    wagon* newnode;
    wagon* firstw;
    wagon* tail;
    firstw = (wagon*)malloc(sizeof(wagon));
    if (firstw == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    firstw->left = firstw;
    firstw->right = firstw;
    firstw->aceso = (rand() % 2 == 0) ? false : true;
    tail = firstw;
    for(int i = 0;i<n-1;i++){
        newnode = cria_node();
        tail->right = newnode;
        newnode->left = tail;
        newnode->right = firstw;
        firstw->left = newnode;
        if (rand()%2 == 0){
            newnode->aceso = false;
        }    
        else{
            newnode->aceso = true;
        }
        tail = newnode;
    }
    *head = firstw;
    return;
}

void delete_train(wagon* head,int n_vagao){
    wagon *current = head;
    wagon *next;

    while(n_vagao > 0){
        next = current->right;
        free(current);
        current = next;
        n_vagao -= 1;
    }
}

void transicao(wagon* head,SDL_Renderer* ren, int direcao){
    SDL_Texture* current_wagon;
    SDL_Texture* next_wagon;
    SDL_Rect current_back = {0,0,WEIGHT,HEIGHT};
    SDL_Rect next_back;
    SDL_Rect trans;
    if (head->aceso == true){
        current_wagon = IMG_LoadTexture(ren,"../assets/Wagon_Day.png");
    }
    else{
        current_wagon = IMG_LoadTexture(ren,"../assets/Wagon_Night.png");
    }
    if (direcao == 0){
        if (head->left->aceso == true){
            next_wagon = IMG_LoadTexture(ren,"../assets/Wagon_Day.png");
        }
        else{
            next_wagon = IMG_LoadTexture(ren,"../assets/Wagon_Night.png");
        }
        next_back = (SDL_Rect) {-1052,0,WEIGHT,HEIGHT};
        trans = (SDL_Rect) {-104,0,104,HEIGHT};
        head = head->left;
    }
    else{
        if (head->right->aceso == true){
            next_wagon = IMG_LoadTexture(ren,"../assets/Wagon_Day.png");
        }
        else{
            next_wagon = IMG_LoadTexture(ren,"../assets/Wagon_Night.png");
        }
        next_back = (SDL_Rect){1052,0,WEIGHT,HEIGHT};
        trans = (SDL_Rect) {948,0,104,HEIGHT};
        head = head->right;

    }
    for(int i = 0;i<2000;i+=10){
        SDL_RenderCopy(ren,current_wagon,NULL,&current_back);
        SDL_RenderCopy(ren,next_wagon,NULL,&next_back);
        SDL_RenderFillRect(ren,&trans);
        SDL_RenderPresent(ren);
        if(direcao = 0){
            current_back.x +=10;
            trans.x += 10;
            next_back.x += 10;
        }
        else{
            current_back.x -=10;
            trans.x -= 10;
            next_back.x -= 10;
        }
        SDL_Delay(10);
    }

}

int main(int argc, char* argv){

    /*INICIALIZAÇÃO*/

    srand(time(0));
    int n_vagao = rand() % (6 + 1 - 3) + 3;
    wagon* head;
    criar_trem(n_vagao,&head);

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    SDL_Window* win =  SDL_CreateWindow("Train Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WEIGHT,HEIGHT,SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win,-1,0);
    SDL_Texture* night_wagon = IMG_LoadTexture(ren, "../assets/Wagon_Night.png");
    assert(night_wagon != NULL);
    SDL_Texture* day_wagon = IMG_LoadTexture(ren,"../assets/Wagon_Day.png");
    assert(day_wagon != NULL);


    /*EXECUÇÃO*/

    SDL_Event evt;
    SDL_Rect background = {0,0,WEIGHT,HEIGHT};

    while (1){

        if (head->aceso == true){
            SDL_RenderCopy(ren,day_wagon,NULL,&background);
        }
        else{
            SDL_RenderCopy(ren,night_wagon,NULL,&background);
        }
        SDL_RenderPresent(ren);

        SDL_WaitEvent(&evt);
        if(evt.type == SDL_KEYDOWN){
            switch(evt.key.keysym.sym){
                case SDLK_LEFT:
                    transicao(head,ren,0);
                    break;
                case SDLK_RIGHT:
                    transicao(head,ren,1);
                    break;
                case SDLK_x:
                    goto exit;
                    break;
                case SDLK_SPACE:
                    head->aceso = !head->aceso;
            }
        }
        if (evt.type == SDL_QUIT){
            break;
        }
    }



    
    /*FINALIZAÇÃO*/
    exit:

    delete_train(head,n_vagao);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();


}

