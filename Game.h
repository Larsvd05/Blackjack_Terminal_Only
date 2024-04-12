//
// Created by lars on 18/01/24.
//

#ifndef BLACKJACK_GAME_H
#define BLACKJACK_GAME_H

#include <cstdint>
#include <vector>
#include <memory>
#include <ctime>
#include "Card.h"
#include "Player.h"
#include "Dealer.h"
#include <iostream>

class Game{
public:
    Game();

    uint8_t getRonde() const;
    void setRonde(uint8_t ronde);

    void setupGame();
    void resetGame();
    void run();
    void startGame();
    void stop();

    uint16_t getTotalPlayingCards();
    const std::vector<std::shared_ptr<Card>> &getSpeelkaarten() const;
    void setSpeelkaarten(const std::vector<std::shared_ptr<Card>> &speelkaarten);
    void generateSpeelkaarten();
    void vraagVoorInput();
    std::string vraagInput(uint8_t promptNummer); // Werkt alleen voor nummer, vanwege een charchecker die controleert of er geen letters in zitten.
    uint16_t processDealerEinde();
    void processInput(std::string userInput);

    void printKaarten();
    void printSpelers();
    std::shared_ptr<Card> trekKaart();
    void addSpeler(std::string aNaam);
    void addKaart(uint8_t waarde, std::string naam, std::string vorm);
    void setSpeler();
    void setDealer();
private:
    std::shared_ptr<Dealer> dealer;
    std::vector<std::shared_ptr<Card>> speelkaarten;
    std::vector<std::shared_ptr<Player>> spelers;
    bool gameFinished = false;
    uint8_t ronde = 0;
    bool stopGame = false;
};

#endif //BLACKJACK_GAME_H
