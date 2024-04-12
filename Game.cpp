//
// Created by lars on 18/01/24.
//
#include "Game.h"
#include "Player.h"
#include <memory>
#include <cmath>
#include <chrono>

Game::Game() {
    srand(time(NULL)); // Seed the random number generator
}

void Game::run() {
    vraagInput(0);
    while(!stopGame) {
        startGame();
        generateSpeelkaarten();
        resetGame();
    }
}

void Game::startGame() {
    vraagInput(1);
    if(!gameFinished) {
        std::shared_ptr<Card> kaart;
        kaart = trekKaart();
        spelers.front()->addKaart(kaart, true);
        kaart = trekKaart();
        dealer->addKaart(kaart, true);
        kaart = trekKaart();
        spelers.front()->addKaart(kaart, true);
        if (spelers.front()->getWaardeKaarten() > 21 && spelers.front()->checkForAce()) {
            spelers.front()->setWaardeKaart("A", 1);
        }
        if (dealer->getWaardeKaarten() > 21 && dealer->checkForAce()) {
            dealer->setWaardeKaart("A", 1);
        }
//          TODO-TEST zorg dat ook bij de dealer de Ace tussen 1 en 11 kan switchen.
        if (spelers.front()->checkFor21()) {
            std::cout << "[Game] - Je hebt gelijk 21 gegooid en hebt 2x je inzet gekregen!" << std::endl;
            gameFinished = true;
            spelers.front()->setTotaleInzet(spelers.front()->getTotaleInzet() + spelers.front()->getInzet()*2);
            std::cout << "[" << spelers.front()->getNaam() << "] - Jouw totale inzetpot bestaat nu uit €"
                      << spelers.front()->getTotaleInzet()
                      << "." << std::endl << std::endl;
        } else {

            kaart = trekKaart();
            dealer->addKaart(kaart);
            std::cout << "[" << spelers.front()->getNaam() << "] - Jouw kaarten zijn:";
            spelers.front()->printKaarten();
            std::cout << " met een totale waarde van " << spelers.front()->getWaardeKaarten() << "." << std::endl;

            while (!gameFinished && !stopGame) {
                vraagVoorInput();
            }
        }
    }

}

void Game::stop() {
    std::cout << "[" << spelers.front()->getNaam() << "] - Het spel is beëindigd en jouw totale inzet is nu "
    << spelers.front()->getTotaleInzet()
    << "." << std::endl;
    stopGame = true;
}

void Game::addSpeler(std::string aNaam) {
    spelers.emplace_back(std::make_shared<Player>(Player(aNaam)));

}

void Game::addKaart(uint8_t waarde, std::string naam, std::string vorm) {
    speelkaarten.emplace_back(std::make_shared<Card>(Card(waarde, naam, vorm)));
}

uint8_t Game::getRonde() const {
    return ronde;
}

void Game::setRonde(uint8_t aRonde) {
    ronde = aRonde;
}

void Game::setupGame() {
    generateSpeelkaarten();
    setSpeler();
    setDealer();
}

const std::vector<std::shared_ptr<Card>> &Game::getSpeelkaarten() const {
    return speelkaarten;
}

void Game::printKaarten(){
    for(auto kaart : speelkaarten){
        std::cout << kaart->toString() << std::endl;
    }
}

void Game::resetGame() {
    for(auto kaart : speelkaarten){
    kaart->setAlOpgegooid(false);
    }
    for(auto speler : spelers){
        speler->resetKaarten();
    }
    dealer->resetKaarten();
    gameFinished = false;
}

void Game::printSpelers() {
    for(auto speler : spelers){
        std::cout << speler->toString() << std::endl;
    }
}



std::shared_ptr<Card> Game::trekKaart() {
    if (getTotalPlayingCards() == 0) {
        std::cerr << "[GameError] - Er zijn geen kaarten meer in de pot!" << std::endl;
        return nullptr; // Geen kaarten meer om te trekken
    }
    uint16_t randomNummer;
    randomNummer = rand() % getTotalPlayingCards();
    std::shared_ptr<Card> gekozenKaart;
    gekozenKaart = speelkaarten.at(randomNummer);

    while (gekozenKaart->isAlOpgegooid()){ // Ga door tot je een kaart vindt die nog niet opgegooid is.
        randomNummer = rand() % getTotalPlayingCards();
        std::cout << "randomnummer: " + randomNummer << std::endl;
        gekozenKaart = speelkaarten[randomNummer];
    }


    return gekozenKaart;
}

uint16_t Game::getTotalPlayingCards() {
    uint16_t totalPlayingCards = 0;
    for(auto kaart : speelkaarten){
        if(!kaart->isAlOpgegooid()){
            ++totalPlayingCards;
        }
    }
    return totalPlayingCards;
}

