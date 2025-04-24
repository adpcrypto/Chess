#pragma once
#include <SDL3_image/SDL_image.h>
#include <vector>

enum Team
{
    WHITE = 0,
    BLACK = 1,
    EMPTY = -1,
    OUT_OF_BOUNDS = -2
};

enum Piecetype
{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

enum MoveType
{
    MOVE,
    CASTLE,
    ENPASSANT,
    NEWPIECE,
    CAPTURE,
    PAWN_DOUBLE_PUSH
};
enum Moved
{
    NOT_MOVED,
    MOVED
};
class Piece;
struct Move
{
    Piece *piece;
    int toLocX;
    int toLocY;
    int moveType;
};
class Piece
{
public:
    // Team enum
    int team;

    // Type enum
    int type;

    // square number, 0 for "a" NOT THE LEFT SIDE,0-7
    int locx;

    // square number, 0 for rank 1 NOT THE BOTTOM SIDE,0-7
    int locy;

    // 0 means not moved, anything else means moved
    int isMoved;

    std::vector<Move> moveList;

    Piece() {}

    Piece(int Team, Piecetype Type, int locX, int locY, int IsMoved)
    {
        team = Team;
        type = Type;
        locx = locX;
        locy = locY;
        isMoved = IsMoved;
        moveList.clear();
    }

    void renderPiece(SDL_Renderer *m_renderer, int players_team, float &w, float &h)
    {
        SDL_Surface *loadedPieceSurface = NULL;
        if (team == WHITE)
        {
            switch (type)
            {
            case PAWN:
                loadedPieceSurface = IMG_Load("res/whitePawn.png");
                break;
            case ROOK:
                loadedPieceSurface = IMG_Load("res/whiteRook.png");
                break;
            case BISHOP:
                loadedPieceSurface = IMG_Load("res/whiteBishop.png");
                break;
            case KNIGHT:
                loadedPieceSurface = IMG_Load("res/whiteKnight.png");
                break;
            case KING:
                loadedPieceSurface = IMG_Load("res/whiteKing.png");
                break;
            case QUEEN:
                loadedPieceSurface = IMG_Load("res/whiteQueen.png");
                break;
            }
        }
        else
        {
            switch (type)
            {
            case PAWN:
                loadedPieceSurface = IMG_Load("res/blackPawn.png");
                break;
            case ROOK:
                loadedPieceSurface = IMG_Load("res/blackRook.png");
                break;
            case BISHOP:
                loadedPieceSurface = IMG_Load("res/blackBishop.png");
                break;
            case KNIGHT:
                loadedPieceSurface = IMG_Load("res/blackKnight.png");
                break;
            case KING:
                loadedPieceSurface = IMG_Load("res/blackKing.png");
                break;
            case QUEEN:
                loadedPieceSurface = IMG_Load("res/blackQueen.png");
                break;
            }
        }

        SDL_FRect pieceLocRect;
        SDL_FRect piecedestRect = {0, 0, 60, 60};

        if (players_team == WHITE)
        {
            pieceLocRect = {w * locx / 8, h * (7 - locy) / 8, w / 8, h / 8};
        }
        else
        {
            pieceLocRect = {w * (7 - locx) / 8, h * locy / 8, w / 8, h / 8};
        }
        SDL_Texture *loadedPieceTexture = SDL_CreateTextureFromSurface(m_renderer, loadedPieceSurface);
        SDL_RenderTexture(m_renderer, loadedPieceTexture, &piecedestRect, &pieceLocRect);

        SDL_DestroySurface(loadedPieceSurface);
        SDL_DestroyTexture(loadedPieceTexture);
    };

    // void deRenderPiece();
};

bool operator==(const Piece &a, const Piece &b)
{
    return a.isMoved == b.isMoved && a.locx == b.locx && a.locy == b.locy && a.team == b.team && a.type == b.type;
}

bool operator==(const Move &a, const Move &b)
{
    return a.moveType == b.moveType && a.piece == b.piece && a.toLocX == b.toLocX && a.toLocY == b.toLocY;
}
