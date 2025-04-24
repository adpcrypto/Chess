#pragma once

#include "Piece.h"
#include <vector>
#include <iostream>
#include <algorithm>

using std::cout;
using std::vector;

class ChessBoard
{
public:
    int noOfWhitePieces, noOfBlackPieces;
    vector<Piece> whitePieces;
    vector<Piece> blackPieces;
    int playersTeam;
    Move previousMove;
    vector<Move> moveList;

    ChessBoard() {}

    ChessBoard(vector<Piece> whitepieces, vector<Piece> blackpieces, int noOfWhitepieces, int noOfBlackpieces, char players_team)
    {
        whitePieces.reserve(16);
        blackPieces.reserve(16);
        // max possible moves in a legal game in one turn is 218
        moveList.reserve(220);
        whitePieces = whitepieces;
        blackPieces = blackpieces;
        noOfWhitePieces = noOfWhitepieces;
        noOfBlackPieces = noOfBlackpieces;
        playersTeam = players_team;
        moveList.clear();
    }

    void renderBoard(SDL_Renderer *m_renderer, float &w, float &h)
    {
        for (auto &piece : whitePieces)
        {
            piece.renderPiece(m_renderer, playersTeam, w, h);
        }
        for (auto &piece : blackPieces)
        {
            piece.renderPiece(m_renderer, playersTeam, w, h);
        }
    }

    int getPieceIndex(float &x, float &y, float &w, float &h, int &pieceTeam)
    {
        int locX = x * 8 / w;
        int locY = 8 - (y * 8 / h);

        if (playersTeam == BLACK)
        {
            locX = 7 - locX;
            locY = 7 - locY;
        }

        // std::cout << "X:" << locX << "Y" << locY;
        return getPieceIndex(locX, locY, pieceTeam);
    }

    // DO NOT CONVERT TO &locX
    // WE ARE PASSING AS POS-1 IN PLACES
    // LVALUE ERROR
    int getWhitePieceIndex(int locX, int locY, int &pieceTeam)
    {
        int index = -1;

        int k = 0;
        for (auto &i : whitePieces)
        {
            if (i.locx == locX && i.locy == locY)
            {
                index = k;
                pieceTeam = WHITE;
                return index;
            }
            k++;
        }
        return index;
    }

    int getBlackPieceIndex(int locX, int locY, int &pieceTeam)
    {
        int index = -1;

        int k = 0;
        for (auto &i : blackPieces)
        {
            if (i.locx == locX && i.locy == locY)
            {
                index = k;
                pieceTeam = WHITE;
                return index;
            }
            k++;
        }

        return index;
    }
    int getPieceIndex(int locX, int locY, int &pieceTeam)
    {
        int index = -1;

        int k = 0;
        for (auto &i : whitePieces)
        {
            if (i.locx == locX && i.locy == locY)
            {
                index = k;
                pieceTeam = WHITE;
                return index;
            }
            k++;
        }

        k = 0;
        for (auto &i : blackPieces)
        {
            if (i.locx == locX && i.locy == locY)
            {
                index = k;
                pieceTeam = BLACK;
                return index;
            }
            k++;
        }
        // }
        return index;
    }

    void getPiece(int &team, std::nullptr_t type, int locX, int locY)
    {
        if (locX > 7 || locX < 0 || locY > 7 || locY < 0)
        {
            cout << "out of bounds";
            team = OUT_OF_BOUNDS;
            return;
        }
        for (auto &i : whitePieces)
        {
            if (i.locx == locX && i.locy == locY)
            {
                cout << "WHITE" << i.type << "\n";
                team = WHITE;
                return;
            }
        }
        for (auto &i : blackPieces)
        {
            if (i.locx == locX && i.locy == locY)
            {
                cout << "BLACK" << i.type << "\n";
                team = BLACK;
                return;
            }
        }
        // NO PIECE HERE
        team = EMPTY;
        // cout << "NO PIECE";
        return;
    }
    void getPiece(int &team, Piece &piece, int locX, int locY)
    {
        if (locX > 7 || locX < 0 || locY > 7 || locY < 0)
        {
            cout << "out of bounds";
            // OUT OF BOUNDS
            team = OUT_OF_BOUNDS;
        }
        for (auto i : whitePieces)
        {
            if (i.locx == locX && i.locy == locY)
            {
                // cout << "WHITE" << i.type << "\n";
                team = WHITE;
                piece = i;
                return;
            }
        }
        for (auto i : blackPieces)
        {
            if (i.locx == locX && i.locy == locY)
            {
                // cout << "BLACK" << i.type << "\n";
                team = BLACK;
                piece = i;
                return;
            }
        }
        // NO PIECE HERE
        team = EMPTY;
        // cout << "NO PIECE";
        return;
    }
    void getKing(int team, Piece &king)
    {
        vector<Piece> a = (team == WHITE ? whitePieces : blackPieces);
        for (auto &I : a)
        {
            if (I.type == KING)
                king = I;
        }
    }

