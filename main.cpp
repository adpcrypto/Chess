// #define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Piece.h"
#include "Chessboard.h"

class Piece;
class ChessBoard;

#define WHITE_HOVER 80, 80, 202.5, 255
#define BLACK_HOVER 40, 40, 110, 255
#define WHITE_SQUARE 255, 255, 255, 255
#define BLACK_SQUARE 120, 90, 60, 255

void renderChoice(SDL_Renderer *, float, float, char);
void renderBackground(SDL_Renderer *, float, float);
void renderInitPieces(SDL_Renderer *, float, float, ChessBoard &);
void putPiecesOnTheBoard(SDL_Renderer *, float, float, ChessBoard &, vector<Piece>, vector<Piece>);
void renderMoveColors(SDL_Renderer *, vector<Move> &, float, float, ChessBoard &, int &);

using std::cin;
using std::cout;
using std::vector;

// Players team, 0 for white,1 for black
int players_team = WHITE;

// Next move, 0 for white,1 for black
int next_Move = WHITE;

// 0 to 15 for player
int selectedPiece = -1;

const int WIDTH = 800, HEIGHT = 800;

int main(int, char **)
{
    int exitflag = false;
    ChessBoard gameBoard;
    SDL_Event windowEvent;
    vector<Move> moves;
    int pieceTeam;

    // For storing the windo size for non-window events (mouse move doesn't return window size)
    float w = WIDTH, h = HEIGHT / 2;

    SDL_SetMainReady();
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);
    // TTF_Init();
    // TTF_Font *Sans = TTF_OpenFont("OpenSans-Regular.ttf", 24);

    SDL_Window *window = SDL_CreateWindow("Chess", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        cout << "Couldn't create Window.Exiting....!";
        return 1;
    }

    SDL_Renderer *m_renderer = SDL_CreateRenderer(window, NULL);
    if (!m_renderer)
    {
        cout << "Couldn't create Renderer.Exiting....!";
        return 1;
    }

    SDL_RenderClear(m_renderer);
    renderChoice(m_renderer, WIDTH, HEIGHT / 2, 'O');
    SDL_SetWindowSize(window, WIDTH, HEIGHT / 2);
    SDL_RenderPresent(m_renderer);

    while (!exitflag)
    {
        if (SDL_WaitEvent(&windowEvent))
        {
            switch (windowEvent.type)
            {
            case SDL_EVENT_QUIT:
                exitflag = true;
                exit(0);
                break;
            // case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            case SDL_EVENT_WINDOW_RESIZED:
                w = windowEvent.window.data1;
                h = windowEvent.window.data2;
                renderChoice(m_renderer, w, h, 'O');
                SDL_RenderPresent(m_renderer);
                continue;
            case SDL_EVENT_MOUSE_MOTION:
                if (windowEvent.motion.x < 0.005 * w || windowEvent.motion.x > 0.995 * w ||
                    windowEvent.motion.y < 0.005 * h || windowEvent.motion.y > 0.995 * h)
                    renderChoice(m_renderer, w, h, 'O');
                else if (windowEvent.motion.x > w / 2)
                    renderChoice(m_renderer, w, h, 'B');
                else
                    renderChoice(m_renderer, w, h, 'W');
                SDL_RenderPresent(m_renderer);
                continue;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                players_team = windowEvent.motion.x < w / 2 ? WHITE : BLACK;
                exitflag = true;
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:

                break;
            }
        }
    }

    SDL_SetWindowSize(window, WIDTH, HEIGHT);

    exitflag = false;

    SDL_RenderClear(m_renderer);
    renderBackground(m_renderer, WIDTH, HEIGHT);
    renderInitPieces(m_renderer, WIDTH, HEIGHT, gameBoard);
    SDL_RenderPresent(m_renderer);

    // Gives priority, yellow color in bottom bar as if notified
    // SDL_RaiseWindow(window);

    // TODO image is being loaded every time in Piece.h, fix it
    // if (players_team == BLACK)
    // {
    //     gameBoard.moveList.clear();
    //     gameBoard.playTheBestMove(next_Move);

    //     gameBoard.moveList.clear();
    //     for (auto &i : gameBoard.whitePieces)
    //         i.moveList.clear();
    //     for (auto &i : gameBoard.blackPieces)
    //         i.moveList.clear();
    //     next_Move = BLACK;
    // }
    while (!exitflag)
    {
        if (SDL_WaitEvent(&windowEvent))
        {
            switch (windowEvent.type)
            {
            case SDL_EVENT_QUIT:
                exitflag = true;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                w = windowEvent.window.data1;
                h = windowEvent.window.data2;
                renderBackground(m_renderer, w, h);
                gameBoard.renderBoard(m_renderer, w, h);
                SDL_RenderPresent(m_renderer);
                continue;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                renderBackground(m_renderer, w, h);
                selectedPiece = gameBoard.getPieceIndex(windowEvent.motion.x, windowEvent.motion.y, w, h, pieceTeam);
                // selectedPiece = index;
                if (selectedPiece == -1)
                {
                    gameBoard.renderBoard(m_renderer, w, h);
                    SDL_RenderPresent(m_renderer);
                    continue;
                }
                if (next_Move == WHITE && pieceTeam == WHITE)
                {
                    gameBoard.getPossibleMoves(gameBoard.whitePieces.at(selectedPiece), true);
                    moves = gameBoard.whitePieces.at(selectedPiece).moveList;
                    renderMoveColors(m_renderer, moves, w, h, gameBoard, pieceTeam);
                }
                else if (next_Move == BLACK && pieceTeam == BLACK)
                {
                    gameBoard.getPossibleMoves(gameBoard.blackPieces.at(selectedPiece), true);
                    // cout << "\nX:" << gameBoard.blackPieces.at(index).locx << "Y" << gameBoard.blackPieces.at(index).locy << "\n";
                    moves = gameBoard.blackPieces.at(selectedPiece).moveList;
                    renderMoveColors(m_renderer, moves, w, h, gameBoard, pieceTeam);
                }
                else
                {
                    moves.clear();
                }

                gameBoard.renderBoard(m_renderer, w, h);

                SDL_RenderPresent(m_renderer);

                continue;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                // cout << "MOUSE UP";
                int x = windowEvent.motion.x;
                int y = windowEvent.motion.y;
                x = x * 8 / w;
                y = 8 - y * 8 / h;
                if (players_team == BLACK)
                {
                    x = 7 - x;
                    y = 7 - y;
                }
                // cout << "\nX" << x << "Y" << y;
                for (auto &move : moves)
                {

                    // cout << "tolocX" << move.toLocX << "tplocY" << move.toLocY << "\n";
                    if (move.toLocX == x && move.toLocY == y)
                    {
                        // cout << "MOVE";
                        gameBoard.movePiece(move);
                        gameBoard.previousMove = move;
                        next_Move = 1 - next_Move;
                        gameBoard.moveList.clear();
                        break;
                    }
                    // cout << "NO MOVE";
                }
                renderBackground(m_renderer, w, h);
                gameBoard.renderBoard(m_renderer, w, h);
                SDL_RenderPresent(m_renderer);
                // renderPieces(m_renderer, w, h, gameBoard, gameBoard.whitePieces, gameBoard.blackPieces);
                // cout << "HERE";
                selectedPiece = -1;
                // gameBoard.nextTurn = next_Move;
                moves.clear();
                for (auto &I : gameBoard.whitePieces)
                {
                    I.moveList.clear();
                }
                for (auto &I : gameBoard.blackPieces)
                {
                    I.moveList.clear();
                }

                gameBoard.moveList.clear();
                //  gameBoard.playTheBestMove(next_Move);
                //  next_Move = 1 - next_Move;
                //  renderBackground(m_renderer, w, h);
                //  gameBoard.renderBoard(m_renderer, w, h);
                //  clearALLloadedMoves();
                continue;
            }
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    // TTF_CloseFont(Sans);

    // std::cout << "HI";

    return 0;
}

void renderInitPieces(SDL_Renderer *m_renderer, float w, float h, ChessBoard &gameBoard)
{
    vector<Piece> whitePieces = {
        Piece(WHITE, PAWN, 0, 1, 0),
        Piece(WHITE, PAWN, 1, 1, 0),
        Piece(WHITE, PAWN, 2, 1, 0),
        Piece(WHITE, PAWN, 3, 1, 0),
        Piece(WHITE, PAWN, 4, 1, 0),
        Piece(WHITE, PAWN, 5, 1, 0),
        Piece(WHITE, PAWN, 6, 1, 0),
        Piece(WHITE, PAWN, 7, 1, 0),
        Piece(WHITE, ROOK, 0, 0, 0),
        Piece(WHITE, ROOK, 7, 0, 0),
        Piece(WHITE, KNIGHT, 1, 0, 0),
        Piece(WHITE, KNIGHT, 6, 0, 0),
        Piece(WHITE, BISHOP, 2, 0, 0),
        Piece(WHITE, BISHOP, 5, 0, 0),
        Piece(WHITE, QUEEN, 3, 0, 0),
        Piece(WHITE, KING, 4, 0, 0)};

    vector<Piece> blackPieces = {
        Piece(BLACK, PAWN, 0, 6, 0),
        Piece(BLACK, PAWN, 1, 6, 0),
        Piece(BLACK, PAWN, 2, 6, 0),
        Piece(BLACK, PAWN, 3, 6, 0),
        Piece(BLACK, PAWN, 4, 6, 0),
        Piece(BLACK, PAWN, 5, 6, 0),
        Piece(BLACK, PAWN, 6, 6, 0),
        Piece(BLACK, PAWN, 7, 6, 0),
        Piece(BLACK, ROOK, 0, 7, 0),
        Piece(BLACK, ROOK, 7, 7, 0),
        Piece(BLACK, KNIGHT, 1, 7, 0),
        Piece(BLACK, KNIGHT, 6, 7, 0),
        Piece(BLACK, BISHOP, 2, 7, 0),
        Piece(BLACK, BISHOP, 5, 7, 0),
        Piece(BLACK, QUEEN, 3, 7, 0),
        Piece(BLACK, KING, 4, 7, 0)};

    putPiecesOnTheBoard(m_renderer, w, h, gameBoard, whitePieces, blackPieces);
}

void renderMoveColors(SDL_Renderer *m_renderer, vector<Move> &moves, float w, float h, ChessBoard &gameBoard, int &pieceTeam)
{
    for (auto &move : moves)
    {
        int locx = move.toLocX, locy = move.toLocY;
        // cout << "Piecex" << move.piece->locx << "Piecey" << move.piece->locy << "\n";
        // cout << "TOlocx" << locx << "TOlocy" << locy << "\n";
        ((locx + locy) % 2 == 0) ? SDL_SetRenderDrawColor(m_renderer, BLACK_HOVER) : SDL_SetRenderDrawColor(m_renderer, WHITE_HOVER);
        int team;

        gameBoard.getPiece(team, nullptr, locx, locy);
        SDL_FRect rectangle;
        if (players_team == WHITE)
            rectangle = {locx * w / 8,
                         h - (locy + 1) * h / 8,
                         w / 8,
                         h / 8};
        else if (players_team == BLACK)
            rectangle = {w - (locx + 1) * w / 8,
                         locy * h / 8,
                         w / 8,
                         h / 8};
        SDL_RenderFillRect(m_renderer, &rectangle);

        int index;
        if (team != -1)
        {
            index = gameBoard.getPieceIndex(locx, locy, pieceTeam);
            if (pieceTeam == WHITE)
            {
                gameBoard.whitePieces.at(index).renderPiece(m_renderer, WHITE, w, h);
            }
            else
            {
                gameBoard.blackPieces.at(index).renderPiece(m_renderer, BLACK, w, h);
            }
        }
    }
}

void putPiecesOnTheBoard(SDL_Renderer *m_renderer, float w, float h, ChessBoard &gameBoard,
                         vector<Piece> whitePieces, vector<Piece> blackPieces)
{
    gameBoard = ChessBoard(whitePieces, blackPieces, whitePieces.size(), blackPieces.size(), players_team);

    gameBoard.renderBoard(m_renderer, w, h);
}

void renderBackground(SDL_Renderer *m_renderer, float w, float h)
{
    bool white = true;
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (white)
            {
                SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(m_renderer, 155, 103, 60, 255);
            }
            white = !white;
            SDL_FRect rectangle = {i * w / 8,
                                   j * h / 8,
                                   w / 8,
                                   h / 8};
            SDL_RenderFillRect(m_renderer, &rectangle);
        }
        white = !white;
    }
}

