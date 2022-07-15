#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "board.h"

struct piece_t
{
    enum size size;
    enum shape shape;
    enum color color;
    enum top top;
};

struct board_t
{
    piece board[DIMENSION][DIMENSION];
    int last_column;
    int last_line;
};

// Revoie le plateau passé en paramètre.
board copy_game(board original_game)
{
    return original_game;
}

// Permet de vider la mémoire du programme.
void destroy_game(board game)
{
    free(game);
}

// Vérifie si la case en "line" "column" est occupée ou non.
// Renvoie "vrai" ou "faux" si c'est le cas.
bool is_occupied(board game, int line, int column)
{
    if (game->board[line][column] == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Récupère la pièce qui est sur "line" et "column" dans le plateau "game".
// Renvoie NULL s'il n'y a pas de pièce
piece get_piece(board game, int line, int column)
{
    if (is_occupied(game, line, column) == 1 && line >= 0 && line < 4 && column >= 0 && column < 4)
    {
        piece p;
        p = malloc(sizeof(struct piece_t));

        p->color = game->board[line][column]->color;
        p->size = game->board[line][column]->size;
        p->top = game->board[line][column]->top;
        p->shape = game->board[line][column]->shape;
        return p;
    }
    else
    {
        return NULL;
    }
}

// Renvoie la taille, la couleur, la forme et le type de la piece, selon la fontion.
// Renvoie la caractéristique par défaut s'il n'y a pas de piece.
enum size piece_size(piece a_piece)
{
    if (a_piece == NULL)
    {
        return TALL;
    }

    return a_piece->size;
}

enum top piece_top(piece a_piece)
{
    if (a_piece == NULL)
    {
        return HOLLOW;
    }

    return a_piece->top;
}

enum color piece_color(piece a_piece)
{
    if (a_piece == NULL)
    {
        return RED;
    }

    return a_piece->color;
}

enum shape piece_shape(piece a_piece)
{
    if (a_piece == NULL)
    {
        return RED;
    }

    return a_piece->shape;
}

int check_line(board game, piece last_piece, int cSize, int cShape, int cColor, int cTop)
{
    for (int i = 0; i < 4; i++)
    {
        if (game->board[game->last_line][i] != NULL)
        {
            if (game->board[game->last_line][i]->size == last_piece->size)
            {
                cSize += 1;
            }
            if (game->board[game->last_line][i]->shape == last_piece->shape)
            {
                cShape += 1;
            }
            if (game->board[game->last_line][i]->color == last_piece->color)
            {
                cColor += 1;
            }
            if (game->board[game->last_line][i]->top == last_piece->top)
            {
                cTop += 1;
            }
        }
    }
    if (cSize == 4 || cShape == 4 || cColor == 4 || cTop == 4)
    {
        return 1;
    }

    return 0;
}

int check_column(board game, piece last_piece, int cSize, int cShape, int cColor, int cTop)
{
    for (int i = 0; i < 4; i++)
    {
        if (game->board[i][game->last_column] != NULL)
        {
            if (game->board[i][game->last_column]->size == last_piece->size)
            {
                cSize += 1;
            }
            if (game->board[i][game->last_column]->shape == last_piece->shape)
            {
                cShape += 1;
            }
            if (game->board[i][game->last_column]->color == last_piece->color)
            {
                cColor += 1;
            }
            if (game->board[i][game->last_column]->top == last_piece->top)
            {
                cTop += 1;
            }
        }
    }
    if (cSize == 4 || cShape == 4 || cColor == 4 || cTop == 4)
    {
        return 1;
    }

    return 0;
}

int check_diag1(board game, piece last_piece, int cSize, int cShape, int cColor, int cTop)
{
    for (int i = 0; i < 4; i++)
    {
        if (game->board[i][i] != NULL)
        {
            if (game->board[i][i]->size == last_piece->size)
            {
                cSize += 1;
            }
            if (game->board[i][i]->shape == last_piece->shape)
            {
                cShape += 1;
            }
            if (game->board[i][i]->color == last_piece->color)
            {
                cColor += 1;
            }
            if (game->board[i][i]->top == last_piece->top)
            {
                cTop += 1;
            }
        }
    }

    if (cSize == 4 || cShape == 4 || cColor == 4 || cTop == 4)
    {
        return 1;
    }

    return 0;
}

int check_diag2(board game, piece last_piece, int cSize, int cShape, int cColor, int cTop)
{
    for (int i = 0; i < 4; i++)
    {
        if (game->board[i][(DIMENSION - 1) - i] != NULL)
        {
            if (game->board[i][(DIMENSION - 1) - i]->size == last_piece->size)
            {
                cSize += 1;
            }
            if (game->board[i][(DIMENSION - 1) - i]->shape == last_piece->shape)
            {
                cShape += 1;
            }
            if (game->board[i][(DIMENSION - 1) - i]->color == last_piece->color)
            {
                cColor += 1;
            }
            if (game->board[i][(DIMENSION - 1) - i]->top == last_piece->top)
            {
                cTop += 1;
            }
        }
    }
    if (cSize == 4 || cShape == 4 || cColor == 4 || cTop == 4)
    {
        return 1;
    }

    return 0;
}

// Renvoie true ou false si la partie est terminée. Elle vérifie s'il y a bien
// 4 caractéristiques communes alignées.
bool has_winner(board game)
{
    int cSize = 0,
        cShape = 0,
        cColor = 0,
        cTop = 0;

    piece last_piece = get_piece(game, game->last_line, game->last_column);

    if (game->last_line == game->last_column && check_diag1(game, last_piece, cSize, cShape, cColor, cTop) == 1)
    {
        return 1;
    }

    if (game->last_column == (DIMENSION - 1) - game->last_line && check_diag2(game, last_piece, cSize, cShape, cColor, cTop) == 1)
    {
        return 1;
    }

    return check_column(game, last_piece, cSize, cShape, cColor, cTop) ||
           check_line(game, last_piece, cSize, cShape, cColor, cTop);
}

// Fontion qui renvoie "vrai" ou "faux" si la piece passée en paramètre est
// présente sur le plateau passé en paramètre.
bool is_present_on_board(board game, piece a_piece)
{
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            if (game->board[i][j] != NULL)
            {
                if (game->board[i][j]->color == a_piece->color &&
                    game->board[i][j]->size == a_piece->size &&
                    game->board[i][j]->shape == a_piece->shape &&
                    game->board[i][j]->top == a_piece->top)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// La fontion vérifie que le placement de la piece est possible, place la piece,
// renvoie SUCCESS en cas de bon déroulement de l'opération, POSITION ou PIECE sinon.
enum return_code place_piece(board game, int line, int column, piece a_piece)
{
    if ((line > DIMENSION || line < 0) || (column > DIMENSION || column < 0))
    {
        return POSITION;
    }
    else if (game->board[line][column] != NULL)
    {
        return PIECE;
    }

    game->board[line][column] = a_piece;
    game->last_column = column;
    game->last_line = line;
    return SUCCESS;
}

// La fonction permet de créer une piece à partir des caractéristiques, et renvoie une piece.
piece get_piece_from_characteristics(enum size asize, enum shape ashape, enum color acolor, enum top atop)
{
    piece p;
    p = malloc(sizeof(struct piece_t));

    p->size = asize;
    p->shape = ashape;
    p->color = acolor;
    p->top = atop;

    return p;
}

// La fonction crée un plateau de taille DIMENSION et rempli les cases vides par NULL.
board new_game()
{
    board game;
    game = malloc(sizeof(struct board_t));

    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            game->board[i][j] = NULL;
        }
    }

    return game;
}