void Game::setDealer() {
    Dealer dealer1;
    dealer = std::make_shared<Dealer>(dealer1);
}

void Game::generateSpeelkaarten() {
    std::string vorm = "♥";
    std::string naam = "J";
    uint8_t waarde = 1;
    uint8_t iterator = 0;
    for(int i = 0; i < 52; i++){
        uint8_t colorNumber = std::floor((i+1)/14);
        switch (colorNumber){
            case 0:
                vorm = "♥";
                if(waarde == 11){
                    naam = "A";
                    addKaart(waarde, naam, vorm);
                    waarde = 1;
                } else if(waarde <= 9){
                    ++waarde;
                    naam = std::to_string(waarde);
                    addKaart(waarde, naam, vorm);
                } else if (waarde == 10){
                    switch(iterator){
                        case 0:
                            naam = "J";
                            ++iterator;
                            addKaart(waarde, naam, vorm);
                            break;
                        case 1:
                            naam = "Q";
                            ++iterator;
                            addKaart(waarde, naam, vorm);
                            break;
                        case 2:
                            naam = "K";
                            iterator = 0;
                            addKaart(waarde, naam, vorm);
                            ++waarde;
                            break;
                    }
                }
                break;
            case 1:
                vorm = "♦";
                if(waarde == 11){
                    naam = "A";
                    addKaart(waarde, naam, vorm);
                    waarde = 1;
                } else if(waarde <= 9){
                    ++waarde;
                    naam = std::to_string(waarde);
                    addKaart(waarde, naam, vorm);
                } else if (waarde == 10){
                    switch(iterator){
                        case 0:
                            naam = "J";
                            ++iterator;
                            addKaart(waarde, naam, vorm);
                            break;
                        case 1:
                            naam = "Q";
                            ++iterator;
                            addKaart(waarde, naam, vorm);
                            break;
                        case 2:
                            naam = "K";
                            iterator = 0;
                            addKaart(waarde, naam, vorm);
                            ++waarde;
                            break;
                    }
                }
                break;
            case 2:
                vorm = "♠";
                if(waarde == 11){
                    naam = "A";
                    addKaart(waarde, naam, vorm);
                    waarde = 1;
                } else if(waarde <= 9){
                    ++waarde;
                    naam = std::to_string(waarde);
                    addKaart(waarde, naam, vorm);
                } else if (waarde == 10){
                    switch(iterator){
                        case 0:
                            naam = "J";
                            ++iterator;
                            addKaart(waarde, naam, vorm);
                            break;
                        case 1:
                            naam = "Q";
                            ++iterator;
                            addKaart(waarde, naam, vorm);
                            break;
                        case 2:
                            naam = "K";
                            iterator = 0;
                            addKaart(waarde, naam, vorm);
                            ++waarde;
                            break;
                    }
                }                break;
            case 3:
                vorm = "♣";
                if(waarde == 11){
                    naam = "A";
                    addKaart(waarde, naam, vorm);
                    waarde = 1;
                } else if(waarde <= 9){
                    ++waarde;
                    naam = std::to_string(waarde);
                    addKaart(waarde, naam, vorm);
                } else if (waarde == 10){
                    switch(iterator){
                        case 0:
                            naam = "J";
                            ++iterator;
                            addKaart(waarde, naam, vorm);
                            break;
                        case 1:
                            naam = "Q";
                            ++iterator;
                            addKaart(waarde, naam, vorm);
                            break;
                        case 2:
                            naam = "K";
                            iterator = 0;
                            addKaart(waarde, naam, vorm);
                            ++waarde;
                            break;
                    }
                }
                break;
            default:

                break;
        }
    }
}

void Game::setSpeler() {
    std::string naamSpeler = "Lars";
//    std::cout << "[Input] - Vul hier uw speelnaam in: ";
//    getline(std::cin, naamSpeler);
    addSpeler(naamSpeler);
    std::cout << "[Game] - U bent succesvol in het spel gezet met de naam " + naamSpeler + "!" << std::endl;
}

