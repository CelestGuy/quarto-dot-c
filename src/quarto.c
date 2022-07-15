#include "./headers/board.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void init_tab(piece ptab[16])
{
    piece p;
    int tour = 0;

    for (int size = 0; size < 2; size++)
    {
        for (int shape = 0; shape < 2; shape++)
        {
            for (int color = 0; color < 2; color++)
            {
                for (int top = 0; top < 2; top++)
                {
                    p = get_piece_from_characteristics(size, shape, color, top);
                    ptab[tour] = p;
                    tour++;
                }
            }
        }
    }
}

void create_players(char player1[], char player2[])
{
    int tour = 0;

    printf("Choisir nom du joueur 1: ");
    fgets(player1, 32, stdin);
    while (player1[tour] != '\0')
    {
        if (player1[tour] == '\n')
        {
            player1[tour] = '\0';
        }
        tour++;
    }

    tour = 0;
    printf("Choisir nom du joueur 2: ");
    fgets(player2, 32, stdin);
    while (player2[tour] != '\0')
    {
        if (player2[tour] == '\n')
        {
            player2[tour] = '\0';
        }
        tour++;
    }
}

void choix_position(int *ligne, int *colonne, piece p, board game, char player[])
{
    printf("\nAu tour de \033[4m\033[32m%s\033[0m\n", player);
    enum return_code res = POSITION;

    while (res != SUCCESS)
    {
        do
        {
            printf("Placer la pièce sur la ligne: ");
            while (scanf("%d", &*ligne) == 0)
            {
                getchar();
            }

            printf("Placer la pièce sur la colonne: ");
            while (scanf("%d", &*colonne) == 0)
            {
                getchar();
            }

        } while ((*ligne < 0 || *ligne >= 4) && (*colonne < 0 || *colonne >= 4));

        res = place_piece(game, *ligne, *colonne, p);
    }
}

int square(int i, int j, piece p)
{
    int res;
    if (piece_shape(p) == SQUARE)
    {
        res = 1;
    }
    else
    {
        if (piece_size(p) == TALL)
        {
            res = (i % 6 != 1 && i % 6 != 5) || (j % 6 != 1 && j % 6 != 5);
        }
        else
        {
            res = (i % 6 != 2 && i % 6 != 4) || (j % 6 != 2 && j % 6 != 4);
        }
    }
    return res;
}

int tall(int i, int j, piece p)
{
    int res;
    if (piece_size(p) == SHORT)
    {
        res = (i % 6 != 1 && i % 6 != 5) && (j % 6 != 1 && j % 6 != 5);
    }
    else
    {
        res = 1;
    }
    return res;
}

int hollow(int i, int j, piece p)
{
    int res;
    if (piece_top(p) == HOLLOW)
    {
        res = (i % 6 != 3) || (j % 6 != 3);
    }
    else
    {
        res = 1;
    }
    return res;
}

void affichage_plateau(board game)
{
    piece p;
    for (int i = 0; i <= 24; i++)
    {
        for (int j = 0; j <= 24; j++)
        {
            p = get_piece(game, i / 6, j / 6);

            if ((j % 6 == 0 || i % 6 == 0) && j <= 24)
            {
                if (i == 0 && j % 6 == 3)
                {
                    printf("\033[47m\033[30m%.2d\033[0m", (j / 6));
                }
                else if (j == 0 && i % 6 == 3)
                {
                    printf("\033[47m\033[30m%.2d\033[0m", (i / 6));
                }
                else
                {
                    printf("\033[37m██\033[0m");
                }
            }
            else
            {
                if (p != NULL)
                {
                    if (tall(i, j, p) && square(i, j, p) && hollow(i, j, p))
                    {
                        if (piece_color(p) == RED)
                        {
                            printf("\033[31m██\033[0m");
                        }
                        else
                        {
                            printf("\033[34m██\033[0m");
                        }
                    }
                    else
                    {
                        printf("\033[40m  \033[0m");
                    }
                }
                else
                {
                    printf("\033[40m  \033[0m");
                }
            }
        }
        printf("\n");
    }
}

