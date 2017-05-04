//
//  noah.c
//  CommandLineBaseball
//
//  Created by Noah Panicola on 4/25/17.
//  Copyright © 2017 self.edu. All rights reserved.
//

#include "clb.h"

void populate(char *filename, Player list[], Player arrayHash[], int arrayLength){
    
    int i = 0;
    int key;
    
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    
    //return NULL if the file could not be read
    if(!fp){
        printf("The file could not be read.\n");
        return;
    }
    
    //populate array
    for(i=0; i < arrayLength; i++){
        fscanf(fp, "%s %s %d %d %d", list[i].first, list[i].last, &list[i].power, &list[i].contact, &list[i].pitching);
        key = getHashNumber(list[i].last, arrayHash);
        strcpy(arrayHash[key].first, list[i].first);
        strcpy(arrayHash[key].last, list[i].last);
        arrayHash[key].power = list[i].power;
        arrayHash[key].pitching = list[i].pitching;
        arrayHash[key].contact = list[i].contact;
        arrayHash[key].drafted = 0;
        arrayHash[key].ID = i;
        arrayHash[key].next = NULL;
        arrayHash[key].overall = (list[i].power + list[i].contact + list[i].pitching) / 3;
        list[i].drafted = 0;
        list[i].ID = i;
        list[i].next = NULL;
        list[i].overall = (list[i].power + list[i].contact + list[i].pitching) / 3;
    }
    fclose(fp);
}

int getLength(char *filename){
    
    FILE *fp = fopen(filename, "r");
    
    //return NULL if the file could not be read
    if(!fp){
        printf("The file could not be read.\n");
        return 0;
    }

    int lines = 0;
    int ch = 0;
    
    while(!feof(fp)){
        ch = fgetc(fp);
        if(ch == '\n'){
            lines++;
        }
    }
    
    fclose(fp);
    return lines;
}

void printArray(Player a[], int arrayLength){
    int i = 0;
    printf("ID First Last Power Contact Pitching\n");
    for(i=0; i < arrayLength; i++){
        if(a[i].drafted == 0){
            printf("%d %s %s %d %d %d\n", a[i].ID, a[i].first, a[i].last, a[i].power, a[i].contact, a[i].pitching);
        }
    }
}

Game* newGame(Player *team1, Player *team2){
    
    if(team1 == NULL || team2 == NULL){
        printf("Team values are null.\nTerminating program...\n");
        exit(-1);
    }
    
    Game *game = malloc(sizeof(Game));
    
    game->balls = 0;
    game->outs = 0;
    game->strikes = 0;
    game->inning = 1;
    
    game->team1 = team1;
    game->team2 = team2;
    game->next = NULL;
    
    game->cpu_pitcher = getHighestPitcher(team1);
    game->user_pitcher = getHighestPitcher(team2);
    
    return game;
}

Game* playGame(Game *game){
    
    AtBat at_bat;
    
    while(game->inning <= 9){
        
        while(game->outs != 3){
            
            at_bat = user_bat(game);
            
            //advance runner if balls = 4
            
            //add an out if strikes = 3
            
            //increment the batter
            game->team1 = game->team1->next;
        }
        
        game->inning ++;
    }
    
    return game;
}

AtBat user_bat(Game *game){
    
    Player *player, *pitcher;
    AtBat hit;
    int guess, pitch;
    
    while(1){
        
        //check for walk and strikeout
        if(game->balls == 4) {
            return WALK;
        } else if(game->strikes == 3) {
            return STRIKEOUT;
        }
        
        //print at bat
        printf("Pitcher: %s %s\nBatter: %s %s\n", game->cpu_pitcher->first, game->cpu_pitcher->first, game->team1->first, game->team1->last);
        printf("Outs: %d\nCount: %d-%d\n",game->outs, game->balls, game->strikes);
        printf("Select a pitch to hit (1-4): ");
        
        //get guess and pitch
        scanf("%d", &guess);
        pitch = (rand() % 5) + 1;
        
        if(pitch == 5){
            game->balls++;
            printf("Ball %d!\n", game->balls);
        } else if(guess == pitch){
            hit = determineHit(*player, *pitcher);
            return hit;
        } else {
            game->strikes++;
            printf("Strike %d!\n", game->strikes);
        }
    }
}

Player* getHighestPitcher(Player *team){
    
    Player *max = team;
    while(team->next != NULL){
        if(team->pitching > max->pitching){
            max->pitching = team->pitching;
        }
        team = team->next;
    }
    return max;
}

Player* getTeam(Game *game){
    
    Player *team;
    
    if(game->inning % 2 == 0){
        team = game->team2;
    } else {
        team = game->team1;
    }
    
    return team;
}
