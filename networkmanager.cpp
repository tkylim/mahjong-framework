#include "networkmanager.h"

#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {

}

void NetworkManager::ReceiveRenderRequest(PlayerId p, game::Hand hand, RenderCallbackType type, game::Tile tile) {
    // TODO:  Networking
    emit PostRenderRequest(p, hand, type, tile);
}

void NetworkManager::ReceiveMeldOffer(PlayerId p, Melds meldOptions) {
    // TODO:  Networking
    emit PostMeldOffer(p, meldOptions);
}

void NetworkManager::ReceiveMeldAccepted(game::Meld meld) {
    // TODO:  Networking
    emit PostMeldAccepted(meld);
}

void NetworkManager::ReceiveMeldDeclined() {
    // TODO:  Networking
    emit PostMeldDeclined();
}
