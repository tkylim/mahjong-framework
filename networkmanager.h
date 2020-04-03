#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

#include "globals.h"
#include "hand.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

signals:
    // These are called on client side only
    void PostRenderRequest(PlayerId p, game::Hand hand, RenderCallbackType type, game::Tile tile);
    void PostMeldOffer(PlayerId p, Melds meldOptions);
    void PostMeldAccepted(game::Meld meld);
    void PostMeldDeclined();

public slots:
    // These are received from the game controller on the server side
    void ReceiveRenderRequest(PlayerId p, game::Hand hand, RenderCallbackType type, game::Tile tile);
    void ReceiveMeldOffer(PlayerId p, Melds meldOptions);
    void ReceiveMeldAccepted(game::Meld meld);
    void ReceiveMeldDeclined();
};

#endif // NETWORKMANAGER_H