    // void getRook(int team, vector<Piece> &rook)
    // {
    //     vector<Piece> a = (team == WHITE ? whitePieces : blackPieces);
    //     for (auto &I : a)
    //     {
    //         if (I.type == KING)
    //             rook.push_back(I);
    //     }
    // }

    bool isCheck(Piece &king)
    {
        vector<Piece> &pieceList = (king.team == WHITE ? blackPieces : whitePieces);
        for (auto &I : pieceList)
        {
            for (auto &J : I.moveList)
            {
                // if (I.type == QUEEN)
                // {
                //     cout << "QUEEN MOVE" << J.toLocX << J.toLocY;
                // }
                if ((J.toLocX == king.locx) && (J.toLocY == king.locy))
                {
                    // cout << "CHECK BY" << I.type << I.locx << I.locy;
                    return true;
                }
            }
        }
        return false;
    }

    void getPossibleMoves(Piece &piece, bool checkCheck)
    {
        if (piece.type == ROOK)
        {
            int team;
            cout << "ROOK" << piece.locx << piece.locy;
            for (int dx = -1; dx < 2; dx++)
            {
                for (int dy = -1; dy < 2; dy++)
                {
                    if ((dx == 0 || dy == 0) && !(dx == 0 && dy == 0))
                    {
                        for (int i = 1; i <= 7; i++)
                        {

                            getPiece(team, nullptr, piece.locx + dx * i, piece.locy + dy * i);
                            // cout << "dx" << dx << "dy" << dy << "i" << i << "team" << team << "\n";
                            if (team == OUT_OF_BOUNDS || team == (piece.team))
                                break;
                            else if (team == EMPTY)
                            {
                                piece.moveList.push_back({&piece, piece.locx + dx * i, piece.locy + dy * i, MOVE});
                                // cout << "\nMOVE:" << piece.type << "team" << piece.team << "FROMx" << piece.locx << "FROMy" << piece.locy << "TOX" << piece.locx + dx * i << "toY" << piece.locx + dy * i << "\n";
                            }
                            else
                            {
                                piece.moveList.push_back({&piece, piece.locx + dx * i, piece.locy + dy * i, CAPTURE});
                                // cout << "\nMOVE:" << piece.type << "team" << piece.team << "FROMx" << piece.locx << "FROMy" << piece.locy << "TOX" << piece.locx + dx * i << "toY" << piece.locx + dy * i << "\n";
                                break;
                            }
                        }
                    }
                }
            }
        }
        else if (piece.type == PAWN)
        {
            cout << "PAWN" << piece.locx << piece.locy;
            int d = (piece.team == WHITE) ? 1 : -1;
            // cout << "d" << d << "\n";
            int t1;
            int t2;
            getPiece(t1, nullptr, piece.locx, piece.locy + d);
            if (!piece.isMoved)
            {
                // cout << "NOT MOVED\n";
                getPiece(t2, nullptr, piece.locx, piece.locy + 2 * d);
                if (t1 == EMPTY && t2 == EMPTY)
                {
                    piece.moveList.push_back({&piece, piece.locx, piece.locy + 2 * d, PAWN_DOUBLE_PUSH});
                    // cout << "DOUBLE PUSH" << piece.locx << piece.locy + 2 * d;
                }
            }

            getPiece(t1, nullptr, piece.locx, piece.locy + d);
            if (t1 == EMPTY)
            {
                piece.moveList.push_back({&piece, piece.locx, piece.locy + d, MOVE});
                // cout << "ONE STEP\n";
            }
            getPiece(t2, nullptr, piece.locx - 1, piece.locy + d);
            if (t2 == (1 - piece.team))
                piece.moveList.push_back({&piece, piece.locx - 1, piece.locy + d, CAPTURE});

            getPiece(t2, nullptr, piece.locx + 1, piece.locy + d);
            if (t2 == (1 - piece.team))
                piece.moveList.push_back({&piece, piece.locx + 1, piece.locy + d, CAPTURE});

            int l = previousMove.toLocX;
            if ((previousMove.moveType == PAWN_DOUBLE_PUSH) && (previousMove.toLocY == piece.locy) && ((l == (piece.locx - 1)) || (l == (piece.locx + 1))))
            {
                cout << "ENPASSANT TO" << l << piece.locy + d;
                piece.moveList.push_back({&piece, l, piece.locy + d, ENPASSANT});
            }
        }
        else if (piece.type == KNIGHT)
        {
            int team;
            cout << "kNIGht" << piece.locx << piece.locy;
            for (int dx = -2; dx < 3; dx++)
            {
                for (int dy = -2; dy < 3; dy++)
                {

                    if (abs(dx) + abs(dy) == 3)
                    {

                        getPiece(team, nullptr, piece.locx + dx, piece.locy + dy);
                        // cout << "dx" << dx << "dy" << dy << "i" << i << "team" << team << "\n";
                        if (team == OUT_OF_BOUNDS || team == (piece.team))
                            continue;
                        else if (team == EMPTY)
                        {
                            piece.moveList.push_back({&piece, piece.locx + dx, piece.locy + dy, MOVE});
                            // cout << "\nMOVE:" << piece.type << "team" << piece.team << "FROMx" << piece.locx << "FROMy" << piece.locy << "TOX" << piece.locx + dx * i << "toY" << piece.locx + dy * i << "\n";
                        }
                        else
                        {
                            piece.moveList.push_back({&piece, piece.locx + dx, piece.locy + dy, CAPTURE});
                            // cout << "\nMOVE:" << piece.type << "team" << piece.team << "FROMx" << piece.locx << "FROMy" << piece.locy << "TOX" << piece.locx + dx * i << "toY" << piece.locx + dy * i << "\n";
                            continue;
                        }
                    }
                }
            }
        }
        else if (piece.type == BISHOP)
        {
            int team;
            cout << "BISHOP" << piece.locx << piece.locy;
            for (int dx = -1; dx < 2; dx++)
            {
                for (int dy = -1; dy < 2; dy++)
                {
                    if (!(dx == 0) && !(dy == 0))
                    {
                        for (int i = 1; i <= 7; i++)
                        {

                            getPiece(team, nullptr, piece.locx + dx * i, piece.locy + dy * i);
                            // cout << "dx" << dx << "dy" << dy << "i" << i << "team" << team << "\n";
                            if (team == OUT_OF_BOUNDS || team == (piece.team))
                                break;
                            else if (team == EMPTY)
                            {
                                piece.moveList.push_back({&piece, piece.locx + dx * i, piece.locy + dy * i, MOVE});
                                // cout << "\nMOVE:" << piece.type << "team" << piece.team << "FROMx" << piece.locx << "FROMy" << piece.locy << "TOX" << piece.locx + dx * i << "toY" << piece.locx + dy * i << "\n";
                            }
                            else
                            {
                                piece.moveList.push_back({&piece, piece.locx + dx * i, piece.locy + dy * i, CAPTURE});
                                // cout << "\nMOVE:" << piece.type << "team" << piece.team << "FROMx" << piece.locx << "FROMy" << piece.locy << "TOX" << piece.locx + dx * i << "toY" << piece.locx + dy * i << "\n";
                                break;
                            }
                        }
                    }
                }
            }
        }
        else if (piece.type == QUEEN)
        {
            int team;
            cout << "QUEEN" << piece.locx << piece.locy;
            for (int dx = -1; dx < 2; ++dx)
            {
                for (int dy = -1; dy < 2; ++dy)
                {
                    if (!(dx == 0 && dy == 0))
                    {
                        for (int i = 1; i <= 7; i++)
                        {

                            getPiece(team, nullptr, piece.locx + dx * i, piece.locy + dy * i);
                            // cout << "dx" << dx << "dy" << dy << "i" << i << "team" << team << "\n";
                            if (team == OUT_OF_BOUNDS || team == (piece.team))
                                break;
                            else if (team == EMPTY)
                            {
                                piece.moveList.push_back({&piece, piece.locx + dx * i, piece.locy + dy * i, MOVE});
                                // cout << "\nMOVE:" << piece.type << "team" << piece.team << "FROMx" << piece.locx << "FROMy" << piece.locy << "TOX" << piece.locx + dx * i << "toY" << piece.locx + dy * i << "\n";
                            }
                            else
                            {
                                piece.moveList.push_back({&piece, piece.locx + dx * i, piece.locy + dy * i, CAPTURE});
                                // cout << "\nMOVE:" << piece.type << "team" << piece.team << "FROMx" << piece.locx << "FROMy" << piece.locy << "TOX" << piece.locx + dx * i << "toY" << piece.locx + dy * i << "\n";
                                break;
                            }
                        }
                    }
                }
            }
        }
        else if (piece.type == KING)
        {

            int team;
            cout << "KING" << piece.locx << piece.locy;
            for (int dx = -1; dx < 2; dx++)
            {
                for (int dy = -1; dy < 2; dy++)
                {
                    if (!(dx == 0 && dy == 0))
                    {

                        getPiece(team, nullptr, piece.locx + dx, piece.locy + dy);
                        // cout << "dx" << dx << "dy" << dy << "i" << i << "team" << team << "\n";
                        if (team == OUT_OF_BOUNDS || team == (piece.team))
                            continue;
                        else if (team == EMPTY)
                        {
                            piece.moveList.push_back({&piece, piece.locx + dx, piece.locy + dy, MOVE});
                            // cout << "\nMOVE:" << piece.type << "team" << piece.team << "FROMx" << piece.locx << "FROMy" << piece.locy << "TOX" << piece.locx + dx * i << "toY" << piece.locx + dy * i << "\n";
                        }
                        else
                        {
                            piece.moveList.push_back({&piece, piece.locx + dx, piece.locy + dy, CAPTURE});
                            // cout << "\nMOVE:" << piece.type << "team" << piece.team << "FROMx" << piece.locx << "FROMy" << piece.locy << "TOX" << piece.locx + dx * i << "toY" << piece.locx + dy * i << "\n";
                            continue;
                        }
                    }
                }
            }

            //checkcheck bcoz castle can't capture the king
            if (!piece.isMoved && !isCheck(piece) && checkCheck)
            {

                int team;
                int castle = true;
                Piece p = piece;
                Piece rook;

                if (isCheck(p))
                    castle = false;

                p.locx = piece.locx + 1;
                getPiece(team, nullptr, p.locx, p.locy);
                if (team != EMPTY)
                    castle = false;
                else
                {
                    ChessBoard c = *this;
                    Move move = {&p, p.locx, p.locy, MOVE};
                    c.movePiece(move);
                    c.checkNextMove(1 - p.team, false);
                    if (c.isCheck(p))
                        castle = false;
                }

                p.locx = piece.locx + 2;
                getPiece(team, nullptr, p.locx, p.locy);
                if (team != EMPTY)
                    castle = false;
                else
                {
                    ChessBoard c = *this;
                    Move move = {&p, p.locx, p.locy, MOVE};
                    c.movePiece(move);
                    c.checkNextMove(1 - p.team, false);
                    if (c.isCheck(p))
                        castle = false;
                }

                p.locx = piece.locx + 3;
                p.locy = piece.locy;
                getPiece(team, rook, p.locx, p.locy);
                if (rook.type != ROOK || piece.team != team)
                    castle = false;

                if (castle)
                    piece.moveList.push_back({&piece, piece.locx + 2, piece.locy, CASTLE});

                team = -3;
                castle = true;
                p = piece;

                if (isCheck(p))
                    castle = false;

                p.locx = piece.locx - 1;
                getPiece(team, nullptr, p.locx, p.locy);
                if (team != EMPTY)
                    castle = false;
                else
                {
                    ChessBoard c = *this;
                    Move move = {&p, p.locx, p.locy, MOVE};
                    c.movePiece(move);
                    c.checkNextMove(1 - p.team, false);
                    if (c.isCheck(p))
                        castle = false;
                }

                p.locx = piece.locx - 2;
                getPiece(team, nullptr, p.locx, p.locy);
                if (team != EMPTY)
                    castle = false;
                else
                {
                    ChessBoard c = *this;
                    Move move = {&p, p.locx, p.locy, MOVE};
                    c.movePiece(move);
                    c.checkNextMove(1 - p.team, false);
                    if (c.isCheck(p))
                        castle = false;
                }

                // TODO QUEENSIDE CASTLING EVEN IF CHECKED IN BW
                p.locx = piece.locx - 3;
                p.locy = piece.locy;
                getPiece(team, nullptr, p.locx, p.locy);
                if (team != EMPTY)
                    castle = false;
                else
                {
                    ChessBoard c = *this;
                    Move move = {&p, p.locx, p.locy, MOVE};
                    c.movePiece(move);
                    c.checkNextMove(1 - p.team, false);
                    if (c.isCheck(p))
                        castle = false;
                }

                p.locx = piece.locx - 4;
                p.locy = piece.locy;
                getPiece(team, rook, p.locx, p.locy);
                if (rook.type != ROOK || piece.team != team)
                    castle = false;

                if (castle)
                    piece.moveList.push_back({&piece, piece.locx - 2, piece.locy, CASTLE});
            }
        }

        vector<Move> toBeRemoved;
        for (auto &i : piece.moveList)
        {

            if (!checkCheck)
                moveList.push_back(i);
            else
            {
                ChessBoard c = *this;
                // c.moveList.clear();
                c.movePiece(i);
                c.checkNextMove(1 - piece.team, false);
                Piece king;
                c.getKing(piece.team, king);
                // cout << (piece.team == WHITE ? "WHITE" : "BLACK") << "KING " << king.locx << king.locy;
                if (!c.isCheck(king))
                {
                    // cout << "\n\nX" << i.piece->locx << "Y" << i.piece->locy << "toX" << i.toLocX << "toY" << i.toLocY;
                    // cout << "NOOOOOOOOO CHECKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKk\n";
                    moveList.push_back(i);
                }
                else
                {
                    // cout << "\n\nX" << i.piece->locx << "Y" << i.piece->locy << "toX" << i.toLocX << "toY" << i.toLocY;
                    // cout << "CHECKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKk\n";
                    toBeRemoved.push_back(i);
                }
            }
        }
        // DO NOT REMOVE toBeRemoved, keeping it inside the for loop returns confusing errors
        for (auto &i : toBeRemoved)
            piece.moveList.erase(std::remove_if(piece.moveList.begin(), piece.moveList.end(), [&i](const Move m) -> bool
                                                { return i == m; }));
        toBeRemoved.clear();
    }