void renderChoice(SDL_Renderer *m_renderer, float w, float h, char loc)
{

    if (loc == 'B')
    {
        SDL_SetRenderDrawColor(m_renderer, WHITE_SQUARE);
        SDL_FRect rectangle = {0,
                               0,
                               w / 2,
                               h};
        SDL_RenderFillRect(m_renderer, &rectangle);

        SDL_SetRenderDrawColor(m_renderer, BLACK_HOVER);
        rectangle = {w / 2,
                     0,
                     w / 2,
                     h};
        SDL_RenderFillRect(m_renderer, &rectangle);
    }
    else if (loc == 'W')
    {
        SDL_SetRenderDrawColor(m_renderer, WHITE_HOVER);
        SDL_FRect rectangle = {0,
                               0,
                               w / 2,
                               h};
        SDL_RenderFillRect(m_renderer, &rectangle);

        SDL_SetRenderDrawColor(m_renderer, BLACK_SQUARE);
        rectangle = {w / 2,
                     0,
                     w / 2,
                     h};
        SDL_RenderFillRect(m_renderer, &rectangle);
    }
    else
    {
        SDL_SetRenderDrawColor(m_renderer, WHITE_SQUARE);
        SDL_FRect rectangle = {0,
                               0,
                               w / 2,
                               h};
        SDL_RenderFillRect(m_renderer, &rectangle);

        SDL_SetRenderDrawColor(m_renderer, BLACK_SQUARE);
        rectangle = {w / 2,
                     0,
                     w / 2,
                     h};
        SDL_RenderFillRect(m_renderer, &rectangle);
    }
}