void Game::processInput(std::string userInput) {
    std::string input = userInput;
    std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    if(input == "hit" || input == "h"){
        std::shared_ptr<Card> kaart = trekKaart();
        spelers.front()->addKaart(kaart, true);
        uint16_t waardeKaartenSpeler = spelers.front()->getWaardeKaarten();
        if(waardeKaartenSpeler > 21 && spelers.front()->checkForAce()){
            spelers.front()->setWaardeKaart("A", 1);
            std::cout << "[" << spelers.front()->getNaam() << "] - Jouw kaarten zijn nu:";
            spelers.front()->printKaarten();
            std::cout << " met een totale waarde van " << spelers.front()->getWaardeKaarten() << "." << std::endl << std::endl;
        } else if(waardeKaartenSpeler > 21){
            std::cout << "[" << spelers.front()->getNaam() << "] - Je hebt helaas verloren met een totale waarde van " << waardeKaartenSpeler << " en jouw inzet wordt nu gebruikt om de slaven in dit casino te betalen, want anders hebben we geen elektriciteit meer." << std::endl;
            gameFinished = true;
            std::cout << "[" << spelers.front()->getNaam() << "] - Jouw totale inzetpot bestaat nu uit €"
            << spelers.front()->getTotaleInzet()
            << "." << std::endl << std::endl;
        } else {
            std::cout << "[" << spelers.front()->getNaam() << "] - Jouw kaarten zijn nu:";
            spelers.front()->printKaarten();
            std::cout << " met een totale waarde van " << spelers.front()->getWaardeKaarten() << "." << std::endl << std::endl;
        }
    } else if(input == "stand" || input == "s"){
        processDealerEinde();
    } else if(input == "double" || input == "d"){
        if(spelers.front()->getTotaleInzet() > spelers.front()->getInzet()) {
            spelers.front()->setTotaleInzet(spelers.front()->getTotaleInzet() - spelers.front()->getInzet());
            spelers.front()->setInzet(spelers.front()->getInzet() * 2);
            spelers.front()->addKaart(trekKaart(), true);
            if(spelers.front()->getWaardeKaarten() > 21 && spelers.front()->checkForAce()){
                spelers.front()->setWaardeKaart("A", 1);
            } else if(spelers.front()->getWaardeKaarten() > 21){
                std::cout << "[" << spelers.front()->getNaam() << "] - Je hebt helaas verloren met een totale waarde van " << spelers.front()->getWaardeKaarten() << " en jouw inzet wordt nu gebruikt om de slaven in dit casino te betalen, want anders hebben we geen elektriciteit meer." << std::endl;
                gameFinished = true;
                std::cout << "[" << spelers.front()->getNaam() << "] - Jouw totale inzetpot bestaat nu uit €"
                          << spelers.front()->getTotaleInzet()
                          << "." << std::endl << std::endl;
                return;
            }
            std::cout << "[" << spelers.front()->getNaam() << "] - Jouw kaarten zijn nu:";
            spelers.front()->printKaarten();
            std::cout << " met een totale waarde van " << spelers.front()->getWaardeKaarten() << "." << std::endl << std::endl;
            processDealerEinde();
        } else {
            std::cout << "[Input] - Dit is een ongeldige invoer, want je hebt niet genoeg totale inzet om dit te proberen. Wee alsnog maar blij, want je hebt nooit geen geld, kankerboef!";
        }
    } else if(input == "split" || input == "spl"){

    } else if(input == "help" || input == "hel"){
        std::cout << "[Help] - Je hebt nu de keuze om te Hitten ('H'), Standen ('S'), Verdubbelen ('D'), Splitten ('SPL') of Stoppen ('STP')" << std::endl
        << "[Hit] - Speler pakt een nieuwe kaart en kan hierna opnieuw kiezen wat hij gaat doen." << std::endl
        << "[Stand] - Speler wil geen nieuwe kaarten meer en houdt het bij deze kaarten. Als deze optie gekozen is, maakt de dealer de ronde af." << std::endl
        << "[Double] - Speler verdubbelt zijn inzet, pakt 1 kaart en hierna maakt de dealer de ronde af." << std::endl
        << "[Split] - Geen idee hoe dit werkt tbh." << std::endl
        << "[Stop] - Voer dit commando uit om per direct het spel te stoppen." << std::endl;
    } else if(input == "stop" || input == "stp"){
        stop();
    } else if(input == "win" || input == "w" ){
        spelers.front()->resetKaarten();
        spelers.front()->addKaart(std::make_shared<Card>(Card(10, "K", "♥", true)),false);
        spelers.front()->addKaart(std::make_shared<Card>(Card(11, "A", "♥" , true)), false);
        processDealerEinde();
    } else {
        std::cout << "[Error] - Dit commando is niet bekend of verkeerd getypt, kijk of je dit goed getypt hebt en probeer opnieuw." << std::endl;
    }
}

void Game::vraagVoorInput() {
    std::string input;
    std::cout << "[Game] - Kies nu wat je wilt doen met deze kaarten, de keuze bestaat uit: Hit 'H',  Stand 'S', Double 'D' of Split 'SPL'. Type Help voor hulp." << std::endl;
    std::cin >> input;
    processInput(input);
}