    void movePiece(Move &move)
    {
        cout << "\nTYPE\n";
        switch (move.moveType)
        {
        case MOVE:
            cout << "MOVE\n";
            break;
        case CASTLE:
            cout << "CASTLE\n";
            break;
        case ENPASSANT:
            cout << "ENPASSANT\n";
            break;
        case CAPTURE:
            cout << "CAPTURE\n";
            break;
        case NEWPIECE:
            cout << "NEW\n";
            break;
        case PAWN_DOUBLE_PUSH:
            cout << "PDP\n";
            break;
        }
        // Move i = move;
        // cout << "\nPIECE:" << i.piece->type;
        // cout << "\nTEAM:" << i.piece->team;
        // cout << "\nFRom:" << i.piece->locx << i.piece->locy;
        // cout << "\nTO:" << i.toLocX << i.toLocY;

        int pieceTeam = -100;
        int index = getPieceIndex(move.piece->locx, move.piece->locy, pieceTeam);
        // cout << "\nMOVE:" << move.piece->type << "team" << move.piece->team << "PT" << pieceTeam << "FROMx" << move.piece->locx << "FROMy" << move.piece->locy << "TOX" << move.toLocX << "toY" << move.toLocY << "\n";
        if (index == -1)
        {
            cout << "\n I" << pieceTeam;
            cout << "\n I\n";
            cout << "\n I\n";
            cout << "\n I\n";
            cout << "\n I\n";
            cout << "\n I\n";
            cout << "\n I\n";
        }
        if (pieceTeam == WHITE)
        {
            if (whitePieces.at(index).type != move.piece->type)
            {
                cout << "\nFUCKED\n";
            }
            whitePieces.at(index).locx = move.toLocX;
            whitePieces.at(index).locy = move.toLocY;
            whitePieces.at(index).isMoved = MOVED;
            if (move.moveType == ENPASSANT)
            {
                int q = getBlackPieceIndex(move.toLocX, move.toLocY - 1, pieceTeam);
                // cout << "TOLOCX" << move.toLocX << "TOLOCY" << move.toLocY << "\n";
                blackPieces.erase(blackPieces.begin() + q);
            }
            else if (move.moveType == CAPTURE)
            {
                int q = getBlackPieceIndex(move.toLocX, move.toLocY, pieceTeam);
                // cout << blackPieces.at(q).locx << blackPieces.at(q).locy << "PT" << pieceTeam << "INDEX" << q << "TOLOCX" << move.toLocX << "TOLOCY" << move.toLocY << "\n";
                blackPieces.erase(blackPieces.begin() + q);
                // cout << "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ" << q << "\n
            }
            else if (move.moveType == CASTLE)
            {
                if (move.toLocX == 6)
                {
                    cout << "\n\nWHITE CASTLE KINGSIDE\n\n";
                    int q = getWhitePieceIndex(7, 0, pieceTeam);
                    whitePieces.at(q).locx = 5;
                    whitePieces.at(q).locy = 0;
                    whitePieces.at(q).isMoved = MOVED;
                }
                else if (move.toLocX == 2)
                {
                    cout << "\n\nWHITE CASTLE QUEENSIDE\n\n";
                    int q = getWhitePieceIndex(0, 0, pieceTeam);
                    whitePieces.at(q).locx = 3;
                    whitePieces.at(q).locy = 0;
                    whitePieces.at(q).isMoved = MOVED;
                }
            }

            // cout << "\nPIECE TEAM IS WHITE\n";
        }
        else if (pieceTeam == BLACK)
        {
            if (blackPieces.at(index).type != move.piece->type)
            {
                cout << "\nFUCKED\n";
            }
            blackPieces.at(index).locx = move.toLocX;
            blackPieces.at(index).locy = move.toLocY;
            blackPieces.at(index).isMoved = MOVED;
            if (move.moveType == ENPASSANT)
            {
                int q = getWhitePieceIndex(move.toLocX, move.toLocY + 1, pieceTeam);
                whitePieces.erase(whitePieces.begin() + q);
            }
            else if (move.moveType == CAPTURE)
            {
                int q = getWhitePieceIndex(move.toLocX, move.toLocY, pieceTeam);
                whitePieces.erase(whitePieces.begin() + q);
            }
            else if (move.moveType == CASTLE)
            {
                if (move.toLocX == 6)
                {
                    int q = getBlackPieceIndex(7, 7, pieceTeam);
                    blackPieces.at(q).locx = 5;
                    blackPieces.at(q).locy = 7;
                    blackPieces.at(q).isMoved = MOVED;
                }
                else if (move.toLocX == 2)
                {
                    int q = getBlackPieceIndex(0, 7, pieceTeam);
                    blackPieces.at(q).locx = 3;
                    blackPieces.at(q).locy = 7;
                    blackPieces.at(q).isMoved = MOVED;
                }
            }
            // cout << "\nPIECE TEAM IS BLACK\n";
        }
    }

    void checkNextMove(int nextTurn, bool checkCheck)
    {
        moveList.clear();

        if (nextTurn == BLACK)
        {
            for (auto &p : blackPieces)
            {
                getPossibleMoves(p, checkCheck);
            }
        }
        else if (nextTurn == WHITE)
        {
            for (auto &p : whitePieces)
            {
                getPossibleMoves(p, checkCheck);
            }
        }
    }

    void playTheBestMove(int nextTurn)
    {

        checkNextMove(nextTurn, true);
        if (playersTeam != nextTurn)
        {
            movePiece(moveList.at(0));
            previousMove = moveList.at(0);
        }
        else
        {
            cout << "next trun is palyer 2";
        }
    }
};