void show_available_pieces(board game, piece ptab[16])
{
    int tour = 0;
    int size, shape, color, top;

    for (int i = 0; i < 12; i++)
    {
        for (int j = 1; j < 48; j++)
        {
            tour = (j / 6) + ((i / 6) * 8);
            size = piece_size(ptab[tour]);
            shape = piece_shape(ptab[tour]);
            color = piece_color(ptab[tour]);
            top = piece_top(ptab[tour]);

            if ((j % 6 == 0 || i % 6 == 0) && j <= 48)
            {
                if ((i == 0 && j % 6 == 3 && tour < 8) || (i == 6 && j % 6 == 3 && tour > 7))
                {
                    if (is_present_on_board(game, ptab[tour]))
                    {
                        printf("    ");
                    }
                    else
                    {
                        printf("\033[37m%1d%1d%1d%1d\033[0m", size, shape, color, top);
                    }
                }
                else if (j % 6 == 2 || j % 6 == 4)
                {
                    printf("\033[37m \033[0m");
                }
                else if (j % 6 < 2 || j % 6 > 4)
                {
                    printf("\033[37m  \033[0m");
                }
            }
            else
            {
                if (tall(i, j, ptab[tour]) && square(i, j, ptab[tour]) && hollow(i, j, ptab[tour]) && !is_present_on_board(game, ptab[tour]))
                {
                    if (piece_color(ptab[tour]) == RED)
                    {
                        printf("\033[31m██\033[0m");
                    }
                    else
                    {
                        printf("\033[34m██\033[0m");
                    }
                }
                else
                {
                    printf("  ");
                }
            }
        }
        printf("\n");
    }
}

void show_selected_piece(piece p)
{
    for (int i = 1; i < 6; i++)
    {
        for (int j = 1; j < 6; j++)
        {
            if (tall(i, j, p) && square(i, j, p) && hollow(i, j, p))
            {
                if (piece_color(p) == RED)
                {
                    printf("\033[31m██\033[0m");
                }
                else
                {
                    printf("\033[34m██\033[0m");
                }
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
}

piece select_piece(board game, char player[])
{
    piece p;
    int size, shape, color, top;

    do
    {
        printf("\033[4m\033[33m%s\033[0m, veuillez sélectionner une piece: ", player);
        while (scanf("%1d%1d%1d%d", &size, &shape, &color, &top) == 0)
        {
            getchar();
        }
        p = get_piece_from_characteristics(size, shape, color, top);

    } while ((size < 0 || size > 1) || (shape < 0 || shape > 1) || (color < 0 || color > 1) || (top < 0 || top > 1) || is_present_on_board(game, p));

    return p;
}

int main(int argc, char **argv)
{
    if (argc >= 2)
    {
        if ((strcmp("-help", argv[1]) == 0) || (strcmp("-h", argv[1]) == 0))
        {
            system("cat < ./manual.txt");
        }
        else
        {
            printf("Mauvais argument rentré ! Faites -help ou -h\n");
        }
        return 0;
    }

    system("clear && cat < ./title.txt");

    piece p;
    piece ptab[16];
    board game = new_game();

    int ligne = 0;
    int colonne = 0;
    int player_turn = 0;

    char player1[32];
    char player2[32];

    init_tab(ptab);
    create_players(player1, player2);

    while (has_winner(game) == 0)
    {
        printf("\nListe des pièces disponibles:\n");
        show_available_pieces(game, ptab);

        p = select_piece(game, player_turn == 0 ? player2 : player1);
        system("clear");
        affichage_plateau(game);
        printf("\nPièce sélectionnée:\n");
        show_selected_piece(p);

        choix_position(&ligne, &colonne, p, game, player_turn == 0 ? player1 : player2);

        system("clear");
        affichage_plateau(game);

        player_turn = (player_turn + 1) % 2;
    }

    system("clear && cat < ./title.txt");
    affichage_plateau(game);
    printf("\033[1mLe gagnant est \033[35m%s\033[0m\033[1m, Bravo à vous !\033[0m\nMerci d'avoir joué, à bientôt !\n", (player_turn == 0 ? player2 : player1));

    destroy_game(game);
    return 0;
}