uint16_t Game::processDealerEinde() {
    std::shared_ptr<Card> kaart;
    uint16_t waardeKaartenDealer;
    while(!gameFinished) {
        std::cout << "[Dealer] - De kaarten van de dealer zijn:";
        dealer->printKaarten();
        std::cout << "met een waarde van " << dealer->getWaardeKaarten() << "."<< std::endl;
        waardeKaartenDealer = dealer->getWaardeKaarten();
        if (waardeKaartenDealer <= 16) {
            kaart = trekKaart();
            dealer->addKaart(kaart, true);
        } else if(waardeKaartenDealer >= 17 && waardeKaartenDealer <= 21){
            if(waardeKaartenDealer > spelers.front()->getWaardeKaarten()){
                std::cout << std::endl << "[Game] - De dealer heeft gewonnen met de volgende kaarten: ";
                dealer->printKaarten();
                std::cout << std::endl << "[Game] - Dit geeft een waarde van " << dealer->getWaardeKaarten() << " in vergelijking met de waarde " << spelers.front()->getWaardeKaarten() << " van de speler." << std::endl;
                spelers.front()->setInzet(0);
                gameFinished = true;
            } else if (waardeKaartenDealer < spelers.front()->getWaardeKaarten()){
                std::cout << std::endl << "[Game] - Dit geeft een waarde van " << spelers.front()->getWaardeKaarten() << " in vergelijking met de waarde " << dealer->getWaardeKaarten() << " van de dealer.";
                std::cout << std::endl << "[Game] - De speler heeft dus gewonnen met de volgende kaarten: ";
                spelers.front()->printKaarten();
                std::cout << std::endl;
                spelers.front()->setTotaleInzet(spelers.front()->getTotaleInzet() + (spelers.front()->getInzet()*2));
                gameFinished = true;
            } else {
                std::cout << std::endl << "[Game] - Gelijkspel" << std::endl;
                spelers.front()->setTotaleInzet(spelers.front()->getTotaleInzet() + (spelers.front()->getInzet()));
                spelers.front()->setInzet(0);
                gameFinished = true;
            }
        } else if(waardeKaartenDealer > 21){
            std::cout << std::endl << "[Game] - De speler heeft gewonnen omdat de dealer over de 21 is gegaan met de volgende kaarten:";
            dealer->printKaarten() ;
            std::cout << "met een waarde van " << dealer->getWaardeKaarten() << "." <<  std::endl;
            spelers.front()->setTotaleInzet(spelers.front()->getTotaleInzet() + (spelers.front()->getInzet()*2));
            gameFinished = true;
        }
    }
    std::cout << "[" << spelers.front()->getNaam() << "] - Jouw totale inzetpot bestaat nu uit €"
    << spelers.front()->getTotaleInzet()
    << "." << std::endl << std::endl;
    return 0;
}

std::string Game::vraagInput(uint8_t promptNummer) {
    uint64_t inputInt;
    std::string input;

    while(true) {
        std::cout << std::endl;
        switch (promptNummer) {
            case 0:
                std::cout
                        << "[Input] - Vul alstublieft uw totale geldinzet hier in (zonder spaties, punten of komma's): " << std::endl;
                break;
            case 1:
                std::cout
                        << "[Game] - Vul alstublieft uw inzet voor deze ronde hier in (zonder spaties, punten of komma's): " << std::endl;
                break;
        }
        std::cin >> input;
//        input = "100";
        if (input == "stop" || input == "stp") {
            stop();
            return "0";
        } else {
            bool isValid = true;
            for (char c: input) {
                if (!std::isdigit(c)) {
                    isValid = false;
                    break;
                }
            }

            if (isValid) {
                bool geldigAantal = false;
                try {
                    inputInt = std::stoull(input);
                    switch (promptNummer) {
                        case 0:
                            spelers.front()->setTotaleInzet(inputInt);
                            std::cout << "[" << spelers.front()->getNaam()
                                      << "] - Jouw totale inzet voor het spel is nu: €"
                                      << spelers.front()->getTotaleInzet()
                                      << ", hopelijk heb je zin om te beginnen!" << std::endl << std::endl;
                            geldigAantal = true;
                            break;
                        case 1:
                            if(spelers.front()->getTotaleInzet() >= inputInt) {
                                spelers.front()->setInzet(inputInt);
                                spelers.front()->setTotaleInzet(spelers.front()->getTotaleInzet() - inputInt);
                                std::cout << "[" << spelers.front()->getNaam()
                                          << "] - Jouw inzet voor deze ronde is nu: €"
                                          << spelers.front()->getInzet()
                                          << " en jouw totale inzet is nog €" << spelers.front()->getTotaleInzet()
                                          << ", succes!" << std::endl << std::endl;
                                geldigAantal = true;
                            } else {
                                std::cout << "[Input] Dit is een ongeldige invoer, want uw totale inzet kan niet lager dan 0 worden." << std::endl;
                                geldigAantal = false;
                            }
                            break;
                    }
                } catch (const std::exception &e) {
                    std::cerr << "[Game] - Fout: " << e.what() << "\n";
                }
                if(geldigAantal) {
                    return input;
                }
            } else {
                std::cout << "[Input] - Ongeldige invoer. Probeer het opnieuw.\n";
            }
        }
    }
}





